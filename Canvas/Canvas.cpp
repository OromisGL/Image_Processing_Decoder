//
// Created by Louis Krämer on 01.10.25.
//

#include "Canvas.h"
#include "../setter.h"

void Canvas::displayCanvas()
{
    auto processor = getProcessor();

    cv::Mat canvas = draw2DCanvas(
        processor->screen.width * 2,
        processor->screen.height * 2,
        cv::Scalar(255, 255, 255));

    drawOut(canvas);

    cv::namedWindow("Ball Drawings", cv::WINDOW_AUTOSIZE);

    cv::imshow("Ball Drawings", canvas);
}

cv::Mat Canvas::draw2DCanvas(int width, int height, const cv::Scalar &background)
{
    cv::Mat pictureCanvas(width, height, CV_8UC3, background);
    return pictureCanvas;
}

void Canvas::drawOut(cv::Mat &canvas)
{
    auto processor = getProcessor();

    const int antialias = cv::LINE_AA;
    int tx = (processor->screen.height * 2) / 2;
    int ty = (processor->screen.width * 2) / 5;
    double scaler = 40.;
    // float ratio =  screen.width / screen.height;

    // double delta_blue = processor_.blue_ball.distances - processor_.camera_position.initial_blue;
    // double delta_green = processor_.green_ball.distances - processor_.camera_position.initial_green;

    for (const auto& current_ball : processor->Ball_set)
    {

        cv::Point center(
            static_cast<int>(current_ball->x * scaler + tx),
            static_cast<int>(current_ball->z * scaler + ty));

        cv::circle(
            canvas,
            center,
            processor->BALL_RADIUS * scaler,
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

    // Triangle to the Camera position
    double a = processor->DISTANCE_BETWEEN_BALL;
    double b = processor->Ball_set[0]->distances; // blue Ball
    double c = processor->Ball_set[1]->distances; // green Ball

    // getting the x and y Komponents of the Camera Position
    double x = (b * b - a * a - c * c) / (2 * a); // x direction
    double z = std::sqrt(c * c - x * x); // z direction

    // adding a x offset to get the center of the Triangle
    double x_offset = a / 2;

    cv::Point2d camera(
            (x + x_offset) * scaler + tx,
            z * scaler + ty);

    // Start drawing the lines on the Canvas
    if (processor->framecount > 1 || processor->framecount % 10 == 0)
    {
        processor->drawPoints.push_back(camera);

        for (auto& p_camera : processor->drawPoints)
        {
            for (int y = 0; y < canvas.rows; y++)
            {
                continue;
            }
        }

        for (std::size_t index = 1; index < processor->drawPoints.size(); ++index)
        {
            const cv::Point2d previous_point = processor->drawPoints[index - 1];
            const cv::Point2d current_point = processor->drawPoints[index];

            cv::line(canvas, previous_point, current_point, cv::Scalar(25,25,25), 2, cv::LINE_AA);
        }
    }

    cv::circle(canvas, camera,
                   3,
                   cv::Scalar(25, 25, 25),
                   cv::FILLED);
}