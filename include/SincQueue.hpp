#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace Threads {

    template <typename T>
    class SincQueue {
     public:
        explicit SincQueue(const T& defaultValue = T{});

        auto push(const T& element) -> void;
        [[nodiscard]] auto pop() -> T;

        [[nodiscard]] auto empty() const -> bool;

        ~SincQueue() = default;

     private:
        std::mutex m_mutex;
        std::condition_variable m_condVariable;

        std::queue<T> m_queue;
    };

}  // namespace Threads

#include "SincQueue.inl"
