//
// Created by Louis Krämer on 29.09.25.
//

#include "processor.h"

void Processor::orig_camera()
{
    cv::FileStorage fs("..settings/camera.yaml", cv::FileStorage::READ);
    fs["K"] >> K_orig_;
    fs["D"] >> D_;

    int inW = (int)fs["image_width"], inH = (int)fs["image_height"];
    std::string model;
    fs["model"] >> model;
}

void Processor::virtual_camera(cv::Mat& frame)
{

    // Brennweite
    double fov = 90 * CV_PI / 180.0;
    double fx = screen.width / (2 * std::tan(fov * 0.5));
    double fy = fx;
    //Bildmitte
    double cx = screen.width * 0.5;
    double cy = screen.height * 0.5;
    int screen_size = screen.width * screen.height;

    cv::Mat K = (cv::Mat_<double>(3,3) <<
                                                    fx, 0, cx,
                                                    0, fy, cy,
                                                    0, 0, 1);

    cv::Mat map1, map2;

    cv::initUndistortRectifyMap(K_orig_, D_, cv::Mat::eye(3,3, CV_64F), K, cv::Size(screen.width, screen.height), CV_16SC2, map1, map2);
    cv::Mat reprojected;
    cv::remap(frame, reprojected, map1, map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT);

}