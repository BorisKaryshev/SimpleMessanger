#include "SincQueue.hpp"

namespace Threads {

    template <typename T>
    inline auto AsyncQueue<T>::push(const T& element) -> void {
        {
            std::lock_guard lock{m_queueMutex};
            m_queue.push(element);
        }
        m_queueCondVariable.notify_one();
    }

    template <typename T>
    [[nodiscard]] inline auto AsyncQueue<T>::pop() -> T {
        std::unique_lock lock{m_queueMutex};
        m_queueCondVariable.wait(lock, [this] { return !m_queue.empty(); });

        T tmp = m_queue.front();
        m_queue.pop();

        return tmp;
    }

    template <typename T>
    [[nodiscard]] inline auto AsyncQueue<T>::tryPop() -> std::optional<T> {
        std::lock_guard lock(m_queueMutex);
        if (m_queue.empty()) {
            return std::nullopt;
        }
        T tmp = m_queue.front();
        m_queue.pop();
        return tmp;
    }

}  // namespace Threads
