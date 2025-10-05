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

    double fps = img.get(cv::CAP_PROP_FPS);

    if (fps > 0)
    {
        delay = std::max(1, static_cast<int>(lround(1000.0 / fps)));
    }

    cv::Mat frame, out;

    Canvas& canvas_ = getCanvas();
    Processor& processor_ = getProcessor();

    //virtual_camera_setting(90);

    while (true)
    {
        img.read(frame);

        if (frame.empty())
            break;

        out.create(frame.size(), frame.type());

        //out = applyVcam(frame);

        processor_.m_frame_processing(frame, out);

        canvas_.displayCanvas();

        processor_.m_display_info(frame, out);
        //draw_to_screen(out, screen.drawPoints);

        // cv::imshow("video input", frame);
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