#include "logic/Client.hpp"
#include "TCP.hpp"
#include "detail/ClientLogic.hpp"

#include <iostream>
#include <string>
#include <thread>

namespace {

    template <typename... Args>
        requires requires(std::stringstream stream, Args... args) { (stream << ... << args); }
    auto print(const Args&... args) -> void {
        std::stringstream stream;
        (stream << ... << args);
        std::cout << stream.str();
    }

}  // namespace

namespace SiM::Logic::Client {

    Client::Client(boost::asio::ip::address address, unsigned short port) : m_connection(std::nullopt) {
        boost::asio::ip::tcp::endpoint endpoint(address, port);
        boost::asio::ip::tcp::socket sock(m_context);
        sock.connect(endpoint);

        m_connection.emplace(Launch::RunManually, std::move(sock));
    }

    auto Client::send(const Message& message) -> void {
        m_connection->send(message.serialize());
    }

    auto Client::stop() -> void {
        m_isRunning = false;
        m_connection->close();
    }

    auto Client::run() -> void {
        std::jthread worker([this] { m_context.run(); });
        Detail::ClientCommandParser parser(*this);
        m_isRunning = true;

        m_sendLoginOnServer();

        while (m_isRunning) {
            std::string command;
            std::getline(std::cin, command);
            parser.parseCommand(command)->execute();
        }
    }

    auto Client::m_sendLoginOnServer() -> void {
        constexpr char delimeter = ' ';

        std::string login;
        print("Enter your login: ");
        std::getline(std::cin, login, delimeter);

        while (login == Constants::serverName) {
            print("\nError: your login should not be equal to '", Constants::serverName, "'. Try again: ");
            std::getline(std::cin, login, delimeter);
        }

        Message message(0, login, Constants::serverName.data(), "");
        send(message);
    }

}  // namespace SiM::Logic::Client
