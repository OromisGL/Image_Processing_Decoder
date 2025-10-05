//
// Created by Louis Krämer on 25.09.25.
//

#include "processor.h"
#include "Canvas/Canvas.h"
#include <memory>


void Processor::m_frame_processing(cv::Mat &input, cv::Mat &output)
{
    this->blue_ball.positions.x = 0;
    this->green_ball.positions.x = 0;
    this->red_ball.positions.x = 0;

    this->blue_ball.positions.y = 0;
    this->green_ball.positions.y = 0;
    this->red_ball.positions.y = 0;

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

            if (newB == 255){ blue_field += 1; blue_ball.positions.x += x;  blue_ball.positions.y += y; }
            else if (newG == 255){ green_field += 1; green_ball.positions.x += x;  green_ball.positions.y += y; }
            else if (newR == 255){ red_field += 1; red_ball.positions.x += x;  red_ball.positions.y += y;}

            out_row[x] = cv::Vec3b(
                static_cast<uchar>(newB),
                static_cast<uchar>(newG),
                static_cast<uchar>(newR)
            );
        }
    }

    blue_ball.field = static_cast<int>(blue_field);
    green_ball.field = static_cast<int>(green_field);
    red_ball.field = static_cast<int>(red_field);

    middle_point(blue_ball);
    middle_point(green_ball);
    middle_point(red_ball);

    this->blue_ball.radius = std::sqrt(static_cast<double>(blue_ball.field) / M_PI);
    this->green_ball.radius = std::sqrt(static_cast<double>(green_ball.field) / M_PI);
    this->red_ball.radius = std::sqrt(static_cast<double>(red_ball.field) / M_PI);

    euclidian_distance_3D(green_ball, camera_position);
    euclidian_distance_3D(blue_ball, camera_position);

    camera_position.z = green_ball.distances * (std::sqrt(3) / 2);

    //euclidian_distance_2D(blue_ball, camera_position);
    //euclidian_distance_2D(green_ball, camera_position);

    //camera_position.initial_blue = blue_ball.distances;
    //camera_position.initial_green = green_ball.distances;

    //std::cout << "\r"
    //      << "Red radii: "   << std::fixed  << red_ball.radius
    //<< "Blue Middle: " << std::setw(8) << blue_ball.middle.first
    //      << "  Green radii: " << std::setw(8) << green_ball.radius
    //      << "  Blue radii: "  << std::setw(8) << blue_ball.radius
    //      << std::flush;
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
