# setup.ps1 — Windows
$ErrorActionPreference = "Stop"

Write-Host "Windows setup"

# Check for SDL3
if (-Not (Test-Path "C:\SDL3")) {
    Write-Host "Building SDL3..."
    git clone https://github.com/libsdl-org/SDL.git C:\SDL3
    cd C:\SDL3
    if (-Not (Test-Path "build")) {
        mkdir build
    }
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A x64
    cmake --build . --config Release
    cmake --install .
} else {
    Write-Host "SDL3 already built"
}

# Configure project
cd $PSScriptRoot
cmake -S . -B build_windows -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A x64

Write-Host ""
Write-Host "Setup complete!"
Write-Host ""
Write-Host "Build all:        cmake --build build_windows --config Release"
Write-Host "Build a target:   cmake --build build_windows --target hello_triangle --config Release"
Write-Host "Run:              .\build_windows\Release\hello_triangle.exe"
Write-Host "Clean:            rmdir /s /q build_windows"
