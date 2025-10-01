cmake_minimum_required(VERSION 3.16)
project(GeocoderDemo)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Find or fetch dependencies
find_package(PkgConfig QUIET)

# Try to find nlohmann/json (primary config format)
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    include(FetchContent)
    message(STATUS "nlohmann/json not found, fetching from GitHub...")
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

# Add executable
add_executable(geocoder_demo src/geocoder_demo.cpp src/GeoConverter.cpp src/ShapeFileHandler.cpp)

# Include current directory for json.hpp
target_include_directories(geocoder_demo     
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/nlohmann_json-src/include>
)

# Find GDAL
find_package(GDAL REQUIRED)

# Link GDAL to the executable
target_link_libraries(geocoder_demo PRIVATE GDAL::GDAL)

# Link libcurl
find_package(CURL REQUIRED)
target_link_libraries(geocoder_demo
    PUBLIC
        CURL::libcurl
        nlohmann_json::nlohmann_json
)
