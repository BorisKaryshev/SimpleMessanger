#include "detail/LoginListener.hpp"
#include "Message.hpp"

#include <iostream>

namespace SiM::Logic::Server::Detail {

    LoginListener::LoginListener(std::unique_ptr<SiM::Connection>& connection,
                                 std::unordered_map<std::string, std::unique_ptr<SiM::Connection>>& table,
                                 std::list<std::unique_ptr<SiM::Connection>>& unauthorizedConnection, std::mutex& listModificationMutex)
        : m_listModificationMutex(listModificationMutex),
          m_connection(connection),
          m_table(table),
          m_unauthorizedConnection(unauthorizedConnection){};

    auto LoginListener::notify(const std::string& serializedMessage) -> void {
        Message message(serializedMessage);
        if (message.to() != SiM::Logic::Constants::serverName) {
            return;
        }

        std::lock_guard lock(m_listModificationMutex);
        m_table.emplace(std::make_pair(message.from(), std::move(m_connection)));

        auto elementToDelete = std::ranges::find_if(m_unauthorizedConnection, [](const auto& ptr) { return ptr == nullptr; });
        m_unauthorizedConnection.erase(elementToDelete);

        std::cout << message.from() << " connected\n";
        // m_table[message.from()]->removeListener(this);
    }

}  // namespace SiM::Logic::Server::Detail
