cmake_minimum_required(VERSION 3.1)
project(convexsky)
set (CMAKE_CXX_STANDARD 14)

include_directories(.)

file(GLOB_RECURSE SRCS gui/*.cpp main.cpp)
file(GLOB_RECURSE HDRS *.h *.hpp)

add_executable(convexsky ${SRCS} ${HDRS})
target_link_libraries(convexsky vvv3dnew GL GLEW freetype png SDL2)
