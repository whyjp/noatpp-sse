#!/bin/bash

# Create third_party directory
mkdir -p third_party

# Clone oatpp
if [ ! -d "third_party/oatpp" ]; then
    echo "Cloning oatpp..."
    git clone --depth 1 --branch 1.3.0 https://github.com/oatpp/oatpp.git third_party/oatpp
fi

# Clone oatpp-websocket
if [ ! -d "third_party/oatpp-websocket" ]; then
    echo "Cloning oatpp-websocket..."
    git clone --depth 1 --branch 1.3.0 https://github.com/oatpp/oatpp-websocket.git third_party/oatpp-websocket
fi

# Clone nlohmann/json
if [ ! -d "third_party/json" ]; then
    echo "Cloning nlohmann/json..."
    git clone --depth 1 --branch v3.11.3 https://github.com/nlohmann/json.git third_party/json
fi

# Clone fmt
if [ ! -d "third_party/fmt" ]; then
    echo "Cloning fmt..."
    git clone --depth 1 --branch 10.2.1 https://github.com/fmtlib/fmt.git third_party/fmt
fi

# Clone spdlog
if [ ! -d "third_party/spdlog" ]; then
    echo "Cloning spdlog..."
    git clone --depth 1 --branch v1.12.0 https://github.com/gabime/spdlog.git third_party/spdlog
fi

echo "All dependencies downloaded successfully!"