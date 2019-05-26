# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5OpenGL CONFIG REQUIRED)

file(GLOB_RECURSE QT_HAL_SRCS
    vvv3d/private/qt_hal_impl/*.cpp
)

add_definitions(-DVVV3D_BACKEND_QT)

list(APPEND SRCS ${QT_HAL_SRCS})
list(APPEND QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL)
