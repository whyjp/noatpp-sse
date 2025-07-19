# Build Instructions

## System Requirements

### Windows (WSL/Native)
```bash
# Install build tools in WSL
sudo apt update
sudo apt install -y cmake build-essential git

# Or use Visual Studio 2022 on native Windows
```

### Linux/Ubuntu
```bash
sudo apt update
sudo apt install -y cmake build-essential git
```

### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install cmake via Homebrew
brew install cmake
```

## Build Steps

### 1. Download Dependencies
```bash
# Linux/macOS
chmod +x setup_dependencies.sh
./setup_dependencies.sh

# Windows
setup_dependencies.bat
```

### 2. Build Project

#### Linux/macOS
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

#### Windows (Visual Studio)
```batch
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### 3. Run Server
```bash
# Linux/macOS
./noatpp-sse

# Windows
Release\noatpp-sse.exe
```

## Test Endpoints

Once server is running on port 8080:

```bash
# Test root endpoint
curl http://localhost:8080/

# Test health endpoint
curl http://localhost:8080/api/health
```

Expected responses:
- `/` returns: "Oatpp SSE Server is running!"
- `/api/health` returns JSON with status, message, and timestamp

## Troubleshooting

1. **CMake not found**: Install cmake package
2. **Compiler errors**: Ensure C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
3. **Network issues**: Check if port 8080 is available
4. **Permission denied**: Run with appropriate privileges or change port in main.cpp