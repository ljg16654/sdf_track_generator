project(ParseSDF)
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
find_package(SDFormat REQUIRED)

include_directories(${SDFormat_INCLUDE_DIRS})

add_executable(gen_sdf mod_cone_pose.cc)
add_executable(gen_track RandomTrackGenerator.cpp)
target_link_libraries(gen_sdf ${SDFormat_LIBRARIES})
