#pragma once

#include <boost/serialization/access.hpp>

#include <string>

namespace SiM {

    /**
     * @brief Container for basic message info with serialization support
     *
     * @details Fields are immutable although, they can be modified via serialization.
     * So class is thread safe except it construction/destruction and serialization.
     */
    class Message {
     public:
        using IdType = unsigned long long;

     public:
        explicit Message() = default;

        explicit Message(std::string serializedString);

        explicit Message(IdType messageI, std::string loginFrom, std::string loginTo, std::string text);

        [[nodiscard]] auto id() const noexcept -> const IdType&;
        [[nodiscard]] auto from() const noexcept -> const std::string&;
        [[nodiscard]] auto to() const noexcept -> const std::string&;
        [[nodiscard]] auto text() const noexcept -> const std::string&;

        [[nodiscard]] auto serialize() const -> const std::string&;

        friend auto operator<=>(const Message& lhs, const Message& rhs) noexcept = default;

     private:
        friend class boost::serialization::access;

        template <typename Archive>
        auto serialize(Archive& arh, [[maybe_unused]] const unsigned int version) -> void;

     private:
        IdType m_messageId{};
        std::string m_from;
        std::string m_to;
        std::string m_text;

        mutable std::string m_serializedMessage;
    };

    template <typename Archive>
    inline auto Message::serialize(Archive& arh, [[maybe_unused]] const unsigned int version) -> void {
        arh& m_messageId;
        arh& m_from;
        arh& m_to;
        arh& m_text;
    }

}  // namespace SiM
