
#!/usr/bin/env bash

set -euo pipefail

echo "========================================"
echo " Installing Full Dev Stack"
echo " Qt6 + Vulkan + SDL2 + gRPC + shaders"
echo "========================================"

OS="$(uname -s)"

install_linux() {
    if ! command -v apt-get >/dev/null 2>&1; then
        echo "ERROR: This Linux installer requires apt-get."
        exit 1
    fi

    sudo apt-get update

    PACKAGES=(
        # Core build tools
        build-essential
        cmake
        ninja-build
        pkg-config
        git

        # Qt6
        qt6-base-dev
        qt6-base-dev-tools
        qt6-tools-dev
        qt6-tools-dev-tools
        qt6-declarative-dev
        qt6-multimedia-dev
        qt6-svg-dev
        qt6-3d-dev

        # SDL2
        libsdl2-dev

        # Vulkan and shaders
        vulkan-tools
        libvulkan-dev
        vulkan-utility-libraries-dev
        glslang-tools
        spirv-tools

        # gRPC and Protobuf
        libgrpc++-dev
        libgrpc-dev
        protobuf-compiler
        libprotobuf-dev
        protobuf-compiler-grpc

        # OpenGL
        libgl1-mesa-dev
        libglu1-mesa-dev

        # X11
        libx11-dev
        libxext-dev
        libxfixes-dev
        libxi-dev
        libxrender-dev
        libxrandr-dev
        libxcb1-dev
        libxkbcommon-dev
        libxkbcommon-x11-dev

        # Wayland
        libwayland-dev

        # Fonts
        libfontconfig1-dev
        libfreetype-dev
    )

    echo "Installing Linux packages..."
    sudo apt-get install -y "${PACKAGES[@]}"
}

install_macos() {
    if ! command -v brew >/dev/null 2>&1; then
        echo "ERROR: Homebrew is required on macOS."
        echo "Install it from https://brew.sh/"
        exit 1
    fi

    PACKAGES=(
        cmake
        ninja
        pkgconf
        git

        # Qt6
        qt

        # SDL2
        sdl2

        # Vulkan: headers, loader and macOS portability
        vulkan-headers
        vulkan-loader
        vulkan-tools
        molten-vk

        # Shader compilation and SPIR-V
        glslang
        spirv-tools
        shaderc

        # gRPC and Protobuf
        grpc
        protobuf

        # Supporting libraries
        freetype
        fontconfig
        libxkbcommon
    )

    echo "Updating Homebrew..."
    brew update

    echo "Installing macOS packages..."
    brew install "${PACKAGES[@]}"

    echo
    echo "Note: Qt 3D availability depends on the Qt/Homebrew"
    echo "version. Check it separately if your project needs Qt3D."
}

case "$OS" in
    Linux)
        install_linux
        ;;
    Darwin)
        install_macos
        ;;
    *)
        echo "ERROR: Unsupported operating system: $OS"
        exit 1
        ;;
esac

echo
echo "========================================"
echo " Verifying key tools"
echo "========================================"

check_command() {
    if command -v "$1" >/dev/null 2>&1; then
        printf '%s: OK (%s)\n' "$1" "$(command -v "$1")"
    else
        printf '%s: MISSING\n' "$1"
    fi
}

check_command cmake
check_command ninja
check_command pkg-config
check_command qmake6
check_command glslc
check_command glslangValidator
check_command spirv-as
check_command protoc
check_command grpc_cpp_plugin

echo
echo "gRPC / Protobuf checks:"

if pkg-config --exists grpc++; then
    echo "gRPC C++: OK"
else
    echo "gRPC C++: CHECK (pkg-config metadata unavailable)"
fi

if pkg-config --exists protobuf; then
    echo "Protobuf: OK"
else
    echo "Protobuf: CHECK (pkg-config metadata unavailable)"
fi

if command -v qmake6 >/dev/null 2>&1; then
    qmake6 -query QT_VERSION
elif command -v qmake >/dev/null 2>&1; then
    qmake -query QT_VERSION
elif [[ "$OS" == "Darwin" ]] && command -v brew >/dev/null 2>&1; then
    echo "Qt installed through Homebrew; checking its prefix:"
    brew --prefix qt
fi

echo
echo "========================================"
echo " Done"
echo "========================================"
