#include <opencv2/opencv.hpp>

#include <forms/forms.hpp>
#include <core/core.hpp>

#include <iostream>

using namespace std;

class ToolWidow: public nv::ui::Form
{
public:
    ToolWidow(std::string const& title, cv::Mat const &rm, cv::Mat &dm, cv::Mat &sm): nv::ui::Form(title), rm_(rm), dm_(dm), sm_(sm){};
    void on_rbutton_down(int x, int y, int) override
    {
        auto right_point = nv::find_matching_point(m_, rm_, cv::Point(x, y));

        auto view = rm_.clone();
        cv::circle(view, right_point, 10, 0, 2);
        cv::namedWindow("Right Point", cv::WINDOW_NORMAL);
        cv::imshow("Right Point", view);
        cv::waitKey(0);
        cv::destroyWindow("Right Point");
    }
protected:
    cv::Mat const &rm_;
    cv::Mat dm_;
    cv::Mat sm_;
};


const std::string resource_path = "/home/bvi/projects/novavision/resources/";

int main(void)
{
    try
    {
        std::vector<int> arr{1,3,2,4,6,5,8,7,10,9,13,11,12,17,14,15,16};
        nv::ui::Plot<int> plot("PLOT", cv::Size(640/2, 480/2));
        plot.draw_grid(5);
        plot.draw(arr, cv::Scalar(255, 0, 0), 1, 5, 5);
        plot.draw_caption("some graph.");
        cv::namedWindow("PLOT_WINDOW", 0);// cv::WINDOW_NORMAL);
        cv::imshow("PLOT_WINDOW", plot.image());
        cv::waitKey(0);
        cv::destroyWindow("PLOT_WINDOW");

        // auto lm = cv::imread(resource_path + "moto/im0.png", 0);
        // if(lm.empty()) throw std::system_error(errno, std::system_category(), "moto/im0.png");
        // auto rm = cv::imread(resource_path + "moto/im1.png", 0);
        // if(rm.empty()) throw std::system_error(errno, std::system_category(), "moto/im1.png");
        // auto dm = rm.clone();
        // auto sm = lm.clone();
        // ToolWidow tool("TOOL", rm, dm, sm);
        // tool.show(lm);

        // cv::waitKey(0);
    }
    catch(exception const& ex){
        cout << "fail: " << ex.what() << endl;
    }

    std::cout << "press enter for exit ...\n";
    getchar();

    return 0;
}