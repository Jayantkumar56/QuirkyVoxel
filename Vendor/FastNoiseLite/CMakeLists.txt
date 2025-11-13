
cmake_minimum_required(VERSION 3.20...4.0)

set(CURRENT_PROJ_NAME "FastNoiseLite")

project(${CURRENT_PROJ_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_library(${CURRENT_PROJ_NAME} INTERFACE include/FastNoiseLite/FastNoiseLite.h)

target_include_directories(${CURRENT_PROJ_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
