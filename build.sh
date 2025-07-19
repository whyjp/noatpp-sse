#!/bin/bash

BUILD_DIR="build-linux"

# Parse command line arguments
CLEAN=false
if [ "$1" = "clean" ]; then
    CLEAN=true
fi

echo "Setting up dependencies..."
chmod +x setup_dependencies.sh
./setup_dependencies.sh

# Clean if requested
if [ "$CLEAN" = true ]; then
    echo "Cleaning build directory..."
    rm -rf $BUILD_DIR
fi

echo "Creating build directory: $BUILD_DIR"
mkdir -p $BUILD_DIR
cd $BUILD_DIR

echo "Running CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

echo "Building project..."
cmake --build . --config Release -j$(nproc)

echo "Build completed!"
echo "Executable location: $BUILD_DIR/noatpp-sse"
echo "Run: ./$BUILD_DIR/noatpp-sse to start the server"