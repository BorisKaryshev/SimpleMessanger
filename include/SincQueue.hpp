#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

namespace Threads {

    template <typename T>
    class AsyncQueue {
     public:
        explicit AsyncQueue() = default;

        AsyncQueue(const AsyncQueue&) = delete;
        auto operator=(const AsyncQueue&) -> AsyncQueue& = delete;

        auto push(const T& element) -> void;

        /**
         * @brief Get element from queue. If queue is empty wait until element appears.
         */
        [[nodiscard]] auto pop() -> T;

        /**
         * @brief Get element if queue is not empty. Wont wait until element appearance.
         */
        [[nodiscard]] auto tryPop() -> std::optional<T>;

        ~AsyncQueue() = default;

     private:
        mutable std::mutex m_queueMutex;
        mutable std::condition_variable m_queueCondVariable;

        std::queue<T> m_queue;
    };

}  // namespace Threads

#include "SincQueue.inl"
