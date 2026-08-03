#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VCPKG_DIR="$ROOT_DIR/vendor/vcpkg"
BUILD_DIR="$ROOT_DIR/build"

echo "Initializing vcpkg..."
git -C "$ROOT_DIR" submodule update --init --recursive

echo "Bootstrapping vcpkg..."
"$VCPKG_DIR/bootstrap-vcpkg.sh" -disableMetrics

echo "Configuring Cineris..."
cmake \
  -S "$ROOT_DIR" \
  -B "$BUILD_DIR" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake"

echo "Building Cineris..."
cmake --build "$BUILD_DIR" --parallel