//
// Created by Louis Krämer on 11.10.25.
//

#include "Simulation.h"

#include "processor.h"


void Simulation::Simulation_loop()
{
    auto processor = getProcessor();
    auto canvas = getCanvas();

    processor->initializeBalls();

    if (processor->Ball_set[0] == nullptr)
    {
        processor->Ball_set[2] = &processor->red_ball;
        processor->Ball_set[1] = &processor->green_ball;
        processor->Ball_set[0] = &processor->blue_ball;
    }

    auto width = processor->screen.width * 2;
    auto height = processor->screen.height * 2;

    int canvas_ratio = width / height;

    float half_x = width * 0.5;
    float half_y = height * 0.5;

    cv::viz::Viz3d simulation_canvas("Simulation");

    simulation_canvas.setWindowSize(cv::Size(width, height));
    simulation_canvas.setBackgroundColor(cv::viz::Color::black());
    cv::namedWindow("video output", cv::WINDOW_AUTOSIZE);

    cv::viz::WCoordinateSystem koordinate_system(canvas_ratio);

    double scaler = 15.;

    cv::Point3d p_blue(processor->blue_ball.x * scaler,
                       processor->blue_ball.y * scaler,
                       processor->blue_ball.z * scaler);

    cv::Point3d p_green(processor->green_ball.x * scaler,
                        processor->green_ball.y * scaler,
                        processor->green_ball.z * scaler);

    cv::Point3d p_red(processor->red_ball.x * scaler,
                      processor->red_ball.y * scaler,
                      processor->red_ball.z * scaler);

    // 1) Mittelpunkt (Schwerpunkt / centroid des Dreiecks)
    cv::Point3d scene_center;
    scene_center.x = (p_blue.x + p_green.x + p_red.x) / 3.0;
    scene_center.y = (p_blue.y + p_green.y + p_red.y) / 3.0;
    scene_center.z = (p_blue.z + p_green.z + p_red.z) / 3.0;

    // 2) Bounding-Radius (maximale Distanz vom Mittelpunkt zur äußeren Oberfläche)
    double ball_radius_world = 3.5 * scaler; // wie in Ihrem WSphere-Aufruf
    double max_distance = 0.0;
    auto consider = [&](const cv::Point3d &p)
    {
        double d = cv::norm(p - scene_center) + ball_radius_world;
        if (d > max_distance) max_distance = d;
    };
    consider(p_blue);
    consider(p_green);
    consider(p_red);

    cv::viz::WSphere blue(cv::Point3d(processor->blue_ball.x * scaler,
                                      processor->blue_ball.y * scaler, processor->blue_ball.z),
                          3.5 * scaler,
                          128,
                          cv::viz::Color::blue());

    cv::viz::WSphere green(cv::Point3d(processor->green_ball.x * scaler,
                                       processor->green_ball.y * scaler, processor->green_ball.z),
                           3.5 * scaler,
                           128,
                           cv::viz::Color::green());

    cv::viz::WSphere red(cv::Point3d(processor->red_ball.x * scaler,
                                     processor->red_ball.y * scaler, processor->red_ball.z),
                         3.5 * scaler,
                         128,
                         cv::viz::Color::red());

    simulation_canvas.showWidget(processor->blue_ball.name, blue);
    simulation_canvas.showWidget(processor->green_ball.name, green);
    simulation_canvas.showWidget(processor->red_ball.name, red);

    simulation_canvas.showWidget("Koordinatensystem", koordinate_system);

    double fx = processor->camera().at<double>(0, 0);
    double fy = processor->camera().at<double>(1, 1);
    double cx = processor->camera().at<double>(0, 2);
    double cy = processor->camera().at<double>(1, 2);


    cv::viz::Camera perspectiveCamera(
        fx,
        fy,
        half_x,
        half_y,
        cv::Size(width, height));

    simulation_canvas.spinOnce(1, true);

    simulation_canvas.setCamera(perspectiveCamera);

    // Viewer Position
    cv::Vec3d eye(0.0, processor->camera_position.PEN_HEIGHT * scaler, -processor->camera_position.z * scaler);
    // look at vector
    cv::Vec3d center(scene_center.x, scene_center.y, 0.0);
    // Up
    cv::Vec3d up(0.0, -1.0, 0.0);

    std::vector<cv::Vec3d> path = generate_path(eye, 50.0, 300.0);

    cv::Affine3d pose = cv::viz::makeCameraPose(eye, center, up);

    simulation_canvas.setViewerPose(pose);

    //cv::VideoWriter videoWriter("simulation.avi",
                                //cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                //60.0, // FPS
                                //cv::Size(width, height));

    int current_index = 0;

    cv::Mat out;

    while (current_index < path.size())
    {

        out.create(cv::Size(width * 2, height * 2), out.type());

        eye = path[current_index];
        pose = cv::viz::makeCameraPose(eye, center, up);
        simulation_canvas.setViewerPose(pose);

        simulation_canvas.spinOnce(10, true);

        cv::Mat snaps = simulation_canvas.getScreenshot();

        //videoWriter.write(snaps);

        processor->m_frame_processing(snaps, out);

        canvas->displayCanvas();

        cv::imshow("video output", out);

        current_index++;
    }

    //videoWriter.release();

}

std::vector<cv::Vec3d> Simulation::generate_path(cv::Vec3d &start, double radius, double point_count)
{
    std::vector<cv::Vec3d> points;
    points.reserve(point_count);

    for (int i = 0; i < point_count; i++)
    {
        double angle = ((double) i / point_count) * M_PI * 2;

        double x = start[0] + radius * std::cos(angle);
        double z = start[2] + radius * std::sin(angle);
        double y = start[1];

        points.push_back(cv::Vec3d(x, y, z));
    }
    return points;
}
