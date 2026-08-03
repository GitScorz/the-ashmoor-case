$ErrorActionPreference = "Stop"

$RootDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$VcpkgDir = Join-Path $RootDir "vendor\vcpkg"
$BuildDir = Join-Path $RootDir "build"
$ToolchainFile = Join-Path $VcpkgDir "scripts\buildsystems\vcpkg.cmake"

Write-Host "Initializing vcpkg..."
git -C $RootDir submodule update --init --recursive

Write-Host "Bootstrapping vcpkg..."
& (Join-Path $VcpkgDir "bootstrap-vcpkg.bat") -disableMetrics

Write-Host "Configuring Cineris..."
cmake `
    -S $RootDir `
    -B $BuildDir `
    "-DCMAKE_TOOLCHAIN_FILE=$ToolchainFile"

Write-Host "Building Cineris..."
cmake `
    --build $BuildDir `
    --config Debug `
    --parallel