

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "NonCopyable.h"

#include <vector>
#include <list>
#include <bit>
#include <optional>
#include <unordered_map>


namespace Mct {

    // Manages a large, fixed-size block of memory by dividing it into power-of-two buddy blocks.
    // 
    // NOTE:
    //      This is not a real memory allocator (like malloc).
    //      It does not own or store any memory addresses (e.g. void*).
    //      It is a purely logical class that manages offsets and sizes
    //      within a conceptual buffer (like a VBO or a large std::vector),
    //      analyzing where a new allocation should be placed.
    //      
    class BuddyAllocator : public NonCopyable {
    private:
        // Represents the state of a node in the binary tree.
        // A node can be free, used, or split into two children of half size.
        //
        struct Node {
            bool IsUsed  = false;
            bool IsSplit = false;
        };

    public:
        // totalSize The total size of the memory block to manage (MUST be a power of two).
        // minBlockSize The smallest block size that can be allocated (MUST be a power of two).
        // 
        BuddyAllocator(const size_t totalSize, const size_t minBlockSize);

        // Allocates a block of memory.
        // Returns The offset (in bytes) of the allocated block.
        std::optional<size_t> Allocate(const size_t requestedSize);

        // Frees a previously allocated block at the offset (in bytes).
        bool Free(size_t offset);

        constexpr size_t GetTotalSize()    const noexcept { return m_TotalSize;    }
        constexpr size_t GetMinBlockSize() const noexcept { return m_MinBlockSize; }

    private:
        std::optional<size_t> SplitBlock(size_t nodeIndex, int targetLevel);

        size_t MergeBlock(size_t nodeIndex);

        constexpr bool IsPowerOfTwo(const size_t n)  const noexcept { 
            return (n != 0) && ((n & (n - 1)) == 0);
        }

        constexpr size_t GetParent(const size_t index) const noexcept {
            return (index - 1) / 2;
        }

        constexpr size_t GetLeftChild(const size_t index) const noexcept {
            return index * 2 + 1;
        }

        constexpr size_t GetRightChild(const size_t index) const noexcept {
            return index * 2 + 2;
        }

        constexpr size_t GetBuddy(const size_t index) const noexcept {
            return (index & 1) ? (index + 1) : (index - 1);
        }

        constexpr int GetLevelForSize(const size_t size) const noexcept {
            return static_cast<int>(std::countr_zero(m_TotalSize) - std::countr_zero(size));
        }

        constexpr int GetLevelForNode(const size_t nodeIndex) const noexcept {
            return static_cast<int>(std::bit_width(nodeIndex + 1) - 1);
        }

        constexpr size_t GetBlockSizeForLevel(const int level) const noexcept {
            return m_TotalSize >> level;
        }

        constexpr size_t RoundUpToBlockSize(const size_t size) const noexcept {
            if (size <= m_MinBlockSize) {
                return m_MinBlockSize;
            }

            // Find smallest integral power of two that is not smaller than x. 
            return std::bit_ceil(size);
        }

        constexpr size_t GetOffsetForNode(const size_t nodeIndex) const noexcept {
            int level = GetLevelForNode(nodeIndex);
            size_t blockSize = GetBlockSizeForLevel(level);

            // Calculate the index of this node within its level.
            int levelStartIndex = (1 << level) - 1;
            int indexInLevel    = static_cast<int>(nodeIndex) - levelStartIndex;

            return static_cast<size_t>(indexInLevel) * blockSize;
        }

    private:
        size_t m_TotalSize;
        size_t m_MinBlockSize;
        int    m_NumLevels;

        // The complete binary tree of all nodes.
        std::vector<Node> m_Tree;

        // An array of free lists, one for each level.
        // m_FreeLists[0] = list of free blocks at level 0 (root, totalSize)
        // m_FreeLists[m_NumLevels-1] = list of free blocks at the leaf level
        std::vector<std::list<size_t>> m_FreeLists;

        // Fast lookup map to find a node index from its offset.
        std::unordered_map<size_t, size_t> m_OffsetToNodeIndex;
    };

}
