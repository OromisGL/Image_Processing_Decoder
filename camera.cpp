//
// Created by Louis Krämer on 29.09.25.
//

#include "processor.h"

cv::Mat Processor::camera()
{
    double horizontal_fov_16_9 = 85.8;
    double vertical_fov_16_9 = 55.2;

    double horizontal_fov_grad = horizontal_fov_16_9 * M_PI / 180;
    double vertical_fov_grad = vertical_fov_16_9 * M_PI / 180;

    double half_width = this->screen.width * 0.5;
    double half_height = this->screen.height * 0.5;

    double fx = half_width / (std::tan(horizontal_fov_grad * 0.5));
    double fy = half_height / (std::tan(vertical_fov_grad * 0.5));
    double cx = half_width;
    double cy = half_height;

    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) <<
                             fx, 0.0, cx,
                             0.0, fy, cy,
                             0.0, 0.0, 1.0);

    return camera_matrix;
}

cv::Mat Processor::K_calib_camera()
{
    double horizontal_fov_16_9 = 85.8;
    double vertical_fov_16_9 = 55.2;

    double horizontal_fov_grad = horizontal_fov_16_9 * M_PI / 180;
    double vertical_fov_grad = vertical_fov_16_9 * M_PI / 180;

    double half_width = 1920.0 * 0.5;
    double half_height = 1440.0 * 0.5;

    double scaling_width = static_cast<double>(this->screen.width) / 1920.0;
    double scaling_height = static_cast<double>(this->screen.height) / 1440.0;

    double fx = half_width / (std::tan(horizontal_fov_grad * 0.5));
    double fy = half_height / (std::tan(vertical_fov_grad * 0.5));
    double cx = half_width;
    double cy = half_height;

    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) <<
                             fx * scaling_width, 0.0, cx * scaling_width,
                             0.0, fy * scaling_height, cy * scaling_height,
                             0.0, 0.0, 1.0);

    return camera_matrix;
}

cv::Mat Processor::camera_optimal_matrix()
{
    cv::Mat K = K_calib_camera();

    this->distCoeffs.at<double>(0) = 0.04856921316554052; // k1 radial
    this->distCoeffs.at<double>(1) = 0.01880262556299999; // k2 radial
    this->distCoeffs.at<double>(2) = -0.004006892571065506; // p1 tangential
    this->distCoeffs.at<double>(3) = -0.0029466514990889453; // p2 tangential

    const cv::Size size(this->screen.width, this->screen.height);

    const double alpha = 1.0;

    cv::Mat newK = cv::getOptimalNewCameraMatrix(K,
                                                 this->distCoeffs,
                                                 size,
                                                 alpha,
                                                 size,
                                                 0);

    if (newK.empty())
    {
        throw std::runtime_error("newK is empty.");
    }

    return newK;
}

void Processor::create_undistort_camera(cv::Mat const &cameraMatrix,
                                        cv::Mat const &orig_camera_Matrix,
                                        cv::Mat distCoeffs,
                                        cv::Size const &imgsize,
                                        cv::Mat &out_map1, cv::Mat &out_map2)
{
    cv::Mat R = cv::Mat::eye(3, 3,CV_64F);

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
