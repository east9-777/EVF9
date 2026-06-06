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
# build_termux.sh — Android / Termux build script
# Automatically installs required packages.

set -e

echo "[EVF9] Build script — Android/Termux"
echo "======================================"

# Install dependencies in Termux
echo "[EVF9] Installing dependencies via pkg..."
pkg install -y cmake clang make git 2>/dev/null || {
    echo "[WARN] pkg install had errors — continuing anyway"
}

# Check SDK
if [ ! -d "sdk/omp/include" ]; then
    echo "[EVF9] Open.MP SDK not found — cloning..."
    git clone --depth=1 https://github.com/openmultiplayer/open.mp-sdk sdk/omp
fi

mkdir -p build
cd build

echo "[EVF9] Configuring CMake for Termux (aarch64)..."
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_COMPILER=clang++

echo "[EVF9] Building..."
cmake --build . -- -j4

if [ -f "evf9.so" ]; then
    echo ""
    echo "[EVF9] Termux build successful!"
    echo "  Output: build/evf9.so"
    echo "  Note: This binary targets your device architecture."
    echo "  For server deployment, use build.sh on Linux (x86_64)."
else
    echo "[ERROR] Build failed"
    exit 1
fi
