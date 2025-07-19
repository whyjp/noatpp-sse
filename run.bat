@echo off

set BUILD_DIR=build-windows
set EXECUTABLE=%BUILD_DIR%\Release\noatpp-sse.exe

if not exist "%EXECUTABLE%" (
    echo Executable not found at %EXECUTABLE%
    echo Please run 'build.bat' first
    pause
    exit /b 1
)

echo Starting server from %EXECUTABLE%...
echo Visit: http://localhost:8080/ or http://localhost:8080/api/health
echo Press Ctrl+C to stop
echo.

"%EXECUTABLE%"
pause