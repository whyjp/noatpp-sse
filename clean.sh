#!/bin/bash

echo "Cleaning all build directories..."

# Remove build directories
rm -rf build-linux
rm -rf build-windows
rm -rf build  # Legacy build directory

echo "Build directories cleaned!"
echo "Run './build.sh' or 'build.bat' to rebuild"