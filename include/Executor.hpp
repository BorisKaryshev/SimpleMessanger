#pragma once

#include <atomic>
#include <concepts>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace Threads {

    /**
     * @brief Thread pool. Amount of threads specified in constructor.
     */
    class Executor {
     public:
        /**
         * @param numOfThreads Amount of threads to be created in Executor.
         */
        explicit Executor(std::size_t numOfThreads = 1);
        ~Executor();

        template <typename Func, typename... Args>
            requires std::invocable<Func, Args...> && std::same_as<std::invoke_result_t<Func, Args...>, void>
        auto addTask(Func&& func, Args&&... args) {
            m_addTask(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        }

     private:
        using m_Task = std::function<void()>;

     private:
        auto m_stop() -> void;

        auto m_addTask(const m_Task& task) -> void;
        auto m_loop() -> void;

     private:
        std::atomic_bool m_isRunning;
        std::mutex m_queueMutex;
        std::condition_variable m_queueCondVariable;

        std::queue<m_Task> m_queue;
        std::vector<std::jthread> m_threads;
    };

}  // namespace Threads
