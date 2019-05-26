#!/bin/bash
mkdir -p ./build
cd build
time ( \
    make test; \
)
