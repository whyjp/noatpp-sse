#!/bin/bash

BUILD_DIR="build-linux-static"

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

echo "Creating static build directory: $BUILD_DIR"
mkdir -p $BUILD_DIR
cd $BUILD_DIR

echo "Running CMake for fully static build..."
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_EXE_LINKER_FLAGS="-static-libgcc -static-libstdc++ -static" \
         -DCMAKE_FIND_LIBRARY_SUFFIXES=".a"

echo "Building project..."
cmake --build . --config Release -j$(nproc)

if [ $? -eq 0 ]; then
    echo "Build completed!"
    echo "Executable location: $BUILD_DIR/noatpp-sse"
    echo "Checking dependencies:"
    ldd $BUILD_DIR/noatpp-sse || echo "Fully static binary - no dynamic dependencies!"
    echo "Size: $(ls -lh $BUILD_DIR/noatpp-sse | awk '{print $5}')"
else
    echo "Build failed!"
    exit 1
fi