
# SPDX-License-Identifier: MIT
# Copyright (c) 2025 Jayantkumar56


function(RegisterSourceFiles FILE_LIST_VAR PREFIX)
    if(DEFINED ${FILE_LIST_VAR})
        set(RESULT_LIST "${${FILE_LIST_VAR}}")
    else()
        set(RESULT_LIST "")
    endif()

    foreach(FILE_PATH ${ARGN})
        list(APPEND RESULT_LIST "${PREFIX}/${FILE_PATH}")
    endforeach()

    set(${FILE_LIST_VAR} "${RESULT_LIST}" PARENT_SCOPE)
endfunction(RegisterSourceFiles)


set(BASE_SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Src")

# Source files in Application
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Application"
    "Application.h"
    "Application.cpp"
    "GameLayer.h"
    "GameLayer.cpp"
    "Layer.h"
    "main.cpp"
    "Window.h"
    "Window.cpp"
)

# Source files in Common
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Common"
    "Assert.h"
)

# Source files in Player
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Player"
    "Player.h"
)

# Source files in Utils
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Utils"
    "Math.h"
)

# Source files in World/Block
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/World/Block"
    "Block.h"
    "BlockType.h"
)

# Source files in World/Chunk
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/World/Chunk"
    "BlockStorage.h"
    "Chunk.h"
    "ChunkCoord.h"
    "Subchunk.h"
)

# Source files in World/TerrainGeneration
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/World/TerrainGeneration"
    "TerrainGenerator.h"
    "TerrainGenerator.cpp"
    "SuperFlatTerrainGen.h"
    "SuperFlatTerrainGen.cpp"
)

# Source files in World
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/World"
    "World.h"
    "World.cpp"
    "WorldConstants.h"
    "WorldSettings.h"
)


source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCE_FILES})
