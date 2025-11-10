
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
    "ImguiContext.h"
    "ImguiContext.cpp"
    "Layer.h"
    "main.cpp"
    "Window.h"
    "Window.cpp"
)

# Source files in Common
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Common"
    "Assert.h"
    "BuddyAllocator.h"
    "BuddyAllocator.cpp"
    "CubeData.h"
    "IdGenerator.h"
    "NonCopyable.h"
)

# Source files in Events
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Events"
    "ApplicationEvents.h"
    "Event.h"
    "EventDispatcher.h"
    "KeyboardEvents.h"
    "MouseEvents.h"
)

# Source files in Player
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Player"
    "CameraController.h"
    "CameraController.cpp"
    "Player.h"
    "Player.cpp"
)

# Source files in Renderer/Camera
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Renderer/Camera"
    "Camera.h"
    "PerspectiveCamera.h"
)

# Source files in Renderer/Mesh
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Renderer/Mesh"
    "GpuMesh.h"
    "Mesh.h"
)

# Source files in Renderer/Primitives
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Renderer/Primitives"
    "BufferHandle.h"
    "BufferLayout.h"
    "FrameBuffer.h"
    "FrameBuffer.cpp"
    "GpuBuffer.h"
    "GpuBuffer.cpp"
    "IndexBuffer.h"
    "IndexBuffer.cpp"
    "IndirectBuffer.h"
    "Shader.h"
    "Shader.cpp"
    "ShaderStorageBuffer.h"
    "ShaderStorageBuffer.cpp"
    "TextureArray.h"
    "TextureArray.cpp"
    "Types.h"
    "VertexArray.h"
    "VertexArray.cpp"
    "VertexBuffer.h"
    "VertexBuffer.cpp"
)

# Source files in Renderer
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Renderer"
    "GameRenderer.h"
    "GameRenderer.cpp"
    "MeshManager.h"
    "MeshManager.cpp"
    "WorldRenderer.h"
    "WorldRenderer.cpp"
)

# Source files in Utils
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Utils"
    "FileUtils.h"
    "Image.h"
    "Image.cpp"
    "Math.h"
)

# Source files in World/Block
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/World/Block"
    "Block.h"
    "BlockDataManager.h"
    "BlockDataManager.cpp"
    "BlockType.h"
)

# Source files in World/Chunk
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/World/Chunk"
    "BlockStorage.h"
    "Chunk.h"
    "ChunkCoord.h"
    "ChunkGpuMesh.h"
    "ChunkMesh.h"
    "ChunkMeshGenerator.h"
    "ChunkMeshGenerator.cpp"
    "ChunkNeighbor.h"
    "ChunkSpan.h"
    "PackedTerrainMesh.h"
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
    "ChunkManager.h"
    "ChunkManager.cpp"
    "World.h"
    "World.cpp"
    "WorldConstants.h"
    "WorldSettings.h"
)


source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCE_FILES})
