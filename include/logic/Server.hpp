#pragma once

#include "Message.hpp"
#include "Notifier.hpp"
#include "TCP.hpp"
#include "detail/ClientsTable.hpp"
#include "detail/ResendingListener.hpp"
#include "logic/Interfaces.hpp"

#include <iostream>
#include <list>
#include <thread>
#include <unordered_map>

namespace SiM::Logic::Server {

    class Server final : public Interfaces::AsioApplicationBase {
     public:
        Server(unsigned short port);

        auto send(const Message& message) -> void override;
        auto stop() -> void override;

        auto run(std::size_t numOfWorkerThread = 1) -> void;

     private:
        auto m_accept() -> void;

     private:
        static constexpr std::string_view ClientNotFoundMessage =
            "Server has no connection with requested client so there is no guarantee, that message will be delivered properly";

     private:
        boost::asio::io_context m_context;

        boost::asio::ip::tcp::endpoint m_endpoint;
        boost::asio::ip::tcp::acceptor m_acceptor;
        Detail::ClientsTable m_tableOfClients;
        Detail::ResendingListener m_listener;

        Message::IdType m_id;
    };

}  // namespace SiM::Logic::Server

namespace SiM {

    using SiM::Logic::Server::Server;

}
