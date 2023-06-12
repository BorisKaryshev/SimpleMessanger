#include "Executor.hpp"

#include <gtest/gtest.h>

#include <unordered_map>

TEST(Executor, Correct) {
    constexpr std::size_t numOfThreads = 8;
    constexpr std::size_t numOfTasks = 100;
    constexpr std::size_t size = 100;

    std::mutex valuesMutex;
    std::unordered_map<std::size_t, std::size_t> values;

    auto task = [&values, &valuesMutex] {
        for (std::size_t i = 0; i < size; ++i) {
            std::lock_guard lock{valuesMutex};
            ++values[i];
        }
    };

    {
        Threads::Executor exec{numOfThreads};

        for (std::size_t i = 0; i < numOfTasks; ++i) {
            exec.addTask(task);
        }
    }

    ASSERT_EQ(values.size(), size);
    for (std::size_t i = 0; i < size; ++i) {
        ASSERT_EQ(values[i], numOfTasks);
    }
}
