#include "Message.hpp"

#include <gtest/gtest.h>

#include <future>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::string_literals;

TEST(SerializeDeserialize, Correct) {
    SiM::Message message(1, "Bob"s, "Mark"s, "Hello, Mark! This is Bob"s);

    const auto serialized = message.serialize();
    SiM::Message deserializedMessage(serialized);

    ASSERT_EQ(message, deserializedMessage);
}

TEST(SerializeDeserialize, emptyMessage) {
    SiM::Message message;

    const auto serialized = message.serialize();
    SiM::Message deserializedMessage(serialized);

    ASSERT_EQ(message, deserializedMessage);
}

TEST(MultipleThreads, Correct) {
    SiM::Message message(1, "Bob"s, "Mark"s, "Hello, Mark! This is Bob"s);

    SiM::Message result;
    auto helper = [&result](const std::string& serializedMessage) -> void { result = SiM::Message(serializedMessage); };
    std::thread thread(helper, message.serialize());
    thread.join();

    ASSERT_EQ(message, result);
}
