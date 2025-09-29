#include <iostream>
#include "processor.h"

int main()
{

    std::string img_path = "../img/1.mp4";
    Processor processor;
    cv::VideoCapture video = processor.in_video(img_path);

    processor.video_loop(video);
}