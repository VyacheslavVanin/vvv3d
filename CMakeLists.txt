cmake_minimum_required(VERSION 3.1)
project(vvv3dnew)
set (CMAKE_CXX_STANDARD 14)

find_package(Freetype REQUIRED)

include_directories(${FREETYPE_INCLUDE_DIRS} vvv3d .)

file(GLOB_RECURSE SRCS vvv3d/*.cpp)
file(GLOB_RECURSE HDRS *.h)

add_library(vvv3dnew SHARED ${SRCS} ${HDRS})
target_link_libraries(vvv3dnew GLEW GL png freetype SDL2)

set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
install(DIRECTORY vvv3d DESTINATION include FILES_MATCHING PATTERN "*.h")
install(TARGETS vvv3dnew DESTINATION lib)
