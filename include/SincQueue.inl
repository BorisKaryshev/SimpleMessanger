#include "SincQueue.hpp"

namespace Threads {

    template <typename T>
    SincQueue<T>::SincQueue(const T& defaultValue) : m_defaultValueToBeReturned{defaultValue} {}

    template <typename T>
    auto SincQueue<T>::add(const T& element) -> void {
        {
            std::lock_guard _{m_mutex};
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
        std::lock_guard _{m_mutex};
        return m_queue.empty();
    }

}  // namespace Threads
