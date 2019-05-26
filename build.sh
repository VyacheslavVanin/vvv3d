#!/bin/bash
mkdir -p ./build
cd build
time ( \
    cmake .. \
        -DWITH_TESTS=yes \
        -DWITH_GL_DEBUG=no \
        -DVVV3D_BACKEND=Qt \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=yes; \
    make -j4; \
)
cp compile_commands.json ..
