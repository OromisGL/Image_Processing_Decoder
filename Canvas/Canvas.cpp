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
    int ty = (screen.width * 2) / 5;
    int tx = (screen.height * 2) / 2;
    float scaler = 50;
    float ratio =  screen.width / screen.height;

    double delta_blue = processor_.blue_ball.distances - processor_.camera_position.initial_blue;
    double delta_green = processor_.green_ball.distances - processor_.camera_position.initial_green;


    for (auto &Ball: Ball_set)
    {
        cv::Point center(
            static_cast<int>(Ball.positions.x * scaler + tx),
            static_cast<int>(Ball.positions.z * scaler + ty));

        cv::Point camera = cv::Point(
            static_cast<int>(camera_position.x * scaler + tx) ,
            static_cast<int>(camera_position.z + delta_blue + ty));

        cv::circle(
            canvas,
            center,
            BALL_RADIUS * scaler,
            Ball.color,
            cv::FILLED,
            antialias);

        //euclidian_distance_2D(Ball, camera_position);

        double radius = vektor_length(center, camera);

        draw_line(canvas, camera, center, {25, 25, 25});

        cv::circle(
            canvas,
            center,
             static_cast<int>(radius) ,
            cv::Scalar(25, 25, 25),
            1,
            antialias);

        cv::circle(canvas, camera,
                   15,
                   cv::Scalar(25, 25, 25),
                   cv::FILLED);
    }
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
