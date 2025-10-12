//
// Created by Louis Krämer on 26.09.25.
//

#include "processor.h"
#include "setter.h"

int screen_x = 1280 / 2;
int screen_y = 720 / 2;


bool Processor::locate_point(cv::Point& p)
{
    int x0 = (p.x / 5) * 5;
    int y0 = (p.y / 5) * 5;

    for (int i = -5; i <= 5; i++)
    {
        for (int j = -5; j <= 5; j++)
        {
            int dx = p.x + j;
            int dy = p.y + i;
            if (dx > 0 && dx <= screen.width &&
                dy > 0 && dy <= screen.height)
            {

            }
        }
    }
    return false;
}

double Processor::get_delta(Ball& b)
{
    return b.distances;
}

void Processor::crossing_point(cv::Mat& out, std::vector<cv::Point2d>& output)
{
    cv::Point2d blue_middle(blue_ball.x, blue_ball.y);
    cv::Point2d green_middle(green_ball.x, green_ball.y);

    double dx = green_middle.x - blue_middle.x;
    double dy = green_middle.y - blue_middle.y;

    cv::Point2d direction(dx,dy);

    double d = std::hypot(dx, dy);
    double blueradius = blue_ball.distances;
    double greenradius = green_ball.distances;

    double rBsqrt = blueradius * blueradius;
    double rGsqrt = greenradius * greenradius;
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

}

void Processor::middle_point(Ball& b)
{
    const auto n = static_cast<double>(b.field);

    if (n == 0)
        return;

    b.middle.first = b.positions.x / n;
    b.middle.second = b.positions.y / n;
}

void Processor::draw_line_to_camera(Ball& b, cv::Mat& img)
{
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