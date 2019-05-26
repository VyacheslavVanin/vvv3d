# Check packages
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image)
PKG_SEARCH_MODULE(FONTCONFIG REQUIRED fontconfig)
PKG_SEARCH_MODULE(GLEW REQUIRED glew)

find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED)

add_definitions(-DVVV3D_BACKEND_SDL)
add_definitions(-DVVV3D_BACKEND_FREETYPE)
add_definitions(-DVVV3D_BACKEND_FONTCONFIG)


file(GLOB_RECURSE FONTCONFIG_SRCS
    vvv3d/private/fc_systemfonts_impl/*.cpp
)
file(GLOB_RECURSE FREETYPE_SRCS
    vvv3d/private/freetype_font_impl/*.cpp
)
file(GLOB_RECURSE SDL_HAL_SRCS
    vvv3d/private/sdl_hal_impl/*.cpp
)

list(APPEND SRCS ${FREETYPE_SRCS} ${FONTCONFIG_SRCS} ${SDL_HAL_SRCS})
