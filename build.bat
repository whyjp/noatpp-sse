@echo off

set BUILD_DIR=build-windows
set CLEAN=false

REM Parse command line arguments
if "%1"=="clean" set CLEAN=true

echo Setting up dependencies...
call setup_dependencies.bat

REM Clean if requested
if "%CLEAN%"=="true" (
    echo Cleaning build directory...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
)

echo Creating build directory: %BUILD_DIR%
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

echo Running CMake...
cmake .. -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A x64

echo Building project...
cmake --build . --config Release --parallel

echo Build completed!
echo Executable location: %BUILD_DIR%\Release\noatpp-sse.exe
echo Run: %BUILD_DIR%\Release\noatpp-sse.exe to start the server
pause