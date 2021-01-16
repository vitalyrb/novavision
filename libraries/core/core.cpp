#include "core.hpp"

#include <thread>

namespace nv
{
metric compare(descriptor const& ld, descriptor const& rd, float threshold)
{
    assert(ld.size() == rd.size());
    size_t count = 0;
    for(size_t i = 0; i < ld.size(); ++i)
    {
        std::cout << static_cast<int>(ld.at(i)) << " == " << static_cast<int>(rd.at(i)) << std::endl;
        if(ld.at(i) == rd.at(i)) count++;
    }

    return metric(static_cast<float>(count)/ld.size(), ld.target_point(), threshold);
}

static double average(uint8_t const *v, size_t size) {
    double sum = 0.0;
    for (size_t i = 0; i < size; i++) {
      sum += v[i];
    }
    return sum / size;
}
static float correlation(uint8_t const *v1, uint8_t const *v2, std::size_t size) {
    double avg1 = average(v1, size);
    double avg2 = average(v2, size);
    auto it1 = v1;
    auto it2 = v2;

    double sum = 0.0;
    double pow1 = 0.0;
    double pow2 = 0.0;

    for (size_t i = 0; i < size; i++, it1++, it2++) {
        double s1 = (*it1 - avg1);
        double s2 = (*it2 - avg2);
        sum += s1 * s2;
        pow1 += s1 * s1;
        pow2 += s2 * s2;
    }

    double correlation_coeff = sum / (std::sqrt(pow1 * pow2));

    return static_cast<float>(correlation_coeff);
}

metric correlation_compare(nv::descriptor const& ld, nv::descriptor const& rd, float threshold)
{
    auto coeff = correlation(&ld.values()[0], &rd.values()[0], ld.values().size());

    return metric(coeff, rd.target_point(), threshold);
}

cv::Point find_matching_point(cv::Mat const& lm, cv::Mat const& rm, cv::Point const &lp)
{
    nv::fibonacci_sequence sequence(64, 100, 0.5);
    nv::descriptor ld(lm, lp, sequence);
    nv::metric best_metric(0.001, cv::Point(-1, -1));
    for(int x = std::max(0, lp.x - 256); x < lp.x; ++x)
    {
        nv::descriptor rd(rm, {x, lp.y}, ld.permitted_sequence());

        auto curr_metric = correlation_compare(ld, rd);

        best_metric = max(best_metric, curr_metric);
    }

    return best_metric.target_point();
}

void build_depth_line(cv::Mat const& lm, cv::Mat const& rm, int y1, int y2, cv::Mat &depth_map)
{
    depth_map = 0;
    cv::Point lp{1464, 1333};
    for(int ly = y1; ly < (y1 + y2); ++ly)
    {
        std::cout << ly << "/" << y2 + y1 << std::endl;
        for(int lx = 0; lx < lm.cols; ++lx)
        {
            cv::Point lp{lx, ly};
            cv::Point rp = nv::find_matching_point(lm, rm, lp);        
            if(rp.x != -1)
            {
                depth_map.at<uint8_t>(lp) = std::abs(lp.x - rp.x);
            }
        }
    }
}

cv::Mat build_depth_map_multy_line(cv::Mat const& lm, cv::Mat const& rm)
{
    cv::Mat depth_map(lm.size(), lm.type());
    size_t cpu_count = std::thread::hardware_concurrency();
    size_t dy = static_cast<size_t>(lm.rows)/cpu_count;
    std::vector<std::thread> workers(cpu_count);
    for(size_t i = 0; i < cpu_count; ++i)
    {
        workers[i] = std::thread([i, dy, &lm, &rm, &depth_map](){build_depth_line(lm, rm, i*dy, dy, depth_map);});
    }

    for(auto &t: workers)
    {
        t.join();
    }

    return depth_map;
}

cv::Mat build_depth_map(cv::Mat const& lm, cv::Mat const& rm)
{
    cv::Mat depth_map(lm.size(), lm.type());
    depth_map = 0;
    cv::Point lp{1464, 1333};
    for(int ly = 0; ly < lm.rows; ++ly)
    {
        std::cout << ly << std::endl;
        for(int lx = 0; lx < lm.cols; ++lx)
        {
            cv::Point lp{lx, ly};
            cv::Point rp = nv::find_matching_point(lm, rm, lp);        
            if(rp.x != -1)
            {
                depth_map.at<uint8_t>(lp) = std::abs(lp.x - rp.x);
            }
        }
    }
 
    return depth_map;
}

}//namespace nv
