//
// Created by Louis Krämer on 25.09.25.
//

#include "processor.h"
#include "Canvas/Canvas.h"
#include <memory>

void Processor::m_frame_processing(cv::Mat &input, cv::Mat &output)
{
    // Reset the Positional informations
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

            if (newB == 255)
            {
                blue_field += 1;
                this->blue_ball.positions.x += x;
                this->blue_ball.positions.y += y;
            }
            else if (newG == 255)
            {
                green_field += 1;
                this->green_ball.positions.x += x;
                this->green_ball.positions.y += y;
            }
            else if (newR == 255)
            {
                red_field += 1;
                this->red_ball.positions.x += x;
                this->red_ball.positions.y += y;
            }

            out_row[x] = cv::Vec3b(
                static_cast<uchar>(newB),
                static_cast<uchar>(newG),
                static_cast<uchar>(newR)
            );
        }
    }

    // Area based of the Ball Color informations
    this->blue_ball.field = static_cast<int>(blue_field);
    this->green_ball.field = static_cast<int>(green_field);
    this->red_ball.field = static_cast<int>(red_field);

    // Calculationg the middle in x y
    middle_point(this->blue_ball);
    middle_point(this->green_ball);
    middle_point(this->red_ball);

    // Getting the radius from the Area
    this->blue_ball.radius = std::sqrt(static_cast<double>(this->blue_ball.field) / M_PI);
    this->green_ball.radius = std::sqrt(static_cast<double>(this->green_ball.field) / M_PI);
    this->red_ball.radius = std::sqrt(static_cast<double>(this->red_ball.field) / M_PI);

    // Init the radii to get the Ratio
    if (this->camera_position.initial_blue_radii == 0.0 ||
        this->camera_position.initial_green_radii == 0.0)
    {
        this->camera_position.initial_blue_radii = this->blue_ball.radius;
        this->camera_position.initial_green_radii = this->green_ball.radius;

        std::cout << "blue radii: " << blue_ball.radius <<
            "green radii: " << green_ball.radius <<
                "red radii: " << red_ball.radius << std::endl;
    }

    // Init the distance
    if (this->camera_position.initial_blue_distance == 0.0 ||
        this->camera_position.initial_green_distance == 0.0)
    {
        this->camera_position.initial_blue_distance = euclidian_distance_3D(
            this->blue_ball,
            this->camera_position);
        this->camera_position.initial_green_distance = euclidian_distance_3D(
            this->green_ball,
            this->camera_position);
    }

    // Calculating the Ratio of the Current radii to the initial radii
    double blue_ratio = this->camera_position.initial_blue_radii / this->blue_ball.radius;
    double green_ratio = this->camera_position.initial_green_radii / this->green_ball.radius;

    // getting the right distance based on the ratio
    this->blue_ball.distances = this->camera_position.initial_blue_distance * blue_ratio;
    this->green_ball.distances = this->camera_position.initial_green_distance * green_ratio;
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
