cmake_minimum_required(VERSION 3.20)

project(noatpp-sse)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static build configuration
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add third-party libraries as subdirectories
add_subdirectory(third_party/oatpp)

# Skip websocket for now - will add later
# add_subdirectory(third_party/oatpp-websocket)

# nlohmann/json is header-only, just include it
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(third_party/json)

# fmt library
set(FMT_INSTALL OFF CACHE INTERNAL "")
add_subdirectory(third_party/fmt)

# spdlog library
set(SPDLOG_BUILD_SHARED OFF CACHE INTERNAL "")
set(SPDLOG_FMT_EXTERNAL ON CACHE INTERNAL "")
add_subdirectory(third_party/spdlog)

# Source files
set(SOURCES
    src/main.cpp
    src/presentation/controllers/HttpController.cpp
    src/infrastructure/oatpp/OatppServerWrapper.cpp
)

# Headers
set(HEADERS
    src/presentation/controllers/HttpController.hpp
    src/infrastructure/oatpp/OatppServerWrapper.hpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries (static)
target_link_libraries(${PROJECT_NAME} PRIVATE
    oatpp
    # oatpp-websocket  # Skip for now
    nlohmann_json::nlohmann_json
    fmt::fmt
    spdlog::spdlog
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Windows specific libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 wsock32)
endif()

# Compiler specific options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /MT)
    set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()