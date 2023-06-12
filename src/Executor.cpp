#include "Executor.hpp"

#include <algorithm>
#include <iostream>

namespace Threads {

    Executor::Executor(std::size_t numOfThreads) : m_isRunning{true} {
        m_threads.reserve(numOfThreads);
        for (std::size_t i = 0; i < numOfThreads; ++i) {
            m_threads.emplace_back(&Executor::m_loop, this);
        }
    }

    Executor::~Executor() {
        stop();
        std::ranges::for_each(m_threads, [](std::thread& thread) { thread.join(); });
    }

    auto Executor::stop() -> void {
        m_isRunning = false;
        m_condVariable.notify_all();
    }

    auto Executor::m_addTask(const Executor::Task& task) -> void {
        if (!m_isRunning) {
            return;
        }

        std::lock_guard lock{m_mutex};
        m_queue.push(task);
    }

    auto Executor::m_loop() -> void {
        for (;;) {
            Executor::Task task;

            {
                std::unique_lock lock{m_mutex};
                m_condVariable.wait(lock, [this] { return !m_queue.empty() || !m_isRunning; });
                if (!m_isRunning && m_queue.empty()) {
                    return;
                }

                task = m_queue.front();
                m_queue.pop();
            }

            task();
        }
    }

}  // namespace Threads
