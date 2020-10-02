//
// Created by v_0ver on 10/1/20.
//

#ifndef ALBION_ONLINE_WALKDETECTOR_H
#define ALBION_ONLINE_WALKDETECTOR_H


class WalkDetector
{
    bool IsWalk(cv::Mat now, cv::Mat last)
    {
        auto result = cv::mean(last - now);
        std::cout << result << std::endl;
        return true;
    }
};


#endif //ALBION_ONLINE_WALKDETECTOR_H
