//
// Created by Louis Krämer on 28.09.25.
//

#include "processor.h"

double Processor::vektor_length(cv::Point2f p1, cv::Point2f p2)
{
    cv::Point2f dir = p2 - p1;
    double len = std::hypot(dir.x, dir.y);
    return len;
}

cv::Point2d Processor::middle_two_vek(cv::Point2d p1, cv::Point2d p2)
{
    return (p2 + p1) / 2;
}

cv::Point2d Processor::target_vek(Ball& b, cv::Point2f p1, cv::Point2f p2)
{
    cv::Point2f middle = middle_two_vek(p1, p2);
    cv::Point2f ball_middle(static_cast<float>(b.middle.first), static_cast<float>(b.middle.second));
    double length_opposite = vektor_length(ball_middle, middle);
    double length_hypot = b.radius + 180;
    double length_adjacent = std::sin(length_opposite / length_hypot) * length_hypot;
    double target = std::cos(length_adjacent/ length_hypot) * length_hypot;

    return {static_cast<float>(length_hypot), static_cast<float>(target)};
}
