@echo off
setlocal enabledelayedexpansion

set BUILD_DIR=build-windows
set CLEAN=false

REM Parse command line arguments
if "%1"=="clean" set CLEAN=true

echo Checking build environment...

REM Check if CMake is available
cmake --version >nul 2>&1
if %errorlevel% equ 0 (
    echo CMake found in PATH
    set CMAKE_CMD=cmake
    goto :check_compiler
)

echo CMake not found in PATH, checking Visual Studio installation...

REM Try to find CMake in Visual Studio installation
set VS_PATHS[0]="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
set VS_PATHS[1]="C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
set VS_PATHS[2]="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

for %%i in (0,1,2) do (
    if exist !VS_PATHS[%%i]! (
        echo Found CMake in Visual Studio: !VS_PATHS[%%i]!
        set CMAKE_CMD=!VS_PATHS[%%i]!
        goto :check_compiler
    )
)

echo ERROR: CMake is not installed or not in PATH
echo Please install CMake from: https://cmake.org/download/
echo Or add CMake to your system PATH
echo.
echo If you have Visual Studio installed, you can also use:
echo "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
pause
exit /b 1

:check_compiler
REM Check if Visual Studio is available
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo WARNING: Visual Studio compiler not found in PATH
    echo This might cause build issues. Please run from Visual Studio Developer Command Prompt
    echo or add Visual Studio to your PATH
    echo.
)

echo Setting up dependencies...
call setup_dependencies.bat
if %errorlevel% neq 0 (
    echo ERROR: Failed to setup dependencies
    pause
    exit /b 1
)

REM Clean if requested
if "%CLEAN%"=="true" (
    echo Cleaning build directory...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
)

echo Creating build directory: %BUILD_DIR%
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

echo Running CMake...
%CMAKE_CMD% .. -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A x64
if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed
    pause
    exit /b 1
)

echo Building project...
%CMAKE_CMD% --build . --config Release --parallel
if %errorlevel% neq 0 (
    echo ERROR: Build failed
    pause
    exit /b 1
)

echo Build completed successfully!
echo Executable location: %BUILD_DIR%\Release\noatpp-sse.exe
echo Run: %BUILD_DIR%\Release\noatpp-sse.exe to start the server
pause