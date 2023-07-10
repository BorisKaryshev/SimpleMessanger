#include "LowLevelMessage.hpp"

namespace SiM::Detail {

    Message::Message(std::string text) : m_text(std::move(text)) {}

    [[nodiscard]] auto Message::text() const noexcept -> const std::string& {
        return m_text;
    }

    [[nodiscard]] auto Message::serialize() const -> const std::string& {
        auto size = std::to_string(m_text.size());
        if (size.size() > MaxSizeLength) {
            throw std::runtime_error("Message too big");
        }

        std::stringstream stream;
        stream << std::setfill('0') << std::setw(MaxSizeLength) << size;
        stream << m_text;
        m_serialized = stream.str();

        return m_serialized;
    }

    auto Message::deserialize(const std::string& str) -> void {
        std::size_t size{std::stoull(str.substr(0, MaxSizeLength))};
        m_text = str.substr(MaxSizeLength);

        m_serialized = std::string(str.data(), MaxSizeLength + size);
    }

    auto operator>>(std::istream& stream, Message& message) -> std::istream& {
        std::size_t size{};
        stream >> std::setw(Message::MaxSizeLength) >> size;

        message.m_text = std::string(size, ' ');
        stream.read(message.m_text.data(), static_cast<std::streamsize>(size));

        message.m_serialized.clear();

        return stream;
    }

}  // namespace SiM::Detail
