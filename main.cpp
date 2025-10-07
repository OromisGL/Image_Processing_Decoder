#include <iostream>
#include "processor.h"
#include "Canvas/Canvas.h"
#include <memory>
#include "setter.h"

int main()
{

    std::string img_path = "../img/1.mp4";

    Processor processor;
    Canvas canvas;

    // Shared Pointer of Processor and Canvas
    setSharedProcessor(&processor);
    setSharedCanvas(&canvas);

    // Start Video read
    cv::VideoCapture video = processor.in_video(img_path);

    // init Default Values of the Balls
    processor.initializeBalls();

    if (processor.Ball_set[0] == nullptr)
    {
        processor.Ball_set[2] = &processor.red_ball;
        processor.Ball_set[1] = &processor.green_ball;
        processor.Ball_set[0] = &processor.blue_ball;
    }
    processor.video_loop(video);
}