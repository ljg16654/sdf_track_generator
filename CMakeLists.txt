project(ParseSDF)
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
find_package(SDFormat REQUIRED)

include_directories(${SDFormat_INCLUDE_DIRS})

add_executable(check_sdf check_sdf.cc)
target_link_libraries(check_sdf ${SDFormat_LIBRARIES})
