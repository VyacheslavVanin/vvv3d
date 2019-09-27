cmake_minimum_required(VERSION 3.1)
project(vvv3d)
set (CMAKE_CXX_STANDARD 14)

set(OpenGL_GL_PREFERENCE "GLVND")

# Set Source files
file(GLOB_RECURSE SRCS
    vvv3d/core/*.cpp
    vvv3d/geometries/*.cpp
    vvv3d/gui/*.cpp
    vvv3d/lighting/*.cpp
    vvv3d/sprites/*.cpp
    vvv3d/utils/*.cpp
    vvv3d/vvvmath/*.cpp
)

if (NOT VVV3D_BACKEND)
    message("VVV3D_BACKEND variable was set to default value (SDL)")
    set(VVV3D_BACKEND "SDL")
endif(NOT VVV3D_BACKEND)

if (VVV3D_BACKEND STREQUAL "SDL")
    message("Use SDL2 backend")
    include(cmake/sdl_backend.cmake)
elseif (VVV3D_BACKEND STREQUAL "Qt")
    message("Use Qt backend")
    include(cmake/qt_backend.cmake)
else (VVV3D_BACKEND STREQUAL "SDL")
    message("VVV3D_BACKEND has invalid value (supported values are SDL)")
endif(VVV3D_BACKEND STREQUAL "SDL")


option(WITH_DEPENDENCIES "BUILD WITH DEPENDENCIES" ON)
if(WITH_DEPENDENCIES)
    add_subdirectory(vvvcfg)
endif(WITH_DEPENDENCIES)

option(WITH_GL_DEBUG "BUILD WITH GL DEBUG" OFF)
if(WITH_GL_DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_GL")
endif(WITH_GL_DEBUG)

# Set include path
include_directories(${FREETYPE_INCLUDE_DIRS}
                    ${SDL2_INCLUDE_DIRS}
                    vvv3d . vvvcfg stdhelper/include)

file(GLOB_RECURSE HDRS *.h *.hpp)
add_library(vvv3d SHARED ${SRCS} ${HDRS})


target_link_libraries(vvv3d
                      ${GLEW_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      ${FREETYPE_LIBRARIES}
                      ${FONTCONFIG_LIBRARIES}
                      ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES}
                      ${QT_LIBRARIES})

option(WITH_TESTS "BUILD WITH TESTS" OFF)
if(WITH_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(WITH_TESTS)

# Installation
set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
install(DIRECTORY vvv3d DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY vvv3d DESTINATION include FILES_MATCHING PATTERN "*.hpp")
install(TARGETS vvv3d DESTINATION lib)
install(DIRECTORY stdhelper/include/vvvstdhelper
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")
