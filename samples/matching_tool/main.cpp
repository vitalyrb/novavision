#include <opencv2/opencv.hpp>

#include <forms/forms.hpp>
#include <core/core.hpp>

#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;



class ToolWidow: public nv::ui::Form
{
public:
    ToolWidow(std::string const& title, cv::Mat const &rm, cv::Mat &dm, cv::Mat &sm): nv::ui::Form(title), rm_(rm), dm_(dm), sm_(sm){};
    void on_rbutton_down(int x, int y, int) override
    {
        next(m_, rm_, {x, y});
        // std::vector<float> corr_arr(0);
        // corr_arr.reserve(m_.cols);
        // auto right_point = nv::find_matching_point(m_, rm_, cv::Point(x, y), [&corr_arr](nv::metric const& m, nv::descriptor const&, nv::descriptor const&)
        // {
        //     corr_arr.push_back(m.coeff());
        // });

        // nv::ui::Plot<float> plot({640/2, 480/2}, 10, "correlation graphic", cv::Scalar().all(0));        
        // plot.draw(corr_arr, cv::Scalar(255, 0, 0), 1, 5, 50);

        // cv::namedWindow("PLOT_WINDOW", 0);// cv::WINDOW_NORMAL);
        // cv::imshow("PLOT_WINDOW", plot.image());
        // //cv::waitKey(0);             

        // auto view = rm_.clone();
        // cv::circle(view, right_point, 10, 0, 2);
        // cv::namedWindow("Right Point", cv::WINDOW_NORMAL);
        // cv::imshow("Right Point", view);
        // cv::waitKey(0);
        // cv::destroyWindow("PLOT_WINDOW");
        // cv::destroyWindow("Right Point");
    }
    void next(cv::Mat const& lm, cv::Mat const& rm, cv::Point const& lp)
    {
        
        cv::Size plot_size{320, 240};
        nv::ui::Form im_view("Sequence Graphic", {640*2, 480});
        nv::ui::Form values_view("Values Graphic", plot_size);        
        nv::ui::Form corr_view("Correlation Graphic", plot_size);        

        corr_view.move({0, 0});
        im_view.move({0, corr_view.size().height});
        values_view.move({corr_view.size().width, 0});

        nv::ui::Plot<float> plot(plot_size, 10, "correlation graphic", cv::Scalar().all(0));        
        nv::ui::Plot<uint8_t> values_plot(plot_size, 10, "values graphic", cv::Scalar().all(0));
        std::vector<float> corr_graph;
        corr_graph.reserve(static_cast<size_t>(lm.cols));        
        nv::fibonacci_sequence sequence(64, 100, 0.5);
        nv::descriptor ld(lm, lp, sequence);
        nv::metric best_metric(0.001, cv::Point(-1, -1));
        for(int x = std::max(0, lp.x - 256); x < lp.x; ++x)
        {
            cv::Point rp{x, lp.y};
            nv::descriptor rd(rm, rp, ld.permitted_sequence());
            auto curr_metric = correlation_compare(ld, rd);
            best_metric = max(best_metric, curr_metric);

            corr_graph.push_back(curr_metric.coeff());
            plot.clear();
            plot.draw(corr_graph, cv::Scalar(255, 0, 0), 2, 1.5, 100.);

            values_plot.clear();
            values_plot.draw(ld.values(), cv::Scalar(255, 0, 0), 1, 3., 1.);
            values_plot.draw(rd.values(), cv::Scalar(0, 0, 255), 1, 3., 1., 20);

            values_view.show(values_plot.image());

            
            auto lft_im = lm({lp.x - 50, lp.y - 50, 100, 100}).clone();
            auto rht_im = rm({rp.x - 50, rp.y - 50, 100, 100}).clone();
            draw(lft_im, ld.permitted_sequence());
            draw(rht_im, rd.permitted_sequence());
            cv::Mat im;
            cv::hconcat(lft_im, rht_im, im);
            //nv::ui::Form im_view("Sequence Graphic", {640*2, 480});
            im_view.show(im);
            //nv::ui::Form corr_view("Correlation Graphic", plot_size);
            corr_view.show(plot.image());
            cv::waitKey(0); 
        }
        im_view.destroy();
        corr_view.destroy();
        values_view.destroy();

        best_metric.target_point();        

    }
protected:
    cv::Mat const &rm_;
    cv::Mat dm_;
    cv::Mat sm_;
};


const std::string resource_path = "/home/bvi/projects/novavision/resources/";

template<class ForwardIt, class Size, class T>
ForwardIt search_n_(ForwardIt first, ForwardIt last,
                          Size count, const T& value)
{
    Size curr_count = 0;
    ForwardIt result, t_last = first;
    auto p = std::distance(first, last);
    std::advance(t_last, p - count + 1);
 
    for (; first != t_last; first++) {
        curr_count = 0;
        result = first;
        while (*first == value) {
            curr_count++;
            if (curr_count == count) {
                return result;
            }
            ++first;
        }
    }
    return last;
}

//template float correlation_(std::vector<int> &v1, std::vector<int> &v2);

int main(void)
{
    try
    {
        // std::vector<int> arr1{1,3,2,4,6,5,8,7,10,9,13,11,12,17,14,15,16};
        // std::vector<int> arr2(arr1.begin(), arr1.end());

        // std::for_each(arr2.begin(), arr2.end(), [](int &n)
        // { 
        //     if(n & 1) n += 3; 
        // });

        // nv::ui::Plot<int> plot1({640/2, 480/2}, 10, "correlation graphic1", cv::Scalar().all(0));
        // nv::ui::Plot<int> plot2({640/2, 480/2}, 10, "correlation graphic2", cv::Scalar().all(0));
        
        // plot1.draw(arr1, cv::Scalar(255, 0, 0), 1, 5, 5);
        // plot2.draw(arr2, cv::Scalar(0, 0, 255), 1, 5, 5);

        // float corr = nv::correlation(arr1, arr2);

        // std::cout << "corr: " << corr << std::endl; 

        // cv::namedWindow("PLOT_WINDOW", 0);// cv::WINDOW_NORMAL);
        // cv::imshow("PLOT_WINDOW", plot.image());
        // cv::waitKey(0);
        // cv::destroyWindow("PLOT_WINDOW");

        auto lm = cv::imread(resource_path + "moto/im0.png", 0);
        if(lm.empty()) throw std::system_error(errno, std::system_category(), "moto/im0.png");
        auto rm = cv::imread(resource_path + "moto/im1.png", 0);
        if(rm.empty()) throw std::system_error(errno, std::system_category(), "moto/im1.png");
        auto dm = rm.clone();
        auto sm = lm.clone();
        ToolWidow tool("TOOL", rm, dm, sm);
        tool.show(lm);

        cv::waitKey(0);
    }
    catch(exception const& ex){
        cout << "fail: " << ex.what() << endl;
    }

    std::cout << "press enter for exit ...\n";
    getchar();

    return 0;
}