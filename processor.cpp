//
// Created by Louis Krämer on 25.09.25.
//

#include "processor.h"
#include "Canvas/Canvas.h"
#include <memory>
#include "setter.h"

void Processor::m_frame_processing(cv::Mat &input, cv::Mat &output)
{
    auto processor = getProcessor();

    // Reset the Positional informations
    processor->blue_ball.positions.x = 0;
    processor->green_ball.positions.x = 0;
    processor->red_ball.positions.x = 0;

    processor->blue_ball.positions.y = 0;
    processor->green_ball.positions.y = 0;
    processor->red_ball.positions.y = 0;

    uint32_t blue_field = 0, green_field = 0, red_field = 0;

    for (int y = 0; y < input.rows; y++)
    {
        auto *row = input.ptr<cv::Vec3b>(y);
        auto *out_row = output.ptr<cv::Vec3b>(y);

        for (int x = 0; x < input.cols; x++)
        {
            cv::Vec3b &img_p = row[x];

            int blue = img_p[0];
            int green = img_p[1];
            int red = img_p[2];

            int diffB = blue - (red + green) / 2;
            int diffG = green - (red + blue) / 2;
            int diffR = red - (green + blue) / 2;

            int newB = diffB > threshold ? 255 : 0;
            int newG = diffG > threshold ? 255 : 0;
            int newR = diffR > threshold ? 255 : 0;

            if (newB == 255){ blue_field += 1; processor->blue_ball.positions.x += x;  processor->blue_ball.positions.y += y; }
            else if (newG == 255){ green_field += 1; processor->green_ball.positions.x += x;  processor->green_ball.positions.y += y; }
            else if (newR == 255){ red_field += 1; processor->red_ball.positions.x += x;  processor->red_ball.positions.y += y; }

            out_row[x] = cv::Vec3b(
                static_cast<uchar>(newB),
                static_cast<uchar>(newG),
                static_cast<uchar>(newR)
            );
        }
    }

    // Area based of the Ball Color informations
    processor->blue_ball.field = static_cast<int>(blue_field);
    processor->green_ball.field = static_cast<int>(green_field);
    processor->red_ball.field = static_cast<int>(red_field);

    // Calculationg the middle in x y
    middle_point(processor->blue_ball);
    middle_point(processor->green_ball);
    middle_point(processor->red_ball);

    // Getting the radius from the Area
    processor->blue_ball.radius = std::sqrt(static_cast<double>(processor->blue_ball.field) / M_PI);
    processor->green_ball.radius = std::sqrt(static_cast<double>(processor->green_ball.field) / M_PI);
    processor->red_ball.radius = std::sqrt(static_cast<double>(processor->red_ball.field) / M_PI);

    // Init the radii to get the Ratio
    if (processor->camera_position.initial_blue_radii == 0.0 || processor->camera_position.initial_green_radii == 0.0)
    {
        processor->camera_position.initial_blue_radii = processor->blue_ball.radius;
        processor->camera_position.initial_green_radii = processor->green_ball.radius;
    }

    // Init the distance
    if (processor->camera_position.initial_blue_distance == 0.0 || processor->camera_position.initial_green_distance == 0.0)
    {
        processor->camera_position.initial_blue_distance = euclidian_distance_3D(processor->blue_ball, processor->camera_position);
        processor->camera_position.initial_green_distance = euclidian_distance_3D(processor->green_ball, camera_position);
    }

    // Calculating the Ratio of the Current radii to the initial radii
    double blue_ratio = processor->camera_position.initial_blue_radii / processor->blue_ball.radius;
    double green_ratio = processor->camera_position.initial_green_radii / processor->green_ball.radius;

    // getting the right distance based on the ratio
    processor->blue_ball.distances = processor->camera_position.initial_blue_distance * blue_ratio;
    processor->green_ball.distances = processor->camera_position.initial_green_distance * green_ratio;

}

/* Test function

cv::Mat Processor::image_color_field(const std::string& img_path, const std::string& img_name)
{
    cv::Mat img = cv::imread(img_path + "/" + img_name, cv::IMREAD_COLOR);
    cv::Mat output(img.rows, img.cols, img.type(), cv::Scalar(0, 0, 0));

    cv::Mat frame;

    if (img.empty()) {
        std::cerr << "Could not open or find the image" << std::endl;
    }

    for (int y = 0; y < img.rows; y++) {
        auto* row = img.ptr<cv::Vec3b>(y);
        auto* out_row = output.ptr<cv::Vec3b>(y);
        for (int x = 0; x < img.cols; x++) {
            cv::Vec3b& img_p = row[x];
            cv::Vec3b& out_p = out_row[x];

            int blue = img_p[0];
            int green = img_p[1];
            int red = img_p[2];

            int diffB = blue - (red + green) / 2;
            int diffG = green - (red + blue) / 2;
            int diffR = red - (green + blue) / 2;

            int newB = diffB > threshold ? 255 : 0;
            int newG = diffG > threshold ? 255 : 0;
            int newR = diffR > threshold ? 255 : 0;

            count.blue += newB / 255;
            count.green += newG / 255;
            count.red += newR / 255;

            out_p = cv::Vec3b(
                static_cast<uchar>(newB),
                static_cast<uchar>(newG),
                static_cast<uchar>(newR)
                );
        }
    }

    std::cout << "Red:: " << count.red << "\n" << "Green: " << count.green << "\n" << "Blue: " << count.blue << std::endl;

    red_ball.radius = std::sqrt(count.red / M_PI );
    green_ball.radius = std::sqrt(count.green / M_PI);
    blue_ball.radius = std::sqrt(count.blue / M_PI);

    std::cout << "Red Ball Radii: " << red_ball.radius << std::endl;
    std::cout << "Green Ball Radii: " << green_ball.radius << std::endl;
    std::cout << "Blue Ball Radii: " << blue_ball.radius << std::endl;

    cv::imshow("Display input", img);
    cv::imshow("Display output",output);
    int k = cv::waitKey(0); // Wait for a keystroke in the window

    if(k == 's')
    {
        cv::destroyAllWindows();
    }
    return output;
}
*/
