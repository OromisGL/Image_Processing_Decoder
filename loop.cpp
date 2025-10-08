//
// Created by Louis Krämer on 26.09.25.
//

#include "processor.h"
#include "Canvas/Canvas.h"
#include "setter.h"

void Processor::video_loop(cv::VideoCapture &img)
{
    cv::namedWindow("video input", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("video output", cv::WINDOW_AUTOSIZE);

    int delay = 1;
    auto canvas = getCanvas();

    // Getting the CameraMatrix based on the frame information and Camera settings resulting from the FOVs
    cv::Mat camera_frame = this->camera();
    // Getting the original Camera Matrix with scaled frame size and distortion Koefficent
    cv::Mat kalib_camera = camera_optimal_matrix();
    // Matrix for remapping the Picture
    cv::Mat map1, map2;
    // output Matrix after applying the new Camera matrix
    cv::Mat undistorted;

    this->initializeBalls();

    if (this->Ball_set[0] == nullptr)
    {
        this->Ball_set[2] = &this->red_ball;
        this->Ball_set[1] = &this->green_ball;
        this->Ball_set[0] = &this->blue_ball;
    }

    double fps = img.get(cv::CAP_PROP_FPS);

    if (fps > 0)
    {
        delay = std::max(1, static_cast<int>(lround(1000.0 / fps)));
    }

    cv::Mat frame, out;

    while (true)
    {
        img.read(frame);

        if (frame.empty())
            break;

        out.create(frame.size(), frame.type());

        this->framecount++;

        this->create_undistort_camera(
            camera_frame,
            kalib_camera,
            this->distCoeffs,
            frame.size(),
            map1,
            map2);

        cv::remap(
            frame,
            undistorted,
            map1,
            map2,
            cv::INTER_LINEAR,
            cv::BORDER_CONSTANT);

        this->m_frame_processing(undistorted, out);

        canvas->displayCanvas();

        this->m_display_info(undistorted, out);

        cv::imshow("video input", undistorted);
        cv::imshow("video output", out);

        int key = cv::waitKey(delay);
        if (key == 27 || key == 'q')
        {
            cv::destroyWindow("video input");
            cv::destroyWindow("video output");
            break;
        }
    }
}
