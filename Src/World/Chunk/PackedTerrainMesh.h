

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Renderer/Mesh/Mesh.h"
#include "Utils/CubeData.h"

#include <glm/glm.hpp> 

#include <vector>


namespace Mct {

    // TODO: Deal with light data.
    struct TerrainVertex {
        uint32_t Data1;
        uint32_t Data2;

        // Data1:
        // 5  bits - X (0-16)
        // 5  bits - Y (0-16)
        // 5  bits - Z (0-16)
        // 4  bits - Position Y Offset (stores 0-15, for 1/16th precision)

        // Data2:
        // 3  bits - Normal ID (0-5, for the 6 cube faces)
        // 5  bits - Packed U (0-16, representing 0.0 to 1.0)
        // 5  bits - Packed V (0-16, representing 0.0 to 1.0)
        // 10 bits - Texture ID bits (supports 1024 unique face textures)

        static constexpr TerrainVertex Create(uint32_t   x,
                                              uint32_t   y,
                                              uint32_t   z,                          // Position (0-16)
                                              uint32_t   yOffset,                    // Y-Offset (0-15)
                                              uint32_t   u,
                                              uint32_t   v,                          // UV (0-16)
                                              uint32_t   texID,
                                              CubeNormal normID) noexcept
        {
            uint32_t data1 = (x & 0x1F)                 |
                             ((y & 0x1F) << 5)          |
                             ((z & 0x1F) << 10)         |
                             ((yOffset & 0xF) << 15);

            uint32_t data2 = (static_cast<uint32_t>(normID) & 0x7)      |
                             ((u & 0x1F) << 3)                          |
                             ((v & 0x1F) << 8)                          |
                             ((texID & 0x3FF) << 13);

            return TerrainVertex{ data1, data2 };
        }
    };

    // CPU-side mesh for a single chunk's geometry pass (e.g. Solid or Water).
    //
    class PackedTerrainMesh : public Mesh {
    public:
        PackedTerrainMesh() :
                Mesh(CreateUploadDescriptor())
        {
            m_UploadDescriptor.Primitive = RenderPrimitive::Triangles;
            m_UploadDescriptor.IndexType = IndexType::UInt32;
        }

        constexpr const void* VertexData() const noexcept override { return m_Vertices.data(); }
        constexpr const void* IndexData()  const noexcept override { return m_Indices.data();  }

        constexpr void AddFace(const TerrainVertex& v1,
                               const TerrainVertex& v2,
                               const TerrainVertex& v3,
                               const TerrainVertex& v4)
        {
            // The starting index for this new quad is the current
            // number of vertices.
            const uint32_t index = static_cast<uint32_t>(m_Vertices.size());

            // Add the 4 vertices
            m_Vertices.push_back(v1);
            m_Vertices.push_back(v2);
            m_Vertices.push_back(v3);
            m_Vertices.push_back(v4);

            // Add the 6 indices (2 triangles)
            m_Indices.push_back(index + 0);
            m_Indices.push_back(index + 1);
            m_Indices.push_back(index + 2);
            m_Indices.push_back(index + 2);
            m_Indices.push_back(index + 3);
            m_Indices.push_back(index + 0);

            // Update the UploadDescriptor
            m_UploadDescriptor.VertexCount = static_cast<uint32_t>(m_Vertices.size());
            m_UploadDescriptor.IndexCount  = static_cast<uint32_t>(m_Indices.size());
        }

        constexpr bool IsEmpty() const noexcept { return m_Indices.empty(); }

        static BufferLayout GetBufferLayout() {
            return BufferLayout{
                    { ShaderDataType::Int,  "i_Data1" },
                    { ShaderDataType::Int,  "i_Data2" }
            };
        }

    private:
        MeshUploadDesc CreateUploadDescriptor() const {
            return MeshUploadDesc{
                .Primitive     { RenderPrimitive::Triangles },
                .VertexCount   { 0                          },
                .VertexLayout  { GetBufferLayout()          },
                .IndexCount    { 0                          },
                .IndexType     { IndexType::UInt32          },
                .InstanceCount { 1                          }
            };
        }

    private:
        std::vector<uint32_t>      m_Indices;
        std::vector<TerrainVertex> m_Vertices;
    };

}
