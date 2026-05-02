#!/usr/bin/env bash
set -e

if ! command -v xcodebuild &> /dev/null; then
    echo "Error: Xcode not found"
    exit 1
fi

brew install cmake

DEPLOYMENT_TARGET="15.0"

if [ ! -d third_party/SDL3 ]; then
    mkdir -p third_party
    git clone --depth 1 https://github.com/libsdl-org/SDL.git third_party/SDL3
fi

(
    cd third_party/SDL3
    mkdir -p build
    cd build
    cmake .. -G Xcode \
        -DCMAKE_SYSTEM_NAME=iOS \
        -DCMAKE_OSX_ARCHITECTURES=arm64 \
        -DCMAKE_OSX_DEPLOYMENT_TARGET=$DEPLOYMENT_TARGET \
        -DSDL_SHARED=OFF \
        -DSDL_STATIC=ON \
        -DSDL_TESTS=OFF
    cmake --build . --config Debug
)

cmake -S . -B build -G Xcode \
    -DCMAKE_SYSTEM_NAME=iOS \
    -DCMAKE_OSX_ARCHITECTURES=arm64 \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=$DEPLOYMENT_TARGET