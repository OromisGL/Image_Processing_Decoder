//
// Created by Louis Krämer on 01.10.25.
//

#include "Canvas.h"


cv::Mat Canvas::draw2DCanvas(int width, int height, const cv::Scalar& background)
{
    cv::Mat pictureCanvas(width, height, CV_8UC3, background);
    return pictureCanvas;
}

void Canvas::drawOut(cv::Mat& canvas)
{
    const int antialias = cv::LINE_AA;
    float ty = screen.width / 2;
    float tx = screen.height / 2;
    float scaler = 15;

    std::vector<Ball_Properties> Ball_set = initializeBalls();

    for (const auto& Ball : Ball_set)
    {
        cv::Point center( Ball.x * scaler + tx, Ball.z * scaler + ty);
        cv::circle(
            canvas,
            center,
            Ball.radius * scaler,
            Ball.color,
            cv::FILLED,
            antialias);
    }

    cv::circle(canvas, cv::Point(
        camera_position.x * scaler + tx,
        camera_position.z * scaler + ty),
        15,
        cv::Scalar(25,25,25),
        cv::FILLED);
}

void Canvas::displayCanvas()
{
    while (true)
    {
        cv::Mat canvas  = draw2DCanvas(
            screen.width,
            screen.height,
            cv::Scalar(255,255,255));

        drawOut(canvas);

        cv::namedWindow("Ball Drawings", cv::WINDOW_AUTOSIZE);

        cv::imshow("Ball Drawings", canvas);

        int key = cv::waitKey(1);
        if (key == 27 || key == 'q')
        {
            cv::destroyWindow("video input");
            cv::destroyWindow("video output");
            break;
        }
    }
}