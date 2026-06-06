#!/bin/bash
# EVF9 - Extended Vehicle Framework 9
#
# Created by EAST9
# Open.MP / SA-MP Community
#
# GitHub: https://github.com/east9-777
# Discord: leivison0230_58048
# Comunidade Discord: https://discord.gg/FJBWAesQV8
#
# Copyright (c) EAST9
# Do not remove this notice.
#
# build.sh — Linux / Ubuntu / Debian build script

set -e

echo "[EVF9] Build script — Linux/Debian"
echo "====================================="

# Check dependencies
for dep in cmake clang++ make git; do
    if ! command -v "$dep" &>/dev/null; then
        echo "[ERROR] Missing dependency: $dep"
        echo "Install: apt install cmake clang make git"
        exit 1
    fi
done

# Check SDK
if [ ! -d "sdk/omp/include" ]; then
    echo "[EVF9] Open.MP SDK not found — fetching via git submodule..."
    git submodule update --init --recursive
    if [ ! -d "sdk/omp/include" ]; then
        echo "[ERROR] SDK fetch failed. Run manually:"
        echo "  git clone https://github.com/openmultiplayer/open.mp-sdk sdk/omp"
        exit 1
    fi
fi

# Parse args
BUILD_TYPE="Release"
BUILD_SAMP="OFF"

for arg in "$@"; do
    case "$arg" in
        --debug)   BUILD_TYPE="Debug" ;;
        --samp)    BUILD_SAMP="ON" ;;
        --help)
            echo "Usage: ./build.sh [--debug] [--samp]"
            echo "  --debug  Build with debug logging enabled"
            echo "  --samp   Build for SA-MP 32-bit (requires 32-bit toolchain)"
            exit 0 ;;
    esac
done

mkdir -p build
cd build

echo "[EVF9] Configuring CMake (type=$BUILD_TYPE, samp=$BUILD_SAMP)..."
cmake .. \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DEVF9_BUILD_SAMP="$BUILD_SAMP" \
    -DCMAKE_CXX_COMPILER=clang++

echo "[EVF9] Building..."
cmake --build . --config "$BUILD_TYPE" -- -j"$(nproc)"

if [ -f "evf9.so" ]; then
    echo ""
    echo "[EVF9] Build successful!"
    echo "  Output: build/evf9.so"
    echo ""
    echo "Install:"
    echo "  1. Copy build/evf9.so          → <server>/plugins/"
    echo "  2. Copy pawno/include/EVF9.inc → <server>/qawno/include/"
    echo "  3. Copy pawno/include/EVF9/    → <server>/qawno/include/EVF9/"
    echo "  4. Add \"evf9\" to config.json plugins section"
else
    echo "[ERROR] Build failed — evf9.so not found"
    exit 1
fi
