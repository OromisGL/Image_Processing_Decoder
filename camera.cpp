//
// Created by Louis Krämer on 29.09.25.
//

#include "processor.h"


void Processor::orig_camera()
{
    cv::FileStorage fs("/Users/glaeder/Developer/Image_Processing_C++_Test/settings/camera.yaml", cv::FileStorage::READ);
    fs["camera"]["K"] >> Ocam.K;
    fs["camera"]["D"] >> Ocam.D;

    int W = (int)fs["camera"]["image_width"], H = (int)fs["camera"]["image_height"];
    Ocam.size = cv::Size(W,H);

    std::string model;
    fs["camera"]["model"] >> model;
}


void Processor::virtual_camera_setting(double fov_deg)
{

    int mapType = CV_16SC2;
    orig_camera();
    // Brennweite
    double fovx = fov_deg * CV_PI / 180.0;
    double fx = screen.width / (2 * std::tan(fovx * 0.5));
    double fy = fx;
    //Bildmitte
    double cx = screen.width * 0.5;
    double cy = screen.height * 0.5;

    Vcam.Knew = (cv::Mat_<double>(3,3) <<
                                                    fx, 0, cx,
                                                    0, fy, cy,
                                                    0, 0, 1);

    cv::initUndistortRectifyMap(
        Ocam.K,
        Ocam.D,
        cv::Mat::eye(3,3, CV_64F),
        Vcam.Knew,
        cv::Size(screen.width, screen.height),
        mapType, Vcam.map1, Vcam.map2);
}

cv::Mat Processor::applyVcam(const cv::Mat& frame)
{
    cv::Mat rectfield;
    cv::remap(frame, rectfield, Vcam.map1, Vcam.map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT);
    return rectfield;
}

