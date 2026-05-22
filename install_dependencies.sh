#!/usr/bin/env bash

set -euo pipefail

echo "========================================"
echo " Installing Full Dev Stack"
echo " Qt6 + Vulkan + SDL2 + gRPC + shaders"
echo "========================================"

sudo apt-get update

PACKAGES=(
    # =========================
    # Core build tools
    # =========================
    build-essential
    cmake
    ninja-build
    pkg-config
    git

    # =========================
    # Qt6
    # =========================
    qt6-base-dev
    qt6-base-dev-tools
    qt6-tools-dev
    qt6-tools-dev-tools
    qt6-declarative-dev
    qt6-multimedia-dev
    qt6-svg-dev
    qt6-3d-dev

    # =========================
    # SDL2
    # =========================
    libsdl2-dev

    # =========================
    # Vulkan
    # =========================
    vulkan-tools
    libvulkan-dev
    vulkan-utility-libraries-dev
    glslang-tools
    spirv-tools

    # =========================
    # gRPC + Protobuf
    # =========================
    libgrpc++-dev
    libgrpc-dev
    protobuf-compiler
    libprotobuf-dev
	protobuf-compiler-grpc

    # =========================
    # OpenGL
    # =========================
    libgl1-mesa-dev
    libglu1-mesa-dev

    # =========================
    # X11 / Linux graphics
    # =========================
    libx11-dev
    libxext-dev
    libxfixes-dev
    libxi-dev
    libxrender-dev
    libxrandr-dev
    libxcb1-dev
    libxkbcommon-dev
    libxkbcommon-x11-dev

    # =========================
    # Wayland
    # =========================
    libwayland-dev

    # =========================
    # Fonts (Qt UI)
    # =========================
    libfontconfig1-dev
    libfreetype-dev
)

echo "Installing packages..."
sudo apt-get install -y "${PACKAGES[@]}"

echo
echo "========================================"
echo " Verifying key tools"
echo "========================================"

command -v glslc >/dev/null && echo "glslc: OK" || echo "glslc: MISSING"
command -v glslangValidator >/dev/null && echo "glslangValidator: OK" || echo "glslangValidator: MISSING"

echo
echo "gRPC check:"
ldconfig -p | grep grpc >/dev/null && echo "gRPC libs: OK" || echo "gRPC libs: CHECK"

echo
echo "========================================"
echo " Done"
echo "========================================"

