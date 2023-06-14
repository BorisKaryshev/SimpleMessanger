#include "Message.hpp"

#include <gtest/gtest.h>

#include <iostream>

using namespace std::string_literals;

auto operator<<(std::ostream& stream, const SiM::Message& message) -> std::ostream& {
    return stream << message.id() << ' ' << message.from() << " " << message.to() << " " << message.text();
}

TEST(SerializeDeserialize, firstCorrect) {
    SiM::Message message(1, "Bob"s, "Mark"s, "Hello"s);

    auto serialized = message.serialize();

    SiM::Message deserializedMessage(serialized);

    std::cout << message << "\n" << deserializedMessage << "\n";
    std::cout << std::boolalpha << (message == deserializedMessage) << "\n";
    ASSERT_EQ(message, deserializedMessage);
}
