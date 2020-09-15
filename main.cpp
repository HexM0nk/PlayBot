#include <iostream>

#include <opencv2/opencv.hpp>

#include <chrono>
#include <opencv2/imgproc.hpp>


#include "ScreenShot.h"
#include "StaticDetector.h"

using namespace cv;

int main(int argc, char** argv)
{

    StaticDetector station("../StarConflict/img/station.png");
    StaticDetector cursor("../StarConflict/img/cursor.png");
    StaticDetector aim("../StarConflict/img/aim.png");

    ScreenShot screen(0,0,1366,768);
    Mat img, show, result;

        for(;;){
            auto start = std::chrono::high_resolution_clock::now();
            screen(img);
            cvtColor(img, img, cv::COLOR_RGB2BGR);
            img.copyTo(show);

            // Detect cursor
            matchTemplate( img, cursor.templ, result, TM_CCORR_NORMED, cursor.mask3c );

            Point cursor_point;
            double cursor_val;
            minMaxLoc(result, nullptr, &cursor_val, nullptr, &cursor_point);
            if (cursor_val>0.95f){
                auto color_blue = Scalar(0,0,255);
                rectangle(show, cursor_point,cursor_point+cursor.Size(),color_blue,2);
                putText(show, std::to_string(cursor_val), cursor_point, FONT_HERSHEY_SIMPLEX, 1, color_blue, 2, LINE_AA);
            }

            // Detect static objects
            matchTemplate( img, station.templ, result, TM_CCORR_NORMED, station.mask3c );

            // Detect aim
            matchTemplate( img, aim.templ, result, TM_CCORR_NORMED, aim.mask3c );

            resize(show, show, cv::Size(), 0.5f, 0.5f );
            imshow("Capture", show);
            char k = waitKey(1);
            if (k == 'q') {
                destroyAllWindows();
                break;
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
            std::cout << "Time taken: " << duration.count() << std::endl;
        }

        return 0;
}
