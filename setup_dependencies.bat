@echo off

REM Create third_party directory
if not exist "third_party" mkdir third_party

REM Clone oatpp
if not exist "third_party\oatpp" (
    echo Cloning oatpp...
    git clone --depth 1 --branch 1.3.0 https://github.com/oatpp/oatpp.git third_party\oatpp
)

REM Clone oatpp-websocket
if not exist "third_party\oatpp-websocket" (
    echo Cloning oatpp-websocket...
    git clone --depth 1 --branch 1.3.0 https://github.com/oatpp/oatpp-websocket.git third_party\oatpp-websocket
)

REM Clone nlohmann/json
if not exist "third_party\json" (
    echo Cloning nlohmann/json...
    git clone --depth 1 --branch v3.11.3 https://github.com/nlohmann/json.git third_party\json
)

REM Clone fmt
if not exist "third_party\fmt" (
    echo Cloning fmt...
    git clone --depth 1 --branch 10.2.1 https://github.com/fmtlib/fmt.git third_party\fmt
)

REM Clone spdlog
if not exist "third_party\spdlog" (
    echo Cloning spdlog...
    git clone --depth 1 --branch v1.12.0 https://github.com/gabime/spdlog.git third_party\spdlog
)

echo All dependencies downloaded successfully!
pause