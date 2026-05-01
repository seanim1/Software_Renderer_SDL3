#!/usr/bin/env bash
# setup.sh — Linux / macOS (M1+)
set -e

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "Linux detected"
    echo "Installing dependencies..."
    sudo apt-get update
    sudo apt-get install -y \
        cmake pkg-config \
        libx11-dev libxext-dev libxcb-xtest0-dev \
        libxrandr-dev libxinerama-dev libxcursor-dev \
        libxi-dev libxss-dev libxkbcommon-dev \
        libwayland-dev wayland-protocols

    echo "Installing Vulkan SDK..."
    sudo apt-get install -y vulkan-tools libvulkan-dev libvulkan1
    
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "macOS (M1+) detected"
    if ! command -v cmake &> /dev/null; then
        brew install cmake
    fi
    
    echo "Installing Vulkan SDK..."
    brew install vulkan-headers vulkan-loader
    
else
    echo "Unsupported OS"
    exit 1
fi

echo "Building SDL3..."
if [ ! -d third_party/SDL3 ]; then
    mkdir -p third_party
    git clone --depth 1 https://github.com/libsdl-org/SDL.git third_party/SDL3
fi

cd third_party/SDL3
mkdir -p build
cd build

if [[ "$OSTYPE" == "darwin"* ]]; then
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64
else
    cmake .. -DCMAKE_BUILD_TYPE=Release -DSDL_X11_XTEST=OFF
fi

cmake --build . -j
cd - > /dev/null

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
