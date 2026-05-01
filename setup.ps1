# setup.ps1 — Windows
$ErrorActionPreference = "Stop"

Write-Host "Windows setup"
Write-Host ""

# Check for CMake
if (-Not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Host "CMake not found. Install from https://cmake.org/download/"
    exit 1
}

# Check for Vulkan SDK
if (-Not (Test-Path "C:\VulkanSDK")) {
    Write-Host "⚠ Vulkan SDK not found at C:\VulkanSDK"
    Write-Host "Download from: https://www.lunarg.com/vulkan-sdk/"
    Write-Host "Or set VK_SDK_PATH environment variable"
    Write-Host ""
}

# Build SDL3
if (-Not (Test-Path "third_party\SDL3")) {
    Write-Host "Cloning SDL3..."
    git clone --depth 1 https://github.com/libsdl-org/SDL.git third_party\SDL3
} else {
    Write-Host "SDL3 already cloned"
}

if (-Not (Test-Path "third_party\SDL3\build")) {
    Write-Host "Building SDL3..."
    cd third_party\SDL3
    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022" -A x64
    cmake --build . --config Debug
    cd ..\..\..
} else {
    Write-Host "SDL3 already built"
}

# Configure project
Write-Host ""
Write-Host "Configuring project..."
cd $PSScriptRoot
cmake -S . -B build -G "Visual Studio 17 2022" -A x64

Write-Host "Clean:              Remove-Item -Recurse -Force build"