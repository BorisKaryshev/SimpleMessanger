#include "SincQueue.hpp"

#include <gtest/gtest.h>

TEST(SingleThread, AppendOrder) {
    constexpr int size = 500;
    Threads::SincQueue<int> queue;

    for (int i = 0; i < size; ++i) {
        queue.push(i);
    }

    for (int i = 0; i < size; ++i) {
        ASSERT_EQ(queue.pop(), i);
    }
}

TEST(SingleThread, Empty) {
    constexpr int size = 500;
    Threads::SincQueue<int> queue;

    for (int i = 0; i < size; ++i) {
        queue.push(i);
    }

    for (int i = 0; i < 2 * size; ++i) {
        if (!queue.empty()) {
            ASSERT_EQ(queue.pop(), i);
        }
    }
}

TEST(MultipleThreads, First) {
    constexpr int size = 500;
    Threads::SincQueue<int> queue;

    std::thread thread{[&queue] {
        for (int i = 0; i < size; ++i) {
            queue.push(i);
        }
    }};

    for (int i = 0; i < size; ++i) {
        ASSERT_EQ(queue.pop(), i);
    }

    thread.join();
}
