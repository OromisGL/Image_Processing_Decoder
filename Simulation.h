//
// Created by Louis Krämer on 11.10.25.
//

#pragma once

#include "Canvas/Canvas.h"
#include <opencv2/viz/viz3d.hpp>
#include <string>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include "setter.h"
#include <opencv2/viz/vizcore.hpp>
#include <opencv2/viz/widgets.hpp>


class Simulation
{
public:

    std::vector<std::unique_ptr<cv::Mat>> snap;

    std::vector<cv::Vec3d> generate_path(cv::Vec3d& start, double radius, double point_count);
    void Simulation_loop();
};
