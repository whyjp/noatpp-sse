cmake_minimum_required(VERSION 3.20)

project(noatpp-sse)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static build configuration
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Force all libraries to use static runtime (MT)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Set compiler flags for static linking
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
endif()

# Add third-party libraries as subdirectories
# Configure oatpp to use static runtime
set(OATPP_BUILD_SHARED OFF CACHE INTERNAL "")
set(OATPP_BUILD_TESTS OFF CACHE INTERNAL "")
set(OATPP_COMPAT_BUILD_NO_THREAD_LOCAL OFF CACHE INTERNAL "")
add_subdirectory(third_party/oatpp)

# Skip websocket for now - will add later
# add_subdirectory(third_party/oatpp-websocket)

# RapidJSON is header-only, just include it
# No need to add as subdirectory, just include the headers

# fmt library
set(FMT_INSTALL OFF CACHE INTERNAL "")
set(FMT_USE_WINDOWS_H OFF CACHE INTERNAL "")
add_subdirectory(third_party/fmt)

# spdlog library
set(SPDLOG_BUILD_SHARED OFF CACHE INTERNAL "")
set(SPDLOG_FMT_EXTERNAL ON CACHE INTERNAL "")
set(SPDLOG_ENABLE_PCH OFF CACHE INTERNAL "")
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
    src/domain/models/HealthStatus.hpp
    src/infrastructure/json/Jsonable.hpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries (static)
target_link_libraries(${PROJECT_NAME} PRIVATE
    oatpp
    # oatpp-websocket  # Skip for now
    fmt::fmt
    spdlog::spdlog
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    src
    third_party/rapidjson/include
)

# Windows specific libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 wsock32)
endif()

# Compiler specific options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
    set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()