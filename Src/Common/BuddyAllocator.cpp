

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Common/Assert.h"
#include "BuddyAllocator.h"

#include <bit>


namespace Mct {

	Mct::BuddyAllocator::BuddyAllocator(const size_t totalSize, const size_t minBlockSize) : 
            m_TotalSize    ( totalSize    ),
            m_MinBlockSize ( minBlockSize )
    {

        MCT_ASSERT(IsPowerOfTwo(totalSize) & IsPowerOfTwo(minBlockSize) && 
                   "Total size and min block size must be powers of two.");

        MCT_ASSERT(minBlockSize <= totalSize &&
                   "Min block size cannot be larger than total size.");

        // Calculate the number of levels
        m_NumLevels = std::countr_zero(totalSize) - std::countr_zero(minBlockSize) + 1;

        // Total nodes in a perfect binary tree is 2*N - 1, where N is num leaves
        size_t numLeaves = totalSize / minBlockSize;
        size_t numNodes  = 2 * numLeaves - 1;

        m_Tree.resize(numNodes);
        m_FreeLists.resize(m_NumLevels);

        // Add the root node (index 0) to the free list for the top level (level 0)
        m_FreeLists[0].push_back(0);
    }

    std::optional<size_t> BuddyAllocator::Allocate(const size_t requestedSize) {
        if (requestedSize == 0) return std::nullopt;

        const size_t blockSize = RoundUpToBlockSize(requestedSize);

        if (blockSize > m_TotalSize) 
            return std::nullopt;

        const int targetLevel = GetLevelForSize(blockSize);

        if (targetLevel < 0 || targetLevel >= m_NumLevels) 
            return std::nullopt;

        // Find smallest available free block that fits (search upward toward root)
        int foundLevel = -1;

        for (int level = targetLevel; level >= 0; --level) {
            if (!m_FreeLists[level].empty()) {
                foundLevel = level;
                break;
            }
        }

        if (foundLevel == -1) 
            return std::nullopt; // no free block available

        // Pop a node from the found level
        size_t nodeIndex = m_FreeLists[foundLevel].front();
        m_FreeLists[foundLevel].pop_front();

        // If the found block is bigger than required, split it down to targetLevel.
        // SplitBlock assumes the caller already removed the parent from its free-list.
        if (foundLevel < targetLevel) {
            auto newNodeOpt = SplitBlock(nodeIndex, targetLevel);
            if (!newNodeOpt) {
                // split failed — restore the original block to its free-list and fail
                m_FreeLists[foundLevel].push_front(nodeIndex);
                return std::nullopt;
            }

            nodeIndex = *newNodeOpt;
        }

        // Sanity-check and allocate the final node
        if (nodeIndex >= m_Tree.size()) {
            // defensive: unexpected, restore original if possible (best-effort)
            // If this happens the allocator is in an inconsistent state; return nullopt.
            return std::nullopt;
        }

        if (m_Tree[nodeIndex].IsUsed || m_Tree[nodeIndex].IsSplit) {
            // Block is not free (unexpected). Can't allocate it.
            // Do not attempt complex rollback here, signal failure.
            return std::nullopt;
        }

        m_Tree[nodeIndex].IsUsed = true;
        const size_t offset = GetOffsetForNode(static_cast<int>(nodeIndex));

        // Record mapping for Free()
        m_OffsetToNodeIndex[offset] = nodeIndex;

        return offset;
    }

    bool BuddyAllocator::Free(size_t offset) {
        // Lookup mapping offset -> node
        auto it = m_OffsetToNodeIndex.find(offset);
        if (it == m_OffsetToNodeIndex.end()) {
            // invalid offset / double free
            return false;
        }

        size_t nodeIndex = it->second;
        // remove mapping immediately (best-effort to avoid double-free reentrancy)
        m_OffsetToNodeIndex.erase(it);

        // Sanity: nodeIndex must be within tree and be marked used
        if (nodeIndex >= m_Tree.size()) return false;
        if (!m_Tree[nodeIndex].IsUsed) return false; // double free / corruption

        // Mark as free
        m_Tree[nodeIndex].IsUsed = false;

        // Push to its level free-list
        int level = GetLevelForNode(nodeIndex);
        if (level < 0 || level >= m_NumLevels) return false; // defensive

        m_FreeLists[level].push_back(nodeIndex);

        // Attempt to merge upward and keep the tree compact
        MergeBlock(nodeIndex);

        return true;
    }

