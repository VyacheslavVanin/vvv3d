#!/bin/bash
mkdir -p ./build
cd build
time ( \
    cmake .. \
        -DVVV3D_BACKEND=SDL \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=yes; \
    make -j4; \
)
cp compile_commands.json ..
