//
// Created by Louis Krämer on 29.09.25.
//

#include "processor.h"
#include "setter.h"

cv::Mat Processor::camera()
{
    auto processor = getProcessor();

    double vertical_fov = 55.2;
    double horizontal_fov = 85.8;

    double horizontal_fov_grad = horizontal_fov * M_PI / 180;
    double vertical_fov_grad = vertical_fov * M_PI / 180;

    double half_width = processor->screen.width * 0.5;
    double half_height = processor->screen.height * 0.5;

    double fx = (half_width) / (std::tan(horizontal_fov_grad * 0.5));
    double fy = (half_height) / (std::tan(vertical_fov_grad * 0.5));
    double cx = half_width ;
    double cy = half_height;

    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) <<
                             fx, 0.0, cx,
                             0.0, fy, cy,
                             0.0, 0.0, 1.0);

    return camera_matrix;
    //return cv::getDefaultNewCameraMatrix(camera_matrix, cv::Size(screen.width, screen.height), true);
}

cv::Mat Processor::K_calib_camera()
{

    auto processor = getProcessor();

    double vertical_fov = 55.2;
    double horizontal_fov = 85.8;

    double horizontal_fov_grad = horizontal_fov * M_PI / 180;
    double vertical_fov_grad = vertical_fov * M_PI / 180;

    double half_width = 1920.0 * 0.5;
    double half_height = 1440.0 * 0.5;

    double scaling_width = static_cast<double>(processor->screen.width) / 1920.0;
    double scaling_height = static_cast<double>(processor->screen.height) / 1440.0;

    double fx = (half_width) / (std::tan(horizontal_fov_grad * 0.5));
    double fy = (half_height) / (std::tan(vertical_fov_grad * 0.5));
    double cx = half_width ;
    double cy = half_height;

    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) <<
                             fx * scaling_width, 0.0, cx * scaling_width,
                             0.0, fy * scaling_height, cy * scaling_height,
                             0.0, 0.0, 1.0);

    return camera_matrix;
}

cv::Mat Processor::camera_optimal_matrix()
{
    auto processor = getProcessor();

    cv::Mat K = K_calib_camera();

    processor->distCoeffs.at<double>(0) = 0.04856921316554052;  // k1 radial
    processor->distCoeffs.at<double>(1) = 0.01880262556299999; // k2 radial
    processor->distCoeffs.at<double>(2) = -0.004006892571065506;  // p1 tangential
    processor->distCoeffs.at<double>(3) = -0.0029466514990889453;  // p2 tangential


    const cv::Size size(processor->screen.width, processor->screen.height);

    const double alpha = 1.0;

    cv::Mat newK = cv::getOptimalNewCameraMatrix(K, processor->distCoeffs, size, alpha, size, 0);

    if (newK.empty())
    {
        throw std::runtime_error("newK is empty.");
    }

    return newK;
}

void Processor::create_undistort_camera(cv::Mat const& cameraMatrix, cv::Mat const& orig_camera_Matrix, cv::Mat distCoeffs, cv::Size const& imgsize,
                                        cv::Mat& out_map1, cv::Mat& out_map2)
{
    cv::Mat R = cv::Mat::eye(3,3,CV_64F);

    cv::initUndistortRectifyMap(
        orig_camera_Matrix,
        distCoeffs,
        R,
        cameraMatrix,
        imgsize,
        CV_32FC1,
        out_map1,
        out_map2);
}

void Processor::orig_camera()
{
    cv::FileStorage fs("/Users/glaeder/Developer/Image_Processing_C++_Test/settings/camera.yaml",
                       cv::FileStorage::READ);
    fs["camera"]["K"] >> Ocam.K;
    fs["camera"]["D"] >> Ocam.D;

    int W = (int) fs["camera"]["image_width"], H = (int) fs["camera"]["image_height"];
    Ocam.size = cv::Size(W, H);

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

    Vcam.Knew = (cv::Mat_<double>(3, 3) <<
                 fx, 0, cx,
                 0, fy, cy,
                 0, 0, 1);

    cv::initUndistortRectifyMap(
        Ocam.K,
        Ocam.D,
        cv::Mat::eye(3, 3, CV_64F),
        Vcam.Knew,
        cv::Size(screen.width, screen.height),
        mapType, Vcam.map1, Vcam.map2);
}

cv::Mat Processor::applyVcam(const cv::Mat &frame)
{
    cv::Mat rectfield;
    cv::remap(frame, rectfield, Vcam.map1, Vcam.map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT);
    return rectfield;
}
