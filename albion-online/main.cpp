//
// Created by v_0ver on 10/1/20.
//
#include <iostream>

#include <opencv2/opencv.hpp>

#include <chrono>
#include <opencv2/imgproc.hpp>


#include "../common/ScreenShot.h"
#include "../common/StaticDetector.h"

#include "../common/WalkDetector.h"




int main(int argc, char** argv)
{
    ScreenShot screen;
    screen.CaptureWindow(std::string("Albion Online Client"));

    cv::Mat img, show, last, result, gray;

    screen(img);

    for(;;)
    {
        screen(img);
        std::cout << screen.GetCenter() << "\t" << screen.GetAbsCenter() <<std::endl;
        //img.copyTo(show);
        cvtColor(img, gray, cv::COLOR_RGB2GRAY);
        cv::resize(gray, gray, cv::Size(), 0.5f, 0.5f  );
        result = gray - last;


        cv::Canny(result, result, 100, 200);
        auto walk = cv::mean(result)[0];
        if (walk>=5)
        {
            std::cout << "Walk\t" << walk << std::endl;
        }
        last = gray;

        //cvtColor(img, img, cv::COLOR_RGB2BGR);
        //cvtColor(show, show, cv::COLOR_BGR2RGB);
        imshow("Capture", result);
        char k = cv::waitKey(100);
        if (k == 'q') {
            cv::destroyAllWindows();
            break;
        }
    }
        // Detect cursor

        return 0;
}
