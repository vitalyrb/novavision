#include <opencv2/opencv.hpp>

#include <core/core.hpp>

#include <iostream>

using namespace std;

const std::string resource_path = "/home/bvi/projects/novavision/resources/";

int main(void)
{
    try
    {
        auto lm = cv::imread(resource_path + "moto/im0_mini.png", 0);
        if(lm.empty()) throw std::system_error(errno, std::system_category(), "moto/im0_mini.png");
        auto rm = cv::imread(resource_path + "moto/im1_mini.png", 0);
        if(rm.empty()) throw std::system_error(errno, std::system_category(), "moto/im1_mini.png");

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