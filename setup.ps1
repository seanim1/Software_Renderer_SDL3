$ErrorActionPreference = "Stop"

if (-Not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Host "Error: CMake not found"
    exit 1
}

if (-Not (Test-Path "C:\VulkanSDK")) {
    $vulkan_url = "https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe"
    $vulkan_installer = "$env:TEMP\vulkan-sdk.exe"
    Invoke-WebRequest -Uri $vulkan_url -OutFile $vulkan_installer
    & $vulkan_installer /S
    Remove-Item $vulkan_installer
}

if (-Not (Test-Path "third_party\SDL3")) {
    git clone --depth 1 https://github.com/libsdl-org/SDL.git third_party\SDL3
}

if (-Not (Test-Path "third_party\SDL3\build")) {
    cd third_party\SDL3
    mkdir build
    cd build
    cmake .. -G "Visual Studio 17 2022" -A x64
    cmake --build . --config Debug
    cd ..\..\..
}

cmake -S . -B build -G "Visual Studio 17 2022" -A x64