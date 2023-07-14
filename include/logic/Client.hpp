#pragma once

#include "Interfaces.hpp"
#include "Message.hpp"
#include "Notifier.hpp"
#include "TCP.hpp"
#include "detail/ClientLogic.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace SiM {

    class Connection;

}

namespace SiM::Logic::Client {

    class Client : public Interfaces::AsioApplicationBase, private SiM::Notifier<Message> {
     public:
        Client(boost::asio::ip::address address, unsigned short port);

        auto send(const Message& message) -> void override;
        auto stop() -> void override;

        auto run() -> void;

     private:
        [[nodiscard]] auto m_sendLoginOnServer() -> std::string;

     private:
        boost::asio::io_context m_context;
        std::optional<Connection> m_connection;

        SiM::Logic::Client::Detail::ClientPrintAllMessages m_printer;
    };

}  // namespace SiM::Logic::Client

namespace SiM {

    using SiM::Logic::Client::Client;

}
