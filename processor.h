//
// Created by Louis Krämer on 25.09.25.
//

#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <filesystem>
#include <unordered_set>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/calib3d.hpp"
#include <opencv2/viz/viz3d.hpp>
#include "setter.h"


struct color_count
{
    int red = 0, green = 0, blue = 0;
};

struct dimension
{
    double x, y, z;
};

struct Ball
{
    int id;
    std::string name;
    double x = 0;
    double y = 0;
    double z = 0;
    dimension positions;
    double distances;
    std::pair<double, double> middle;
    cv::Scalar color;
    int field;
    double radius;
};

struct Screen
{
    std::vector<cv::Point2d> drawPoints;
    int width = 1280;
    int height = 720;
};

struct screen_M
{
    int x = 1280 / 2;
    int y = 720 / 2;
};

struct INITIAL_CAMERA_POSITION
{
    float PEN_HEIGHT = 18;
    float INITIAL_Z_OFFSET = 18;
    double initial_green_radii = 0.0;
    double initial_blue_radii = 0.0;
    double initial_blue_distance = 0.0;
    double initial_green_distance = 0.0;
    int x = 0;
    float y = PEN_HEIGHT;
    float z = INITIAL_Z_OFFSET;
};

struct Intrinsics
{
    cv::Mat K; // 3x3, CV_64F
    cv::Mat D; // 1x5 (pinhole) oder 1x4 (fisheye), CV_64F
    cv::Size size; // Bildgröße der Originalaufnahme
    bool fisheye = false;
};

struct VirtualCam
{
    cv::Mat Knew; // Ziel-Intrinsics (3x3, CV_64F)
    cv::Mat map1, map2; // Remap-Tabellen
    cv::Size outSize; // Zielgröße
};


class Processor
{
public:
    std::vector<cv::Point> drawPoints;
    INITIAL_CAMERA_POSITION camera_position;
    VirtualCam Vcam;
    Intrinsics Ocam;
    cv::Mat K_orig_, D_;
    Screen screen;
    color_count count;
    Ball blue_ball, green_ball, red_ball;
    float BALL_RADIUS = 3;
    float DISTANCE_BETWEEN_BALL = 9;
    static constexpr int threshold = 80;
    std::array<Ball*, 3> Ball_set;
    int framecount = 0;
    cv::Mat distCoeffs = cv::Mat::zeros(1,4, CV_64F);

    // Drwaing
    void initializeBalls();

    void m_display_info(cv::Mat &frame, cv::Mat &out);

    void draw_line_to_camera(Ball &b, cv::Mat &img);

    void draw_line(cv::Mat &img, cv::Point2f p1, cv::Point2f p2, cv::Scalar color);

    void draw_to_screen(cv::Mat &out, std::vector<cv::Point2d> &drawPoints);

    // Frame Processor Function
    void m_frame_processing(cv::Mat &input, cv::Mat &output);

    // Math Functions
    void middle_point(Ball &b);

    void crossing_point(cv::Mat &out, std::vector<cv::Point2d> &output);

    double get_delta(Ball &b);

    //Vector Functions
    double euclidian_distance_3D(Ball &p1, INITIAL_CAMERA_POSITION &p2);

    double euclidian_distance_2D(Ball &p1, INITIAL_CAMERA_POSITION &p2);

    double vektor_length(cv::Point2f p1, cv::Point2f p2);

    cv::Point2d middle_two_vek(cv::Point2d p1, cv::Point2d p2);

    cv::Point2d target_vek(Ball& b, cv::Point2f p1, cv::Point2f p2);

    // Test Function
    // cv::Mat image_color_field(const std::string& img_path, const std::string& img_name);
    // Video Input
    cv::VideoCapture in_video(const std::string &video_path);

    void video_loop(cv::VideoCapture &img);

    //Camera Settings and Virtual Camera
    cv::Mat K_calib_camera();

    void create_undistort_camera(cv::Mat const& cameraMatrix, cv::Mat const& orig_camera_Matrix, cv::Mat distCoeffs, cv::Size const& imgsize,
                                        cv::Mat& out_map1, cv::Mat& out_map2);

    cv::Mat camera_optimal_matrix();

    cv::Mat camera();

    void virtual_camera_setting(double fov_deg);

    void orig_camera();

    cv::Mat applyVcam(const cv::Mat &frame);
};
