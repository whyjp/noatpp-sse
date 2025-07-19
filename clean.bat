@echo off

echo Cleaning all build directories...

REM Remove build directories
if exist "build-linux" rmdir /s /q "build-linux"
if exist "build-windows" rmdir /s /q "build-windows" 
if exist "build" rmdir /s /q "build"

echo Build directories cleaned!
echo Run 'build.bat' or './build.sh' to rebuild
pause