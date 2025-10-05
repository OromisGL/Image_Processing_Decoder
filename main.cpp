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

    setSharedProcessor(&processor);
    setSharedCanvas(&canvas);

    cv::VideoCapture video = processor.in_video(img_path);
    //canvas.displayCanvas();

    processor.video_loop(video);

}