//
// Created by v_0ver on 9/15/20.
//

#ifndef PLAYBOT_SCREENSHOT_H
#define PLAYBOT_SCREENSHOT_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>


class ScreenShot
{
    Display *display;
    Window root;
    int left, top, width, height;
    XImage *img{nullptr};

public:

    ScreenShot(const int left, const int top, const int width, const int height) :
            left(left),
            top(top),
            width(width),
            height(height)
    {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);
    }

    void operator()(cv::Mat &cvImg)
    {
        if (img != nullptr)
        {
            XDestroyImage(img);
        }
        img = XGetImage(display, root, left, top, width, height, AllPlanes, ZPixmap);
        cvImg = cv::Mat(height, width, CV_8UC4, img->data);
    }

    cv::Point GetAbsPosition(const cv::Point point){
        auto p = cv::Point(left, top);
        return point+p;

    }

    void GetAbsCenter(cv::Point *point){
        Screen*  scrn = DefaultScreenOfDisplay(display);
        point->x =  scrn->width/2;
        point->y =  scrn->height/2;

    }



    ~ScreenShot()
    {
        if (img != nullptr)
        {
            XDestroyImage(img);
        }
        XCloseDisplay(display);
    }
};

#endif //PLAYBOT_SCREENSHOT_H
