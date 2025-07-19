# Oatpp SSE Server

Simple oatpp-based HTTP server with WebSocket and SSE support.

## Prerequisites

- CMake 3.20+
- C++20 compatible compiler
- Git

### Windows
- Visual Studio 2022 or later
- Git for Windows

### Linux/Unix
- GCC 10+ or Clang 12+
- Make

## Quick Start

### Windows
```batch
# Clean build (optional)
build.bat clean

# Build
build.bat

# Run server
run.bat
```

### Linux/Unix
```bash
# Clean build (optional)
chmod +x *.sh
./build.sh clean

# Build
./build.sh

# Run server
./run.sh
```

## Build Directory Structure

The build system creates separate directories for each platform:
- `build-windows/` - Windows build files and executable
- `build-linux/` - Linux build files and executable

## Available Scripts

### Build Scripts
- `build.sh [clean]` - Linux build script
- `build.bat [clean]` - Windows build script

### Run Scripts  
- `run.sh` - Start Linux server
- `run.bat` - Start Windows server

### Clean Scripts
- `clean.sh` - Clean all build directories
- `clean.bat` - Clean all build directories

### Dependency Scripts
- `setup_dependencies.sh` - Download dependencies (Linux)
- `setup_dependencies.bat` - Download dependencies (Windows)

## Endpoints

- `GET /` - Root endpoint: "Oatpp SSE Server is running!"
- `GET /api/health` - Health check endpoint: JSON with status, message, timestamp

## Server Configuration

- **Port**: 8080
- **Host**: localhost
- **Endpoints**: http://localhost:8080/ and http://localhost:8080/api/health

## Project Structure

```
├── src/
│   ├── infrastructure/oatpp/     # Oatpp wrapper classes
│   ├── presentation/controllers/ # HTTP controllers
│   └── main.cpp                  # Application entry point
├── third_party/                  # External dependencies (auto-downloaded)
├── build-windows/                # Windows build output
├── build-linux/                  # Linux build output
├── CMakeLists.txt               # Build configuration
├── *.sh, *.bat                  # Build/run scripts
└── README.md                    # This file
```

## Development

1. **Clean build**: `./build.sh clean` or `build.bat clean`
2. **Build**: `./build.sh` or `build.bat`  
3. **Run**: `./run.sh` or `run.bat`
4. **Test**: `curl http://localhost:8080/api/health`