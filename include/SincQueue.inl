#include "SincQueue.hpp"

namespace Threads {

    template <typename T>
    SincQueue<T>::SincQueue() {}

    template <typename T>
    auto SincQueue<T>::push(const T& element) -> void {
        {
            std::lock_guard lock{m_mutex};
            m_queue.push(element);
        }
        m_condVariable.notify_one();
    }

    template <typename T>
    auto SincQueue<T>::pop() -> T {
        T tmp;
        {
            std::unique_lock lock{m_mutex};

            m_condVariable.wait(lock, [this] { return !m_queue.empty(); });

            tmp = m_queue.front();
            m_queue.pop();
        }

        return tmp;
    }

    template <typename T>
    auto SincQueue<T>::empty() const -> bool {
        std::lock_guard lock{m_mutex};
        return m_queue.empty();
    }

}  // namespace Threads
