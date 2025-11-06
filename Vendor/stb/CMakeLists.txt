
cmake_minimum_required(VERSION 3.20...4.1)
project(stb LANGUAGES C)

add_library(stb STATIC
    StbImpl.cpp
)

# Include directory for stb headers
target_include_directories(stb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Suppress warnings from vendor code
if(MSVC)
    target_compile_options(stb PRIVATE /W0)
else()
    target_compile_options(stb PRIVATE -w)
endif()
