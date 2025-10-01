cmake_minimum_required(VERSION 4.0)
project(Image_Processing_C___Test)

set(CMAKE_CXX_STANDARD 20)

add_executable(Image_Processing_C___Test main.cpp
        processor.cpp
        processor.h
        loop.cpp
        math.cpp
        assets.cpp
        vector.cpp
        camera.cpp
        Canvas/Canvas.cpp
        Canvas/Canvas.h)

set(OpenCV_DIR "/opt/homebrew/opt/opencv/lib/cmake/opencv4")
set(OpenMVG_DIR "$ENV{HOME}/local/openmvg/lib/openMVG")

find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs highgui calib3d viz)
find_package(OpenMVG REQUIRED CONFIG)

target_link_libraries(Image_Processing_C___Test PRIVATE ${OpenCV_LIBS})

set_target_properties(Image_Processing_C___Test PROPERTIES
        MACOSX_RPATH ON
        INSTALL_RPATH "@loader_path")
