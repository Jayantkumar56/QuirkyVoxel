

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "NonCopyable.h"
#include "NonMovable.h"

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <type_traits>


namespace Mct {

    template<typename Task_T, typename Functor_T>
    requires std::invocable<Functor_T, Task_T&&> || std::invocable<Functor_T, Task_T&>
    class TaskProcessorPool : public NonCopyable, public NonMovable {
        static_assert(std::is_nothrow_move_constructible_v<Task_T>, "Task_T must be nothrow-move-constructible.");

    public:
        template<typename Processor_T>
        requires std::is_same_v<std::decay_t<Processor_T>, Functor_T>
        TaskProcessorPool(size_t numThreads, Processor_T&& processor) :
                m_Stop          ( false                                ),
                m_TaskProcessor ( std::forward<Processor_T>(processor) )
        {
            m_Workers.reserve(numThreads);

            for (size_t i = 0; i < numThreads; ++i) {
                m_Workers.emplace_back([this] { WorkerLoop(); });
            }
        }

        ~TaskProcessorPool() {
            Shutdown();

            for (std::thread &worker : m_Workers) {
                if (worker.joinable()) worker.join();
            }
        }

        template<typename T>
        requires std::is_same_v<std::decay_t<T>, Task_T>
        bool Submit(T&& task) {
            {
                std::unique_lock<std::mutex> lock(m_QueueMutex);
                if (m_Stop.load(std::memory_order_acquire)) 
                    return false; // Pool is shutting down

                m_TaskQueue.push(std::forward<T>(task));
            }

            m_Condition.notify_one();
            return true;
        }

        template<typename Vector>
        requires std::is_same_v<std::decay_t<Vector>, std::vector<Task_T>>
        bool SubmitBatch(Vector&& tasks) {
            if (tasks.empty()) {
                // Return true if not stopped.
                return !m_Stop.load(std::memory_order_acquire);
            }

            {
                std::unique_lock<std::mutex> lock(m_QueueMutex);
                if (m_Stop.load(std::memory_order_acquire))
                    return false; // Pool is shutting down

                if constexpr (std::is_rvalue_reference_v<Vector&&>) {
                    // Vector is an rvalue, so we can safely MOVE elements from it
                    for (auto& task : tasks) {
                        m_TaskQueue.push(std::move(task));
                    }
                }
                else {
                    // Vector is an lvalue, so we must COPY elements
                    for (const auto& task : tasks) {
                        m_TaskQueue.push(task);
                    }
                }
            }

            m_Condition.notify_all();
            return true;
        }

        // Initiate shutdown. After this, Submit will return false.
        void Shutdown() noexcept {
            bool expected = false;

            if (m_Stop.compare_exchange_strong(expected, true)) {
                m_Condition.notify_all();
            }
        }

    private:
        void WorkerLoop() {
            while (true) {
                std::unique_lock<std::mutex> lock(m_QueueMutex);
                m_Condition.wait(lock, [this] {
                    return m_Stop.load(std::memory_order_acquire) || !m_TaskQueue.empty();
                });

                if (m_Stop.load(std::memory_order_acquire) && m_TaskQueue.empty()) {
                    return;
                }

                Task_T task = std::move(m_TaskQueue.front());
                m_TaskQueue.pop();

                lock.unlock();

                // Execute task and guard against exceptions so threads don't call std::terminate.
                try {
                    if constexpr (std::invocable<Functor_T, Task_T&&>)
                        m_TaskProcessor(std::move(task));
                    else
                        m_TaskProcessor(task);
                } 
                catch (...) {
                    // Swallow or log, do not rethrow, otherwise std::terminate in thread.
                    // TODO: provide user callback/logging hook for exceptions
                }
            }
        }

    private:
        std::atomic<bool>        m_Stop{ false };
        Functor_T                m_TaskProcessor;

        std::queue<Task_T>       m_TaskQueue;
        std::mutex               m_QueueMutex;

        std::vector<std::thread> m_Workers;
        std::condition_variable  m_Condition;
    };

}
