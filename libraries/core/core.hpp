#pragma once

#include <opencv2/opencv.hpp>

#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace nv
{
class limititions
{
public:
	bool is_permitte(cv::Point const&) const { return true; }	
};

class sequence
{
public:
    sequence(): vec_(0){}
	sequence(size_t size): vec_(0)
	{
		vec_.reserve(size);
	}
    sequence(const std::initializer_list<cv::Point> &list): vec_(0)
	{
        vec_.reserve(list.size());
		vec_.insert(vec_.end(), list.begin(), list.end());
	}        
	size_t size() const {return vec_.size();}
	void push(cv::Point const& p)
	{
		vec_.push_back(p);
	}
    cv::Point at(size_t i) const { return vec_.at(i); }
	std::vector<cv::Point>::iterator begin() { return vec_.begin(); }
	std::vector<cv::Point>::const_iterator end() const { return vec_.end(); }	
	std::vector<cv::Point>::const_iterator begin() const { return vec_.begin(); }
	std::vector<cv::Point>::iterator end() { return vec_.end(); }

protected:
	std::vector<cv::Point> vec_;
};

void draw(cv::Mat &m, nv::sequence const& s);

class descriptor
{
public:
	descriptor(cv::Mat const& m, cv::Point const& target_point, nv::sequence const &sequence, nv::limititions const& limititions = nv::limititions())
		: target_point_(target_point), permitted_sequence_(sequence.size())
	{
		values_.reserve(sequence.size());
		for(auto const& seq_point: sequence)
		{
			auto curr_point = target_point + seq_point;
			if(limititions.is_permitte(curr_point))
			{
				permitted_sequence_.push(seq_point);
				auto val = m.at<uint8_t>(curr_point);
				values_.push_back(val);
			}

		}
	}
	nv::sequence const& permitted_sequence() const {return permitted_sequence_;}
	std::vector<uint8_t> const& values() const {return values_;}
	cv::Point const& target_point() const {return target_point_;}
    size_t size() const { return values_.size(); }
    uint8_t at(size_t i) const {return values_.at(i);}
    cv::Point point(size_t i) const 
    {
        return target_point_ + permitted_sequence_.at(i);
    }
protected:
	const cv::Point target_point_;
	nv::sequence permitted_sequence_;
	std::vector<uint8_t> values_;
};

class metric
{
public:
    metric(float coeff, cv::Point target_point, float threshold = 0.8): coeff_(coeff), threshold_(threshold), target_point_(target_point){}
    metric(cv::Point target_point): coeff_(NAN), threshold_(0.8), target_point_(target_point){}
    operator bool()
    {
        return coeff_ > threshold_;
    }
    float coeff() const { return coeff_;}
    cv::Point target_point() const {return target_point_;}
protected:
    float coeff_;
    float threshold_;
    cv::Point target_point_;
};

metric max(metric const& m1, metric const& m2)
{
    return m1.coeff() > m2.coeff() ? m1 : m2;
}

class fibonacci_sequence: public sequence
{
    // struct point_hash 
    // {
    //     std::size_t operator()(const cv::Point& p) const 
    //     {
    //         return std::sqrt(p.x*p.x + p.y*p.y);
    //     };
    // };
    float radius_;
public:
    float radius() const {return radius_;}
    fibonacci_sequence(size_t number, float length, float step): radius_(length/2)
    {
        assert(number >= 8);
        assert(vec_.empty());
        cv::Point prev_point{-1, -1};
        auto R = length / (2 * number * step);
        size_t count = 0;
        for (int i = 0; count < number; i++) 
        {
            float fi = i * step;
            float k = R * fi;
            cv::Point curr_point;
            curr_point.x = static_cast<int>(k * cos(fi));
            curr_point.y = static_cast<int>(k * sin(fi));
            if(prev_point != curr_point)
            {          
                vec_.emplace_back(curr_point);
                prev_point = curr_point;
                ++count;  
            }
        }
        assert(adjacent_find(vec_.begin(), vec_.end()) == vec_.end());
        assert(vec_.size() == number);
    }
};

double average(uint8_t const *v, size_t size);
float correlation(uint8_t const *v1, uint8_t const *v2, std::size_t size);

metric compare(nv::descriptor const& ld, nv::descriptor const& rd, float threshold = 0.9);
metric correlation_compare(nv::descriptor const& ld, nv::descriptor const& rd, float threshold = 0.9);

cv::Point find_matching_point(cv::Mat const& lm, cv::Mat const& rm, cv::Point const &lp);

cv::Mat build_depth_map(cv::Mat const& lm, cv::Mat const& rm);
cv::Mat build_depth_map_multy_line(cv::Mat const& lm, cv::Mat const& rm);

template<typename C>
float correlation(C &v1, C &v2) 
{
    auto avg1 = std::accumulate(v1.begin(), v1.end(), 0);
    auto avg2 = std::accumulate(v2.begin(), v2.end(), 0);
    
    float sum  = 0.0;
    float pow1 = 0.0;
    float pow2 = 0.0;

    for (auto it1 = v1.begin(), it2 = v2.begin(); it1 != v1.end(); it1++, it2++) {
        float s1 = (*it1 - avg1);
        float s2 = (*it2 - avg2);
        sum  += s1 * s2;
        pow1 += s1 * s1;
        pow2 += s2 * s2;
    }

    return static_cast<float>(sum / (std::sqrt(pow1 * pow2)));
}

cv::Point find_matching_point(cv::Mat const& lm, cv::Mat const& rm, cv::Point const &lp, std::function<void (metric const&, nv::descriptor const&, nv::descriptor const&)> const& each_loop);
}//namespace nv



