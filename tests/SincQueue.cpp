#include "SincQueue.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>
#include <vector>

TEST(SingleThread, AppendOrder) {
    constexpr int size = 500;
    Threads::AsyncQueue<int> queue;

    for (int i = 0; i < size; ++i) {
        queue.push(i);
    }

    for (int i = 0; i < size; ++i) {
        ASSERT_EQ(queue.pop(), i);
    }
}

TEST(SingleThread, Empty) {
    constexpr int size = 500;
    Threads::AsyncQueue<int> queue;

    for (int i = 0; i < size; ++i) {
        queue.push(i);
    }

    for (int i = 0; i < 2 * size; ++i) {
        auto result = queue.tryPop();
        if (result) {
            ASSERT_EQ(*result, i);
        }
    }
}

TEST(MultipleThreads, AddInOneThreadPopInAnother) {
    constexpr int size = 500;
    Threads::AsyncQueue<int> queue;

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

TEST(MultipleThreads, TestForDRD) {
    constexpr int size = 300;
    Threads::AsyncQueue<int> queue;

    std::vector<int> valuePopped;
    valuePopped.reserve(size);

    std::vector<int> valuePushed;
    valuePushed.reserve(size);

    auto pusher = [&queue, &valuePushed](int low, int high) -> void {
        static std::mutex m;
        for (int i = low; i < high; ++i) {
            queue.push(i);
            {
                std::lock_guard lock{m};
                valuePushed.push_back(i);
            }
        }
    };

    auto popper = [&valuePopped, &queue](int numOfPops) -> void {
        static std::mutex m;
        for (int i = 0; i < numOfPops; ++i) {
            auto value = queue.pop();
            {
                std::lock_guard lock{m};
                valuePopped.push_back(value);
            }
        }
    };

    std::thread pushers[] = {std::thread(pusher, 0, size / 3), std::thread(pusher, size / 3, 2 * size / 3),
                             std::thread(pusher, 2 * size / 3, size)};

    std::thread poppers[] = {std::thread(popper, size / 4), std::thread(popper, size / 4), std::thread(popper, size / 4),
                             std::thread(popper, size / 4)};

    for (auto& thread : pushers) {
        thread.join();
    }
    for (auto& thread : poppers) {
        thread.join();
    }

    std::ranges::sort(valuePopped);
    std::ranges::sort(valuePushed);
    ASSERT_EQ(valuePopped, valuePushed);
}
