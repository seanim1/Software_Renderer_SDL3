# copy-dlls.ps1 — Copy SDL3.dll to all build targets
$ErrorActionPreference = "Stop"

Write-Host "Copying SDL3.dll to build directories..."
Write-Host ""

$source_dll = "third_party\SDL3\build\Debug\SDL3.dll"

if (-Not (Test-Path $source_dll)) {
    Write-Host "Error: SDL3.dll not found at $source_dll"
    exit 1
}

$targets = @(
    "build\Part_00_init_SDL3\Debug\",
    "build\Part_01_window\Debug\",
    "build\Part_02_vk_instance\Debug\"
)

foreach ($target in $targets) {
    if (-Not (Test-Path $target)) {
        Write-Host "Creating directory: $target"
        mkdir $target -Force | Out-Null
    }
    
    Write-Host "Copying to: $target"
    Copy-Item $source_dll "$target\SDL3.dll" -Force
}

Write-Host ""
Write-Host "✓ All DLLs copied successfully!"