#include "logic/Server.hpp"

namespace SiM::Logic::Server {

    Server::Server(unsigned short port)
        : m_endpoint(boost::asio::ip::tcp::v4(), port), m_acceptor(m_context, m_endpoint), m_listener(*this), m_id(0){};

    auto Server::send(const Message& message) -> void {
        std::cout << "Sending message from '" << message.from() << "' to '" << message.to() << "' : '" << message.text() << "'\n";

        try {
            m_tableOfClients[message.to()].send(message.serialize());
        } catch (std::out_of_range& exception) {
            Message receiverNotFound(++m_id, SiM::Logic::Constants::serverName.data(), message.from(), ClientNotFoundMessage.data());

            m_tableOfClients[message.from()].send(receiverNotFound.serialize());
        }
    };

    auto Server::run(std::size_t numOfWorkerThread) -> void {
        m_accept();
        m_tableOfClients.runAll();

        std::list<std::jthread> workers;
        auto workerFunction = [this] { m_context.run(); };
        for ([[maybe_unused]] auto i : std::views::iota(0) | std::views::take(numOfWorkerThread)) {
            workers.emplace_front(workerFunction);
        }
    }

    auto Server::stop() -> void {
        m_acceptor.close();
        m_tableOfClients.stopAll();
    };

    auto Server::m_accept() -> void {
        auto handler = [this](const boost::system::error_code& errorCode, boost::asio::ip::tcp::socket sock) {
            if (!errorCode) {
                auto& connection = m_tableOfClients.emplace(std::move(sock));
                connection.addListener(std::addressof(m_listener));
            }
            m_accept();
        };
        m_acceptor.async_accept(handler);
    }

}  // namespace SiM::Logic::Server
