#pragma once

#include "TCP.hpp"
#include "detail/LoginListener.hpp"

#include <boost/asio/ip/tcp.hpp>

#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace SiM::Logic::Server::Detail {

    class LoginListener;

}

namespace SiM::Logic::Server::Detail {

    class ClientsTable {
     public:
        auto emplace(boost::asio::ip::tcp::socket sock) -> SiM::Connection&;

        auto runAll() -> void;
        auto stopAll() -> void;

        [[nodiscard]] auto operator[](const std::string& login) const -> SiM::Connection&;

     private:
        using ConnectionPtr = std::unique_ptr<SiM::Connection>;

     private:
        std::mutex m_modificationMutex;
        std::atomic_bool m_isRunning;
        std::list<ConnectionPtr> m_unauthorizedConnections;
        std::unordered_map<std::string, ConnectionPtr> m_connection;
        std::list<LoginListener> m_loginListeners;
    };

}  // namespace SiM::Logic::Server::Detail
