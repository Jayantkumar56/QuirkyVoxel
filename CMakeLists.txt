
# SPDX-License-Identifier: MIT
# Copyright (c) 2025 Jayantkumar56


# =============================================================================
# Project Setup
# =============================================================================

cmake_minimum_required(VERSION 3.20...4.1)
project("Minecraft-Clone" LANGUAGES CXX)

include( "BuildFiles/BuildSettings.cmake"   )
include( "BuildFiles/CompilerOptions.cmake" )
include( "BuildFiles/SrcFiles.cmake"        )


# =============================================================================
# Project Sources & Executable
# =============================================================================

set(CURRENT_PROJ_NAME "Minecraft-Clone")

# Create the main executable for our game
add_executable(${CURRENT_PROJ_NAME} ${SOURCE_FILES})

enable_release_optimizations_for(${CURRENT_PROJ_NAME})
add_compiler_flags_for(${CURRENT_PROJ_NAME})

# Set startup project in visual studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CURRENT_PROJ_NAME})


# =============================================================================
# Includes and Linking
# ============================================================================

include( "BuildFiles/ExternalLibraries.cmake")
