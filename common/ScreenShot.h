//
// Created by v_0ver on 9/15/20.
//

#ifndef PLAYBOT_SCREENSHOT_H
#define PLAYBOT_SCREENSHOT_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <opencv2/opencv.hpp>
#define ACTIVE_WINDOWS "_NET_CLIENT_LIST"


class ScreenShot
{
    Display *display;
    Window window;
    XImage *img{nullptr};
    std::chrono::steady_clock::time_point lastTime;
    std::chrono::milliseconds frameInterval, Interval;

    cv::Rect rect;
    cv::Point center;
    cv::Point position;
public:

    // Get frame capture time im milliseconds
    std::chrono::milliseconds GetFrameInterval()
    {
        return frameInterval;
    }

    // Get time from last frame update in milliseconds
    std::chrono::milliseconds GetInterval()
    {
        return Interval;
    }

    ScreenShot()
    {
        display = XOpenDisplay(nullptr);
        window = DefaultRootWindow(display);
        RefreshGeometry();
    }

    // Update rect, center, position, etc. coord.
    void RefreshGeometry()
    {
        // Get window geometry
        XWindowAttributes xwa;
        XGetWindowAttributes(display, window, &xwa);
        rect = {0,0,xwa.width,xwa.height};
        center = {rect.width/2 ,rect.height/2};

        // Find position of window
        Window child;
        auto root = DefaultRootWindow(display);
        XTranslateCoordinates( display, window, root, xwa.x, xwa.y, &position.x, &position.y, &child );

    }

    // Capture window by name.
    // U can get all available window names by command: "wininfo -root -tree"
    bool CaptureWindow(std::string name)
    {
        auto IsFind = false;
        auto root = DefaultRootWindow(display);
        Atom atom = XInternAtom(display, ACTIVE_WINDOWS, true);
        Atom actualType;
        int format;
        unsigned long numItems;
        unsigned long bytesAfter;

        unsigned char *data = nullptr;
        Window *list;

        int status = XGetWindowProperty(display, root, atom, 0L, (~0L), false,
                                        AnyPropertyType, &actualType, &format, &numItems, &bytesAfter, &data);
        list = (Window *)data;
        XTextProperty text;

        if (status >= Success && numItems) {
            for (int i = 0; i < numItems; ++i) {
                status = XGetWMName(display, list[i], &text);
                auto windowName =  text.value;
                if (status >= Success) {
                    std::string windowNameStr((char *)windowName);
                    if (windowNameStr.find(name) == 0)
                    {
                        window = list[i];
                        std::cout << windowName << "\tFound!" << std::endl;
                        IsFind=true;
                        break;
                    }

                    std::cout << windowName << std::endl;
                }
            }
        }

        XFree(data);
        RefreshGeometry();
        return IsFind;
    }

    // Set new rect for capture in local coord
    void SetNewRect(cv::Rect newrect)
    {
        rect=newrect;
        RefreshGeometry();
    }

    // Update screenshot
    void operator()(cv::Mat &cvImg)
    {
        auto startTime = std::chrono::steady_clock::now();

        if (img != nullptr)
        {
            XDestroyImage(img);
        }
        img = XGetImage(display, window, rect.x, rect.y, rect.width, rect.height, AllPlanes, ZPixmap);
        cvImg = cv::Mat(rect.height, rect.width, CV_8UC4, img->data);

        auto endTime = std::chrono::steady_clock::now();
        frameInterval = duration_cast<std::chrono::milliseconds>(endTime - startTime);
        Interval = duration_cast<std::chrono::milliseconds>(endTime - lastTime);
        lastTime = endTime;
    }

    // Get window center
    cv::Point GetCenter(){
        return center;

    }

    // Get window center in absolute coord.
    cv::Point GetAbsCenter(){
        return center+position;

    }

    cv::Rect GetRect(){
        return rect;
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
