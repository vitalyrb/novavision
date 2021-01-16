#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>

namespace nv::ui
{
class Form
{
public:
    explicit Form(std::string const& title): title_(title)
    {
        create();
    }
    Form(std::string const& title, cv::Size const& size): title_(title)
    {
        create(size);
    }    
    virtual ~Form()
    {
        destroy();
    }
    void create(cv::Size const& size = cv::Size{-1, -1})
    {
        cv::namedWindow(title_, cv::WINDOW_NORMAL);
        setMouseCallback(title_, callback_func, this);
        if(size.width != -1)
        {
            resize(size);
            m_ = cv::Mat(size, cv::CV_U8C1);
            m_ = 0;
        }
    }
    Form& show(cv::Mat m)
    {
        cv::imshow(title_, m);

        return *this;
    }
    Form& move(cv::Point const &p)
    {
        cv::moveWindow(title_, p.x, p.y);

        return *this;
    }
    Form& resize(cv::Size const &s)
    {
        cv::resizeWindow(title_, s.width, s.height);

        return *this;
    }
    void destroy()
    {
        cv::destroyWindow(title_);
    }
    virtual void on_lbutton_down(int x, int y){}
    virtual void on_rbutton_down(int x, int y){}
    virtual void on_mbutton_down(int x, int y){}
    virtual void on_lbutton_down(int x, int y){}
protected:
    static void callback_func(int event, int x, int y, int flags, void* userdata)
    {
        Form *form = static_cast<Form*>(userdata);
        form->callback(event, x, y, flags);
    }
    void callback(int event, int x, int y, int flags)
    {
        switch(event)
        {
            case cv::EVENT_LBUTTONDOWN: on_lbutton_down(x, y);
            case cv::EVENT_RBUTTONDOWN: on_rbutton_down(x, y);
            case cv::EVENT_MBUTTONDOWN: on_mbutton_down(x, y);
            case cv::EVENT_MOUSEMOVE:   on_lbutton_down(x, y);
        };
    }
protected:
    std::string title_;
    cv::Mat m_;
};

}//namespace nv:;ui