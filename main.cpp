#include <iostream>
#include "processor.h"
#include "Canvas/Canvas.h"
#include <memory>
#include "setter.h"

int main()
{

    std::string img_path = "../img/1.mp4";

    auto canvas = std::make_shared<Canvas>();
    auto processor = std::make_shared<Processor>();

    // Shared Pointer of Processor and Canvas
    setSharedCanvas(canvas);
    setSharedProcessor(processor);

    // Start Video read
    cv::VideoCapture video = processor->in_video(img_path);

    if (cv::VideoCapture V(0); !V.isOpened()) { // Check if the webcam is opened
        std::cerr << "Error: Webcam not detected!" << std::endl;
        return -1;
    }

    //Start Videoloop
    processor->video_loop(video);
}