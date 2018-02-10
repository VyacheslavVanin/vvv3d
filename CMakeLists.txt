cmake_minimum_required(VERSION 3.1)
project(vvv3d)
set (CMAKE_CXX_STANDARD 14)

# Check packeages
find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(FONTCONFIG REQUIRED fontconfig)
PKG_SEARCH_MODULE(GLEW REQUIRED glew)

option(WITH_DEPENDENCIES "BUILD WITH DEPENDENCIES" ON)
if(WITH_DEPENDENCIES)
    add_subdirectory(vvvcfg)
endif(WITH_DEPENDENCIES)

# Set include path
include_directories(${FREETYPE_INCLUDE_DIRS}
                    ${SDL2_INCLUDE_DIRS}
                    vvv3d . vvvcfg)

# Set Source files
file(GLOB_RECURSE SRCS vvv3d/*.cpp)
file(GLOB_RECURSE HDRS *.h *.hpp)
add_library(vvv3d SHARED ${SRCS} ${HDRS})

target_link_libraries(vvv3d
                      ${GLEW_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      ${PNG_LIBRARIES} ${JPEG_LIBRARIES} ${FREETYPE_LIBRARIES}
                      ${FONTCONFIG_LIBRARIES}
                      ${SDL2_LIBRARIES})

enable_testing()
add_subdirectory(tests)

# Installation
set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
install(DIRECTORY vvv3d DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY vvv3d DESTINATION include FILES_MATCHING PATTERN "*.hpp")
install(TARGETS vvv3d DESTINATION lib)