    std::optional<size_t> BuddyAllocator::SplitBlock(size_t nodeIndex, int targetLevel) {
        // sanity checks
        if (nodeIndex >= m_Tree.size()) return std::nullopt;
        int currentLevel = GetLevelForNode(nodeIndex);
        if (!(currentLevel < targetLevel && targetLevel < m_NumLevels)) return std::nullopt;
        if (m_Tree[nodeIndex].IsUsed) return std::nullopt; // cannot split a used block

        // keep splitting down left children; push right buddy to the free-list each step
        while (currentLevel < targetLevel) {
            // mark parent as split
            m_Tree[nodeIndex].IsSplit = true;

            const size_t left  = GetLeftChild(nodeIndex);
            const size_t right = GetRightChild(nodeIndex);

            // bounds check (defensive)
            if (left >= m_Tree.size() || right >= m_Tree.size()) {
                // rollback the split mark we just set to keep structure sane
                m_Tree[nodeIndex].IsSplit = false;
                return std::nullopt;
            }

            // initialize child nodes (defensive)
            m_Tree[left].IsUsed   = false;
            m_Tree[left].IsSplit  = false;
            m_Tree[right].IsUsed  = false;
            m_Tree[right].IsSplit = false;

            // push the right buddy into the free-list for the next level
            m_FreeLists[currentLevel + 1].push_back(right);

            // continue splitting the left child (do NOT add left to free-list)
            nodeIndex = left;
            ++currentLevel;
        }

        // nodeIndex now points to a leaf at targetLevel; caller will mark IsUsed.
        return nodeIndex;
    }

    size_t BuddyAllocator::MergeBlock(size_t nodeIndex) {
        // Determine current level of nodeIndex
        int curLevel = GetLevelForNode(nodeIndex);
        // Defensive: if nodeIndex is out-of-range, return as-is
        if (nodeIndex >= m_Tree.size() || curLevel >= m_NumLevels || curLevel < 0)
            return nodeIndex;

        size_t cur = nodeIndex;

        // Attempt to merge while we are not at the root
        while (curLevel > 0) {
            const size_t buddy = GetBuddy(cur);

            // Bounds check for buddy
            if (buddy >= m_Tree.size()) break;

            // Buddy must be free (not used, not split) to allow merge
            if (m_Tree[buddy].IsUsed || m_Tree[buddy].IsSplit) break;

            // Ensure 'cur' is present in its free-list (we pushed it prior to calling MergeBlock)
            auto& fl = m_FreeLists[curLevel];
            auto itCur = std::find(fl.begin(), fl.end(), cur);
            if (itCur == fl.end()) {
                // If cur is not found, cannot safely merge; stop
                break;
            }

            // Ensure buddy is present in the free-list as well. If it's not,
            // we cannot merge because that would remove a block that may be in-use by another actor.
            auto itBuddy = std::find(fl.begin(), fl.end(), buddy);
            if (itBuddy == fl.end()) {
                // Leave cur in the free-list and stop merging.
                break;
            }

            // Remove both siblings from this level's free-list
            fl.erase(itCur);
            // Re-find buddy iterator if erase invalidated earlier iterator ordering
            itBuddy = std::find(fl.begin(), fl.end(), buddy);
            if (itBuddy != fl.end()) {
                fl.erase(itBuddy);
            }
            else {
                // If buddy erase failed for some reason (race/inconsistency), put cur back and stop.
                fl.push_back(cur);
                break;
            }

            // Mark their parent as not split and not used, and add parent to upper level free-list
            const size_t parent = GetParent(cur);
            if (parent >= m_Tree.size()) {
                // Unexpected; stop merging and attempt to keep structure sane by re-adding parent? stop.
                m_FreeLists[curLevel].push_back(cur);
                break;
            }
            m_Tree[parent].IsSplit = false;
            m_Tree[parent].IsUsed = false;

            // Move up one level
            cur = parent;
            --curLevel;

            // push parent into the next-level free-list (if we can continue merging, this will be removed)
            m_FreeLists[curLevel].push_back(cur);
        }

        return cur;
    }

}
