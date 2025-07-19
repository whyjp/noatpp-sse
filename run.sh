#!/bin/bash

BUILD_DIR="build-linux"
EXECUTABLE="$BUILD_DIR/noatpp-sse"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable not found at $EXECUTABLE"
    echo "Please run './build.sh' first"
    exit 1
fi

echo "Starting server from $EXECUTABLE..."
echo "Visit: http://localhost:8080/ or http://localhost:8080/api/health"
echo "Press Ctrl+C to stop"
echo ""

$EXECUTABLE