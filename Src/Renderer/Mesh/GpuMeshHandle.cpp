

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "GpuMeshHandle.h"
#include "Renderer/MeshManager.h"


namespace Mct {

	GpuMeshHandle::GpuMeshHandle(MeshManager*                     manager, 
                                 VertexBufferHandle               vbo, 
                                 std::optional<IndexBufferHandle> ibo) : 
            m_Manager ( manager ), 
            VboHandle ( vbo     ), 
            IboHandle ( ibo     )
    {}

    GpuMeshHandle::~GpuMeshHandle() {
        if (m_Manager) {
            m_Manager->ScheduleForDeletion(*this);
        }
    }

    GpuMeshHandle::GpuMeshHandle(GpuMeshHandle&& other) noexcept : 
            m_Manager ( other.m_Manager ),
            VboHandle ( other.VboHandle ),
            IboHandle ( other.IboHandle )
    {
        // Steal the resource by nulling out the other handle's manager.
        // This prevents its destructor from also freeing the memory.
        other.m_Manager = nullptr;
    }

    GpuMeshHandle& GpuMeshHandle::operator=(GpuMeshHandle&& other) noexcept {
        if (this != &other) {
            if (m_Manager) {
                m_Manager->ScheduleForDeletion(*this);
            }

            m_Manager = other.m_Manager;
            VboHandle = other.VboHandle;
            IboHandle = other.IboHandle;

            // Null out 'other'
            other.m_Manager = nullptr;
        }

        return *this;
    }

}
