#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace Threads {

    template <typename T>
    class SincQueue {
     public:
        explicit SincQueue();

        auto push(const T& element) -> void;
        [[nodiscard]] auto pop() -> T;

        [[nodiscard]] auto empty() const -> bool;

        ~SincQueue() = default;

     private:
        mutable std::mutex m_mutex;
        mutable std::condition_variable m_condVariable;

        std::queue<T> m_queue;
    };

}  // namespace Threads

#include "SincQueue.inl"
