//
// Created by Louis Krämer on 01.10.25.
//

#include "Canvas.h"
#include "../setter.h"

cv::Mat Canvas::draw2DCanvas(int width, int height, const cv::Scalar &background)
{
    cv::Mat pictureCanvas(width, height, CV_8UC3, background);
    return pictureCanvas;
}

void Canvas::drawOut(cv::Mat &canvas)
{
    Processor& processor_ = getProcessor();

    const int antialias = cv::LINE_AA;
    int tx = (screen.height * 2) / 2;
    int ty = (screen.width * 2) / 5;
    float scaler = 40;
    // float ratio =  screen.width / screen.height;

    // double delta_blue = processor_.blue_ball.distances - processor_.camera_position.initial_blue;
    // double delta_green = processor_.green_ball.distances - processor_.camera_position.initial_green;


    for (const auto& current_ball : processor_.Ball_set)
    {

        //if (current_ball->id == 0)
        //    continue;

        // double distance_hypot_blue_camera = std::hypot(current_ball->x - camera_position.x, current_ball->z - processor_.camera_position.z);

        cv::Point center(
            static_cast<int>(current_ball->x * scaler + tx),
            static_cast<int>(current_ball->z * scaler + ty));

        cv::circle(
            canvas,
            center,
            BALL_RADIUS * scaler,
            current_ball->color,
            cv::FILLED,
            antialias);

        cv::circle(
            canvas,
            center,
             static_cast<int>(current_ball->distances * scaler) ,
            cv::Scalar(25, 25, 25),
            1,
            antialias);
    }

    double a = DISTANCE_BETWEEN_BALL;
    double b = processor_.Ball_set[0]->distances; // blue Ball
    double c = processor_.Ball_set[1]->distances; // green Ball

    double x = (b * b - a * a - c * c) / (2 * a); // x direction
    double z = std::sqrt(c * c - x * x); // z direction

    double x_offset = a / 2; // Offset

    cv::Point camera(
            static_cast<int>((x + x_offset) * scaler + tx),
            static_cast<int>(z * scaler + ty));

    if (processor_.framecount > 1)
    {
        processor_.drawPoints.push_back(camera);
        std::vector<cv::Point> pts;
        pts.reserve(processor_.drawPoints.size());
        for (const auto& p : processor_.drawPoints)
            pts.emplace_back(cvRound(p.x), cvRound(p.y));

        cv::polylines(
            canvas,
            std::vector<std::vector<cv::Point>>{pts},
            false,
            cv::Scalar(25,25,25),
            2,
            cv::LINE_AA
        );
    }

    cv::circle(canvas, camera,
                   3,
                   cv::Scalar(25, 25, 25),
                   cv::FILLED);
}


void Canvas::displayCanvas()
{
    cv::Mat canvas = draw2DCanvas(
        screen.width * 2,
        screen.height * 2,
        cv::Scalar(255, 255, 255));

    drawOut(canvas);

    cv::namedWindow("Ball Drawings", cv::WINDOW_AUTOSIZE);

    cv::imshow("Ball Drawings", canvas);
}
