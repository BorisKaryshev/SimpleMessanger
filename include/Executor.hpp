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
     * @brief Thread pool. Contains some amount of threads, and execute tasks in them.
     */
    class Executor {
     public:
        /**
         * @param numOfThreads Number of threads to be created in Executor.
         */
        explicit Executor(std::size_t numOfThreads = 1);
        ~Executor();

        template <typename Func, typename... Args>
            requires std::invocable<Func, Args...> && std::same_as<std::invoke_result_t<Func, Args...>, void>
        auto addTask(Func&& func, Args&&... args) {
            m_addTask(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        }

        /**
         * @brief Stops accepting new tasks, thus stopping Executor. Tasks which have been added before stop will be executed.
         */
        auto stop() -> void;

     private:
        using Task = std::function<void()>;

     private:
        auto m_addTask(const Task& task) -> void;
        auto m_loop() -> void;

     private:
        std::atomic_bool m_isRunning;
        std::mutex m_mutex;
        std::condition_variable m_condVariable;
        std::queue<Task> m_queue;
        std::vector<std::thread> m_threads;
    };

}  // namespace Threads
