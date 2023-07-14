#include "logic/Client.hpp"
#include "TCP.hpp"
#include "detail/ClientLogic.hpp"

#include "detail/AsyncPrint.hpp"

#include <iostream>
#include <string>
#include <thread>

namespace SiM::Logic::Client {

    Client::Client(boost::asio::ip::address address, unsigned short port) : m_connection(std::nullopt) {
        boost::asio::ip::tcp::endpoint endpoint(address, port);
        boost::asio::ip::tcp::socket sock(m_context);
        sock.connect(endpoint);

        m_connection.emplace(Launch::RunManually, std::move(sock));
        m_connection->addListener(std::addressof(m_printer));
    }

    auto Client::send(const Message& message) -> void {
        m_connection->send(message.serialize());
    }

    auto Client::stop() -> void {
        m_connection->close();
    }

    auto Client::run() -> void {
        Detail::ClientCommandParser parser(*this);
        m_connection->run();

        std::jthread worker([this] { m_context.run(); });

        parser.setLogin(m_sendLoginOnServer());

        while (m_connection->isRunning()) {
            std::string command;
            std::getline(std::cin, command);
            parser.parseCommand(command)->execute();
        }
    }

    auto Client::m_sendLoginOnServer() -> std::string {
        std::string login;
        print(std::cout, "Enter your login: ");
        std::getline(std::cin, login);

        while (login == Constants::serverName) {
            print(std::cout, "\nError: your login should not be equal to '", Constants::serverName, "'. Try again: ");
            std::getline(std::cin, login);
        }

        Message message(0, login, Constants::serverName.data(), "");
        send(message);

        return login;
    }

}  // namespace SiM::Logic::Client
