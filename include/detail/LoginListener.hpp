#pragma once

#include "Notifier.hpp"
#include "TCP.hpp"
#include "logic/Interfaces.hpp"

#include <list>
#include <memory>

namespace SiM::Logic::Server::Detail {

    class LoginListener final : public SiM::Notifier<std::string>::Listener {
     public:
        LoginListener(std::unique_ptr<SiM::Connection>& connection,
                      std::unordered_map<std::string, std::unique_ptr<SiM::Connection>>& table,
                      std::list<std::unique_ptr<SiM::Connection>>& unauthorizedConnection, std::mutex& listModificationMutex);

        auto notify(const std::string& serializedMessage) -> void override;

     private:
        std::mutex& m_listModificationMutex;
        std::unique_ptr<SiM::Connection>& m_connection;
        std::unordered_map<std::string, std::unique_ptr<SiM::Connection>>& m_table;
        std::list<std::unique_ptr<SiM::Connection>>& m_unauthorizedConnection;
    };

}  // namespace SiM::Logic::Server::Detail
