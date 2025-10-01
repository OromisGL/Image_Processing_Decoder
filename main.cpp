#include <iostream>
#include "processor.h"
#include "Canvas/Canvas.h"

int main()
{

    std::string img_path = "../img/1.mp4";
    Processor processor;
    Canvas canvas;
    cv::VideoCapture video = processor.in_video(img_path);

    // processor.video_loop(video);
    canvas.displayCanvas();

}