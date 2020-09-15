//
// Created by v_0ver on 9/15/20.
//

#ifndef PLAYBOT_STATICDETECTOR_H
#define PLAYBOT_STATICDETECTOR_H

#include <opencv2/core/softfloat.hpp>
#include <opencv2/core/cvstd.hpp>

const static cv::String window_name = "img";

const int max_value_H = 360/2;
const int max_value = 255;

int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;
int R = 0, G = 0, B=0;

static void on_low_H_thresh_trackbar(int, void *)
{
    low_H = cv::min(high_H-1, low_H);
    cv::setTrackbarPos("Low H", window_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = cv::max(high_H, low_H+1);
    cv::setTrackbarPos("High H", window_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = cv::min(high_S-1, low_S);
    cv::setTrackbarPos("Low S", window_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = cv::max(high_S, low_S+1);
    cv::setTrackbarPos("High S", window_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = cv::min(high_V-1, low_V);
    cv::setTrackbarPos("Low V", window_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = cv::max(high_V, low_V+1);
    cv::setTrackbarPos("High V", window_name, high_V);
}

class StaticDetector
{
public:
    cv::Mat templ, mask1c, mask3c;

    void InitMask(){
        auto scalar = cv::Scalar(0, 0, 0);
        cv::inRange(templ, scalar, scalar,mask1c);
        cv::bitwise_not(mask1c, mask1c);
        cv::Mat in[] = {mask1c, mask1c, mask1c};
        cv::merge(in, 3, mask3c);
    }

    StaticDetector(const char* path){
        templ = cv::imread(path, cv::IMREAD_COLOR );
        InitMask();
    }
    cv::Point Size(){
        return cv::Point(templ.cols, templ.rows);
    }
    int GetHeight(){
        return templ.rows;
    }
    int GetWQidth(){
        return templ.cols;
    }
    void Show(){
        namedWindow( window_name, cv::WINDOW_AUTOSIZE);
        cv::createTrackbar("Low H", window_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
        cv::createTrackbar("High H", window_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
        cv::createTrackbar("Low S", window_name, &low_S, max_value, on_low_S_thresh_trackbar);
        cv::createTrackbar("High S", window_name, &high_S, max_value, on_high_S_thresh_trackbar);
        cv::createTrackbar("Low V", window_name, &low_V, max_value, on_low_V_thresh_trackbar);
        cv::createTrackbar("High V", window_name, &high_V, max_value, on_high_V_thresh_trackbar);

        cv::createTrackbar("Red", window_name, &R, max_value, nullptr);
        cv::createTrackbar("Green", window_name, &G, max_value, nullptr);
        cv::createTrackbar("Blue", window_name, &B, max_value, nullptr);

        cv::Mat templ_HSV, templ_threshold;

        while (true) {
            // Convert from BGR to HSV colorspace
            cv::cvtColor(templ, templ_HSV, cv::COLOR_BGR2HSV);
            // Detect the object based on HSV Range Values
            cv::inRange(templ_HSV, cv::Scalar(low_H, low_S, low_V), cv::Scalar(high_H, high_S, high_V), templ_threshold);

            // Show the frames
            cv::imshow(window_name, templ_threshold);
            auto key = cv::waitKey(30);
            if (key == 'q' || key == 27)
            {
                break;
            }
        }
    }
    void ShowMask(){
        cv::imshow("mask", mask1c);
        cv::waitKey(0);
    }

    void MatchOne(cv::Mat *img, int method){

    }


    void MatchMany(cv::Mat *img, int method){

    }


    ~StaticDetector(){

    }
};


#endif //PLAYBOT_STATICDETECTOR_H
