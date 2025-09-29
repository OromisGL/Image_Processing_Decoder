//
// Created by Louis Krämer on 25.09.25.
//

#pragma once
#include <string>
#include <iostream>
#include <opencv2/videoio.hpp>
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>
#include <iostream>
#include <opencv2/videoio.hpp>
#include <filesystem>
#include <iomanip>
#include "opencv2/calib3d/calib3d.hpp"
#include <unordered_set>

struct color_count
{
    int red = 0, green = 0, blue = 0;
};

struct Ball
{
    std::string name;
    int64_t x = 0, y = 0;
    std::vector<std::pair<int, int>> positions;
    std::pair<double, double> middle;
    double radius;
};

struct Screen
{
    std::vector<cv::Point2d> drawPoints;
};

struct screen_M
{
    int x = 1280 / 2;
    int y = 720 / 2;
};

struct INITIAL_CAMERA_POSITION
{
    float BALL_RADIUS = 3;
    float DISTANCE_BETWEEN_BALL = 9;
    float PEN_HEIGHT = 270 + 180;
    float INITIAL_Z_OFFSET = 18;
    int x = (1280 / 2);
    float y = PEN_HEIGHT;
    float z = INITIAL_Z_OFFSET;
};

class Processor {
private:
    Screen screen;
    color_count count;
    Ball red_ball, green_ball, blue_ball;
    static constexpr int threshold = 80;
    void m_display_info(cv::Mat& frame, cv::Mat& out);
    void m_frame_processing(cv::Mat& input, cv::Mat& output);
    void add_x_y(Ball& b);
    void middle_point(Ball& b);
    void draw_line_to_camera(Ball& b, cv::Mat& img);
    void draw_line(cv::Mat& img, cv::Point2f p1, cv::Point2f p2);
    double vektor_length(cv::Point2f p1, cv::Point2f p2);
    cv::Point2d middle_two_vek(cv::Point2d p1, cv::Point2d p2);
    cv::Point2d target_vek(Ball& b, cv::Point2f p1, cv::Point2f p2);
    void crossing_point(cv::Mat& out, std::vector<cv::Point2d>& output);
    void draw_to_screen(cv::Mat& out, std::vector<cv::Point2d>& drawPoints);
    // Test Function
    //cv::Mat image_color_field(const std::string& img_path, const std::string& img_name);
public:
    Processor() = default;
    cv::VideoCapture in_video(const std::string& video_path);
    void video_loop(cv::VideoCapture& img);

};
