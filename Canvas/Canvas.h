//
// Created by Louis Krämer on 01.10.25.
//

#pragma once
#include "../processor.h"
#include <algorithm>


class Canvas : Processor {
public:
    cv::Mat draw2DCanvas(int width, int height, const cv::Scalar& background);
    void drawOut(cv::Mat& canvas);
    void displayCanvas();
};
