#include <opencv2/opencv.hpp>

#include <forms/forms.hpp>
#include <core/core.hpp>

#include <iostream>

using namespace std;

class ToolWidow: public nv::ui::Form
{
public:
    ToolWidow(std::string const& title, cv::Mat const &rm, cv::Mat &dm, cv::Mat &sm) nv::ui::Form(title), lm_(m), rm_(rm), dm_(dm), sm_(sm){};
    void on_rbutton_down(int x, int y)
    {
        auto point = nv::find_matching_point(lm_, rm_, cvPoint(x, y));

        mak
    }
protected:
    cv::Mat &lm_;
    cv::Mat const &rm_;
    cv::Mat dm_;
    cv::Mat sm_;
};


const std::string resource_path = "/home/bvi/projects/novavision/resources/";

int main(void)
{
    try
    {
        auto lm = cv::imread(resource_path + "moto/im0.png", 0);
        if(lm.empty()) throw std::system_error(errno, std::system_category(), "moto/im0.png");
        auto rm = cv::imread(resource_path + "moto/im1.png", 0);
        if(rm.empty()) throw std::system_error(errno, std::system_category(), "moto/im1.png");

        imshow("LEFT: ", lm);
        imshow("RIGHT: ", rm);

        cv::waitKey(0);

        cv::destroyAllWindows();

        auto depth_map = nv::build_depth_map_multy_line(lm, rm);

        cv::namedWindow("DEPTH", cv::WINDOW_NORMAL);
        cv::imshow("DEPTH", depth_map);

        cv::waitKey(0);        
    }
    catch(exception const& ex){
        cout << "fail: " << ex.what() << endl;
    }

    getchar();

    return 0;
}