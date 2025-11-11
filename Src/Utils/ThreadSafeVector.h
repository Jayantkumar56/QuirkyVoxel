

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "NonCopyable.h"
#include "NonMovable.h"

#include <vector>
#include <mutex>
#include <optional>


namespace Mct {

    template<typename T>
    class ThreadSafeVector : public NonCopyable, public NonMovable {
    public:
        ThreadSafeVector() = default;

        template<typename Item_T>
        requires std::is_same_v<std::decay_t<Item_T>, T>
        void Push(Item_T&& item) {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Vector.emplace_back(std::forward<Item_T>(item));
        }

        std::vector<T> Drain() {
            std::vector<T> oldVector;

            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                std::swap(m_Vector, oldVector);
            }

            return oldVector;
        }

        size_t Size() {
            std::lock_guard<std::mutex> lock(m_Mutex);
            return m_Vector.size();
        }

    private:
        std::vector<T> m_Vector;
        std::mutex     m_Mutex;
    };

}
