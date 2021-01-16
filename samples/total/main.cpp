#include <opencv2/opencv.hpp>

#include <iostream>
#include <functional>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void walk(cv::Mat& m, cv::Rect const& r, std::function<void (cv::Mat &, cv::Point const&)> f)
{
    for(int y = r.y; y < r.br().y; ++y)
    {
        for(int x = r.x; x < r.br().x; ++x)
        {
            f(m, cv::Point(x, y));
        }
    }
}

const std::string resource_path = "/home/bvi/projects/novavision/resources/";

void cut_and_save(cv::Rect const& r, std::string const& path)
{
    for (const auto &it: fs::directory_iterator(path))
    {
        if(!fs::is_directory(it))
        {
            std::string new_filename = path + it.path().stem().string() + "_nano.png";
            auto m = cv::imread(it.path());
            cv::imwrite(new_filename, m(r));
        }
    }
}

int main(void)
{
    try
    {
        std::string filename = resource_path + "moto/im0.png";
        auto m = cv::imread(filename, 0);
        if(m.empty()) throw std::system_error(errno, std::system_category(), filename);

        // cv::Point ltp{1960, 1000};
        // cv::Point brp{2900, 1910};

        cv::Point ltp{1960 + 500, 1000 + 500};
        cv::Point brp{2900, 1910};        // cv::Point ltp{1960, 1000};
        // cv::Point brp{2900, 1910};



        cv::Rect r{ltp.x, ltp.y, brp.x - ltp.x, brp.y - ltp.y};

        walk(m, r, [](cv::Mat& m, cv::Point const& curr_point)
        {
            m.at<uint8_t>(curr_point) /= 4;
        });

        std::string title = "WALK";

        cv::namedWindow(title, cv::WINDOW_NORMAL);
        cv::imshow(title, m);

        cv::waitKey(0);

        std::cout << "select action [1-2]: \n1: exit.\n2: cut and save images ...\n\n";
        auto v = getchar();
        if(v == '1') std::cout << "exit ...\n";
        else if(v == '2') {cut_and_save(r, resource_path + "moto/");}
        else std::cout << "fail choese ... exit ...\n";
    }
    catch(exception const& ex){
        cout << "fail: " << ex.what() << endl;
    }

    getchar();

    return 0;
}