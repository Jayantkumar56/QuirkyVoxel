

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "ChunkMeshGenerator.h"
#include "ChunkMesh.h"
#include "ChunkNeighbor.h"
#include "World/ChunkManager.h"
#include "World/Block/BlockDataManager.h"


namespace Mct {

    static bool ShouldDrawFace(const Block current, 
                               const Block neighbor) noexcept;

    static void AddFaceIfVisible(uint32_t x, uint32_t y, uint32_t z,      // Current block coords
                                 const Block             currentBlock,
                                 const CubeNormal        face,
                                 PackedTerrainMesh&      targetMesh,
                                 const SubChunkNeighbor& neighbor);


    void ChunkMeshGenerator::GenerateChunkMeshes(ChunkMesh& outMesh, const ChunkMeshInputView& neighbor) {
        MCT_ASSERT(
            (neighbor.Main  ||
             neighbor.North ||
             neighbor.South ||
             neighbor.East  || 
             neighbor.West  ) &&
            "All the chunks should be valid (i.e. not null)."
        );

        constexpr auto getNeighborSubchunk = +[] (const ChunkSpan<const Block> chunk, 
                                                  const size_t yIdx, 
                                                  const int    offset) noexcept -> SubchunkSpan<const Block>
        {
            const int newIdx = static_cast<int>(yIdx) + offset;
            if (newIdx < 0 || newIdx >= static_cast<int>(WorldConst::SubchunkCount))
                return ChunkManager::GetEmptySubchunk().GetBlocks();

            return chunk.GetSubchunkForRead(static_cast<size_t>(newIdx));
        };

        for (size_t yIdx = 0; yIdx < WorldConst::SubchunkCount; ++yIdx) {
            SubChunkNeighbor subchunkNeighbor{
                .Main   { neighbor.Main.GetSubchunkForRead(yIdx)       },
                .Top    { getNeighborSubchunk(neighbor.Main, yIdx, +1) },
                .Bottom { getNeighborSubchunk(neighbor.Main, yIdx, -1) },
                .North  { neighbor.North.GetSubchunkForRead(yIdx)      },
                .South  { neighbor.South.GetSubchunkForRead(yIdx)      },
                .East   { neighbor.East.GetSubchunkForRead(yIdx)       },
                .West   { neighbor.West.GetSubchunkForRead(yIdx)       }
            };

            GenerateSubchunkMesh(&outMesh.SubchunkMesh[yIdx], subchunkNeighbor);
        }
	}

	void ChunkMeshGenerator::GenerateSubchunkMesh(SubchunkMesh* outMesh, const SubChunkNeighbor& neighbor) {
        MCT_ASSERT(outMesh && "Cannot Write to null mesh, provide valid non null mesh.");

        neighbor.Main.ForEachConstXYZ([&neighbor, outMesh](const uint32_t x, 
                                                           const uint32_t y, 
                                                           const uint32_t z, 
                                                           const Block currentBlock)
        {
            if (!currentBlock.HaveMesh()) {
                return;
            }

            // Decide which mesh this block belongs to
            PackedTerrainMesh& targetMesh = (currentBlock.IsWater()) ?
                                            outMesh->WaterMesh :
                                            outMesh->SolidMesh;

            AddFaceIfVisible( x, y, z, currentBlock, CubeNormal::Top,    targetMesh, neighbor );
            AddFaceIfVisible( x, y, z, currentBlock, CubeNormal::Bottom, targetMesh, neighbor );
            AddFaceIfVisible( x, y, z, currentBlock, CubeNormal::East,   targetMesh, neighbor );
            AddFaceIfVisible( x, y, z, currentBlock, CubeNormal::West,   targetMesh, neighbor );
            AddFaceIfVisible( x, y, z, currentBlock, CubeNormal::South,  targetMesh, neighbor );
            AddFaceIfVisible( x, y, z, currentBlock, CubeNormal::North,  targetMesh, neighbor );
        });
	}

    static bool ShouldDrawFace(const Block current, 
                               const Block neighbor) noexcept 
    {
        // If neighbor is air (doesn't have a mesh), always draw.
        if (!neighbor.HaveMesh()) {
            return true;
        }

        // Don't draw solid faces next to other solid faces.
        if (current.IsSolid() && neighbor.IsSolid()) {
            return false;
        }

        // Don't draw water faces next to other water faces.
        if (current.IsWater() && neighbor.IsWater()) {
            return false;
        }

        // Draw all other combinations (solid-water, water-solid, solid-transparent etc.)
        return true;
    }

    static void AddFaceIfVisible(uint32_t x, uint32_t y, uint32_t z,      // Current block coords
                                 const Block             currentBlock,
                                 const CubeNormal        face,
                                 PackedTerrainMesh&      targetMesh,
                                 const SubChunkNeighbor& neighbor)
    {
        const auto& offset      = CUBE_NEIGHBOR_OFFSETS[static_cast<size_t>(face)];
        const auto& faceIndices = CUBE_FACE_INDICES[static_cast<size_t>(face)];

        const Block neighborBlock = neighbor.GetBlock(
            static_cast<int>(x) + static_cast<int>(offset.x),
            static_cast<int>(y) + static_cast<int>(offset.y),
            static_cast<int>(z) + static_cast<int>(offset.z)
        );

        if (!ShouldDrawFace(currentBlock, neighborBlock)) {
            return; // Face is occluded
        }

        const uint32_t texID = currentBlock.GetFaceTexture(face);

        const std::array<glm::vec2, 4>& uv = currentBlock.GetUV(face);

        // TODO: Fetch correct offset from the block
        uint32_t yOffset = 0;

        // The 4 vertex corner positions relative to (0,0,0) face origin.
        const auto& vc1 = CUBE_CORNERS[faceIndices.x];          // Index for corner 1
        const auto& vc2 = CUBE_CORNERS[faceIndices.y];          // Index for corner 2
        const auto& vc3 = CUBE_CORNERS[faceIndices.z];          // Index for corner 3
        const auto& vc4 = CUBE_CORNERS[faceIndices.w];          // Index for corner 4

        TerrainVertex v1 = TerrainVertex::Create(x + vc1.x, y + vc1.y, z + vc1.z, yOffset, uv[0].x, uv[0].y, texID, face);
        TerrainVertex v2 = TerrainVertex::Create(x + vc2.x, y + vc2.y, z + vc2.z, yOffset, uv[1].x, uv[1].y, texID, face);
        TerrainVertex v3 = TerrainVertex::Create(x + vc3.x, y + vc3.y, z + vc3.z, yOffset, uv[2].x, uv[2].y, texID, face);
        TerrainVertex v4 = TerrainVertex::Create(x + vc4.x, y + vc4.y, z + vc4.z, yOffset, uv[3].x, uv[3].y, texID, face);

        targetMesh.AddFace(v1, v2, v3, v4);
    }

}
