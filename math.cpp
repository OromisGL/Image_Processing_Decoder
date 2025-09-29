//
// Created by Louis Krämer on 26.09.25.
//

#include "processor.h"

int screen_x = 1280 / 2;
int screen_y = 720 / 2;



void Processor::crossing_point(cv::Mat& out, std::vector<cv::Point2d>& output)
{
    cv::Point2d blue_middle(blue_ball.middle.first,blue_ball.middle.second);
    cv::Point2d green_middle(green_ball.middle.first, green_ball.middle.second);

    double dx = green_middle.x - blue_middle.x;
    double dy = green_middle.y - blue_middle.y;

    cv::Point2d direction(dx,dy);

    double d = std::hypot(dx, dy);
    double newBlueradius = blue_ball.radius + 180;
    double newGreenradius = green_ball.radius + 180;

    double rBsqrt = newBlueradius * newBlueradius;
    double rGsqrt = newGreenradius * newGreenradius;
    double dsqrt = d * d;

    double a = (rBsqrt - rGsqrt + dsqrt) / (2 * d);
    double h2 = rBsqrt - a * a;
    const double h = std::sqrt(std::max(0.0, h2));
    double s = a / d;

    cv::Point2d P0 = blue_middle + direction * s;
    cv::Point2d perpendicular(-dy, dx);
    cv::Point2d off = (h / d) * perpendicular;

    const cv::Point2d P1 = P0 + off;
    const cv::Point2d P2 = P0 - off;

    output.insert(output.end(), P1);


    cv::circle(out, P1, 3, cv::Scalar(255,255,255), cv::FILLED, cv::LINE_AA);
    //cv::circle(out, P2, 3, cv::Scalar(255,255,255), cv::FILLED, cv::LINE_AA);

}

void Processor::middle_point(Ball& b)
{
    const auto n = static_cast<double>(b.positions.size());

    if (n == 0)
        return;

    b.middle.first = static_cast<double>(b.x) / n;
    b.middle.second = static_cast<double>(b.y) / n;
}

void Processor::add_x_y(Ball& b)
{
    for (const auto& p : b.positions)
    {
        b.x += p.first;
        b.y += p.second;
    }
}

void Processor::draw_line_to_camera(Ball& b, cv::Mat& img)
{
    INITIAL_CAMERA_POSITION camera;
    cv::Point2f p1(b.middle.first,b.middle.second);

    cv::Point2f p2(screen_x, screen_y + 360);

    // float z = camera.z + len;

    // float d = std::sqrt(camera.x * camera.x + camera.y * camera.y + z * z);

    //std::cout << "\r"
    //      << "Vektor Länge "<< b.name << std::setw(8)  << d
    //      << std::flush;

    cv::line(img,
             cv::Point(cvRound(p1.x),    cvRound(p1.y)),
             cv::Point(cvRound(p2.x), cvRound(p2.y)),
             cv::Scalar(255,255,255), 2, cv::LINE_AA);
}