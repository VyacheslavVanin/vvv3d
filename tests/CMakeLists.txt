cmake_minimum_required(VERSION 3.1)
project(vvvcfg_test)

enable_testing()

set (CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS} .)

file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HDRS *.h *.hpp)
add_executable(vvv3d_test ${SRCS} ${HDRS})
target_link_libraries(vvv3d_test vvvcfg vvv3d ${GTEST_LIBRARIES} Threads::Threads)

add_test(vvv3d_test vvv3d_test)
