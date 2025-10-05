//
// Created by Louis Krämer on 26.09.25.
//

#include "processor.h"


std::vector<Ball> Processor::initializeBalls()
{
    Ball redball;
    Ball greenball;
    Ball blueball;
    const float triangle_length = DISTANCE_BETWEEN_BALL;
    const float left = -triangle_length / 2;
    const float right = triangle_length / 2;

    const float triangle_height = triangle_length * std::sqrt(3) / 2;
    const float bottom = camera_position.PEN_HEIGHT - triangle_height / 3;
    const float top = camera_position.PEN_HEIGHT + triangle_height * 2 / 3;

    std::vector<Ball> Ball_set;

    redball.positions.x = 0;
    redball.positions.y = top;
    redball.positions.z = 0;
    redball.color = cv::Scalar(0, 0, 255);

    greenball.positions.x = right;
    greenball.positions.y = bottom;
    greenball.positions.z = 0;
    greenball.color = cv::Scalar(0,255,0);

    blueball.positions.x = left;
    blueball.positions.y = bottom;
    blueball.positions.z = 0;
    blueball.color = cv::Scalar(255,0,0);

    Ball_set.push_back(blueball);
    Ball_set.push_back(greenball);
    // Ball_set.push_back(redball);

    return Ball_set;
}

void Processor::draw_to_screen(cv::Mat& out, std::vector<cv::Point2d>& drawPoints)
{
    crossing_point(out, drawPoints);

    std::vector<cv::Point> pts;
    pts.reserve(drawPoints.size());
    for (const auto& p : drawPoints)
        pts.emplace_back(cvRound(p.x), cvRound(p.y));

    cv::polylines(
        out,
        std::vector<std::vector<cv::Point>>{pts},
        false,
        cv::Scalar(255,255,255),
        2,
        cv::LINE_AA
    );
}

void Processor::draw_line(cv::Mat& img, cv::Point2f p1, cv::Point2f p2, cv::Scalar color = cv::Scalar(255,255,255))
{
    cv::line(img,
             cv::Point(cvRound(p1.x),    cvRound(p1.y)),
             cv::Point(cvRound(p2.x), cvRound(p2.y)),
             color , 1, cv::LINE_AA);
}

void Processor::m_display_info(cv::Mat& frame, cv::Mat& out)
{

    cv::Point2f blue_middle(blue_ball.middle.first,blue_ball.middle.second);
    cv::Point2f green_middle(green_ball.middle.first, green_ball.middle.second);

    cv::Point2f blue_green_center = middle_two_vek(blue_middle, green_middle);

    cv::Point2f target = target_vek(blue_ball, blue_middle, green_middle);


    draw_line(out, blue_middle, green_middle);
    // draw_line(out, blue_green_center, target);



    double len_blue_green = vektor_length(blue_middle, green_middle);

    // Make Point for  Middle Point in the Frame
    const std::string label = cv::format("Red radii: %.6f  Green radii: %.6f  Blue radii: %.6f  Distance Green - Blue: %.6f",
                                             red_ball.radius, green_ball.radius, blue_ball.radius, len_blue_green);

    int baseline = 0;
    const int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    const double fontScale = 0.6;
    const int thickness = 1;


    cv::Size sz = cv::getTextSize(label, fontFace, fontScale, thickness, &baseline);

    cv::Rect bg(10, 10, sz.width + 12, sz.height + baseline + 10);

    //draw_line(blue_ball, out);
    //draw_line(green_ball, out);
    // draw_line(red_ball, out);

    /*cv::Mat maskBlue = cv::Mat::zeros(out.size(), CV_8U);
    cv::Mat maskGreen = cv::Mat::zeros(out.size(), CV_8U);

    int rB = cvRound(blue_ball.radius + 180);
    int rG = cvRound(green_ball.radius + 180);

    cv::circle(maskBlue, blue_middle, rB, 255, 1, cv::LINE_8);
    cv::circle(maskGreen, green_middle, rG, 255, 1, cv::LINE_8);

    cv::Mat inter;
    cv::bitwise_and(maskBlue, maskGreen, inter);

    std::vector<cv::Point> crossing;
    cv::findNonZero(inter, crossing);

    for (const auto& p : crossing)
    {
        cv::circle(out, p, 3, cv::Scalar(255,255,255), cv::FILLED, cv::LINE_AA);
    }
    */
    /*
    std::vector<cv::Point> ringPtsBlue;
    std::vector<cv::Point> ringPtsGreen;


    cv::ellipse2Poly(blue_middle,
    cv::Size(
        static_cast<int>(blue_ball.radius + 180),
        static_cast<int>(blue_ball.radius + 180)),
        0, 0,360, 1,
        ringPtsBlue);

    cv::ellipse2Poly(green_middle,
    cv::Size(
        static_cast<int>(green_ball.radius + 180),
        static_cast<int>(green_ball.radius + 180)),
        0, 0,360, 1,
        ringPtsGreen);


    // Hashmap der Punktwolke für schnelleres Finden der Schnittpunkte !zu ungenau
    auto key = [](int x, int y)->uint64_t
    {
        return (uint64_t(uint32_t(x)) << 32) | uint32_t(y);
    };

    std::unordered_set<uint64_t> blueSet;

    blueSet.reserve(ringPtsBlue.size()*2);

    for (const auto& p : ringPtsBlue)
    {
        blueSet.insert(key(p.x, p.y));
    }

    for (const auto& p : ringPtsGreen)
    {
        if (blueSet.count(key(p.x, p.y)))
        {
            cv::circle(
            out,
            {static_cast<int>(p.x), static_cast<int>(p.y)},
            1, cv::Scalar(255,255,255),
            cv::FILLED,
            cv::LINE_AA
            );
        }

    }
    */

    cv::circle(
        out,
        {static_cast<int>(blue_green_center.x), static_cast<int>(blue_green_center.y)},
        3, cv::Scalar(255,255,255),
        cv::FILLED,
        cv::LINE_AA
        );

    cv::circle(
        out,
        {static_cast<int>(blue_ball.middle.first), static_cast<int>(blue_ball.middle.second)},
        static_cast<int>(blue_ball.distances / 5), cv::Scalar(255,255,255),
        1,
        cv::LINE_AA);

    cv::circle(
        out,
        {static_cast<int>(green_ball.middle.first), static_cast<int>(green_ball.middle.second)},
        static_cast<int>(green_ball.distances / 5), cv::Scalar(255,255,255),
        1,
        cv::LINE_AA);

    cv::circle(
        frame,
        {static_cast<int>(red_ball.middle.first), static_cast<int>(red_ball.middle.second)},
        5, cv::Scalar(255,255,255),
        cv::FILLED,
        cv::LINE_AA);




    cv::rectangle(out, bg, cv::Scalar(255, 255, 255), cv::FILLED);

    cv::putText(out, label,
            cv::Point(bg.x + 6, bg.y + 6 + sz.height),
            fontFace, fontScale, cv::Scalar(0, 0, 0), thickness, cv::LINE_AA);
}

cv::VideoCapture Processor::in_video(const std::string& video_path)
{
    if (!std::filesystem::exists(video_path)) {
        std::cerr << "Datei nicht gefunden: " << video_path << "\n";
        return {};
    }

    cv::VideoCapture in_video(video_path, cv::CAP_ANY);

    in_video.release();
    if (in_video.open(video_path, cv::CAP_AVFOUNDATION) && in_video.isOpened()) {
        std::cout << "Backend: " << in_video.getBackendName() << "\n";
        return in_video;
    }

    std::cerr << "Konnte Video nicht öffnen: " << video_path << "\n";
    return {};
}