#pragma once

#include <iomanip>
#include <sstream>
#include <string>

namespace SiM::Detail {

    class Message {
     public:
        Message(const std::string& text = "");
        [[nodiscard]] auto text() const noexcept -> const std::string&;
        [[nodiscard]] auto serialize() const -> const std::string&;

        auto deserialize(const std::string& str) -> void;

        friend auto operator>>(std::istream& stream, Message& message) -> std::istream&;

     public:
        static constexpr std::size_t MaxSizeLength = 5;

     private:
        std::string m_text;
        mutable std::string m_serialized;
    };

}  // namespace SiM::Detail
