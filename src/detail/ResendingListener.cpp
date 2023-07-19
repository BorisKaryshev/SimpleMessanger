#include "detail/ResendingListener.hpp"
#include "Message.hpp"
#include "logic/Server.hpp"

namespace SiM::Logic::Server::Detail {

    ResendingListener::ResendingListener(Server& server) : m_server(server) {}

    auto ResendingListener::notify(const std::string& serializedMessuage) -> void {
        Message message(serializedMessuage);

        std::cout << "Got message from '" << message.from() << "' to '" << message.to() << "' :" << message.text() << "'\n";
        if (message.to() != Logic::Constants::serverName) {
            m_server.send(message);
        }
    }

}  // namespace SiM::Logic::Server::Detail
