#include "Message.hpp"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <sstream>

namespace SiM {

    Message::Message(Message::IdType id, std::string from, std::string to, std::string text)
        : m_messageId(id), m_from(from), m_to(to), m_text(text) {}

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

    namespace {}  // namespace

    Message::Message(const std::string& serializedString) {
        std::stringstream stream(serializedString);

        Message m;

        boost::archive::text_iarchive inArchive(stream);
        inArchive >> m;

        std::swap(*this, m);
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

    // without static fields 3934 ms
    // with static fields

}  // namespace SiM
