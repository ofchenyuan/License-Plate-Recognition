PROJECT(pjt)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(change_img src/change_img.cpp)
add_executable(cascade_car_test src/cascade_car_test.cpp)

link_directories(${OpenCV_LIBRARY_DIRS})

target_link_libraries(change_img ${OpenCV_LIBS})
target_link_libraries(cascade_car_test ${OpenCV_LIBS})

