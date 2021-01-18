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
        cv::setMouseCallback(title_, callback_func, this);
        if(size.width != -1)
        {
            resize(size);
            m_ = cv::Mat(size, CV_8UC1);
            m_ = 0;
        }
    }
    Form& show(cv::Mat m)
    {
        m_ = m;
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
    virtual void on_lbutton_down(int, int, int){}
    virtual void on_rbutton_down(int, int, int){}
    virtual void on_mbutton_down(int, int, int){}
    virtual void on_mouse_move(int, int, int){}
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
            case cv::EVENT_LBUTTONDOWN: on_lbutton_down(x, y, flags); break;
            case cv::EVENT_RBUTTONDOWN: on_rbutton_down(x, y, flags); break;
            case cv::EVENT_MBUTTONDOWN: on_mbutton_down(x, y, flags); break;
            case cv::EVENT_MOUSEMOVE:   on_mouse_move(x, y, flags);   break;
        };
    }
protected:
    std::string title_;
    cv::Mat m_;
};

template<typename T>
class Plot
{
public:
    Plot(std::string const& title, cv::Size const& size, cv::Scalar const& background = cv::Scalar().all(0))
        : size_(size), m_(size, CV_8UC3, background), title_(title) 
    {
    }
    void draw_caption(std::string const& caption)
    {
        int fontFace = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
        double fontScale = 1;
        int thickness = 1;
        int baseline=0;
        cv::Size textSize = cv::getTextSize(caption, fontFace,
                                    fontScale, thickness, &baseline);
        baseline += thickness;        

        cv::Point textOrg((m_.cols - textSize.width)/2, textSize.height + 2);// (m_.rows + textSize.height)/2);
        cv::putText(m_, caption, textOrg, fontFace, fontScale, cv::Scalar::all(128), thickness, 8);        

    }
    void draw_grid(int grid_size, cv::Scalar const& grid_color = cv::Scalar().all(24))
    {
        for(int y = m_.rows; y > 0; y -= grid_size)
        {
            cv::line(m_, {0, y}, {m_.cols, y}, grid_color, 1, cv::LINE_AA);
        }
        for(int x = 0; x < m_.cols; x += grid_size)
        {
            cv::line(m_, {x, 0}, {x, m_.rows}, grid_color, 1, cv::LINE_AA);
        }        
    }
    void draw(std::vector<T> const& values, cv::Scalar const& color, int thickness = 1, float x_scale = 1., float y_scale = 1.)
    {
        cv::Point prev_point(0, m_.rows - values[0]*y_scale);
        for(size_t i = 1; i < values.size(); ++i)
        {
            cv::Point curr_point(x_scale*i, m_.rows - values[i]*y_scale);
            cv::line(m_, prev_point, curr_point, color, thickness);
            prev_point = curr_point;
        }
    }
    cv::Mat const& image() const
    {
        return m_;
    }
protected:
    cv::Size size_;
    cv::Mat m_;
    std::string title_;
};

} //namespace nv::ui
