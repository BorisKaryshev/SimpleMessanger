#include "detail/ClientsTable.hpp"
#include "detail/LoginListener.hpp"

namespace SiM::Logic::Server::Detail {

    auto ClientsTable::emplace(boost::asio::ip::tcp::socket sock) -> SiM::Connection& {
        std::lock_guard lock(m_modificationMutex);

        m_unauthorizedConnections.emplace_front(std::make_unique<SiM::Connection>(SiM::Launch::RunManually, std::move(sock)));
        ConnectionPtr& emplacedConnection = m_unauthorizedConnections.front();
        m_loginListeners.push_front(LoginListener(emplacedConnection, m_connection, m_unauthorizedConnections, m_modificationMutex));

        emplacedConnection->addListener(&m_loginListeners.front());
        if (m_isRunning) {
            emplacedConnection->run();
        }
        return *emplacedConnection;
    }

    auto ClientsTable::runAll() -> void {
        auto run = [](Connection& connection) { connection.run(); };

        std::lock_guard lock(m_modificationMutex);

        m_isRunning = true;
        std::ranges::for_each(m_unauthorizedConnections, run, &std::unique_ptr<SiM::Connection>::operator*);
        std::ranges::for_each(m_connection | std::views::values, run, &std::unique_ptr<SiM::Connection>::operator*);
    }

    auto ClientsTable::stopAll() -> void {
        m_isRunning = false;
        auto close = [](SiM::Connection& connection) { connection.close(); };

        std::ranges::for_each(m_unauthorizedConnections, close, &ConnectionPtr::operator*);
        std::ranges::for_each(m_connection | std::views::values, close, &ConnectionPtr::operator*);
    }

    [[nodiscard]] auto ClientsTable::operator[](const std::string& login) const -> SiM::Connection& {
        if (!m_connection.contains(login)) {
            throw std::out_of_range("Server have no connection with client with this login");
        }
        return *m_connection.at(login);
    }

}  // namespace SiM::Logic::Server::Detail
