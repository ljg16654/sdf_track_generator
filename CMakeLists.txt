project(ParseSDF)
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
find_package(SDFormat REQUIRED)

include_directories(${SDFormat_INCLUDE_DIRS})

add_executable(mod_cone_pose mod_cone_pose.cc)
target_link_libraries(mod_cone_pose ${SDFormat_LIBRARIES})
