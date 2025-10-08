//
// Created by Louis Krämer on 26.09.25.
//

#include "processor.h"
#include "Canvas/Canvas.h"
#include "setter.h"

void Processor::video_loop(cv::VideoCapture& img)
{

    cv::namedWindow("video input", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("video output", cv::WINDOW_AUTOSIZE);

    int delay = 1;
    auto canvas = getCanvas();
    auto processor = getProcessor();

    cv::Mat camera_frame = processor->camera();
    cv::Mat kalib_camera = camera_optimal_matrix();
    cv::Mat map1, map2;

    cv::Mat undistorted;

    processor->initializeBalls();

    if (processor->Ball_set[0] == nullptr)
    {
        processor->Ball_set[2] = &processor->red_ball;
        processor->Ball_set[1] = &processor->green_ball;
        processor->Ball_set[0] = &processor->blue_ball;
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

        processor->framecount++;

        processor->create_undistort_camera(camera_frame, kalib_camera, processor->distCoeffs, frame.size(), map1, map2);

        cv::remap(frame, undistorted, map1, map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT);

        processor->m_frame_processing(undistorted, out);
        canvas->displayCanvas();

        processor->m_display_info(undistorted, out);

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