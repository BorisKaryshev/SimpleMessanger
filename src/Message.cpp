#include "Message.hpp"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <sstream>

namespace SiM {

    Message::Message(Message::IdType messageId, std::string loginFrom, std::string loginTo, std::string text)
        : m_messageId(messageId), m_from(std::move(loginFrom)), m_to(std::move(loginTo)), m_text(std::move(text)) {
        [[maybe_unused]] auto tmp = serialize();
    }

    [[nodiscard]] auto Message::id() const noexcept -> const IdType& {
        return m_messageId;
    }

    [[nodiscard]] auto Message::from() const noexcept -> const std::string& {
        return m_from;
    }

    [[nodiscard]] auto Message::to() const noexcept -> const std::string& {
        return m_to;
    }

    [[nodiscard]] auto Message::text() const noexcept -> const std::string& {
        return m_text;
    }

    Message::Message(std::string serializedString) : m_serializedMessage(std::move(serializedString)) {
        std::stringstream stream(m_serializedMessage);

        boost::archive::text_iarchive inArchive(stream);
        inArchive >> *this;
    }

    [[nodiscard]] auto Message::serialize() const -> const std::string& {
        if (m_serializedMessage.empty()) {
            std::stringstream stream;

            boost::archive::text_oarchive archive(stream);
            archive << *this;

            m_serializedMessage = stream.str();
        }

        return m_serializedMessage;
    }

}  // namespace SiM
