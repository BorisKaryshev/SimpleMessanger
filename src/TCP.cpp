#include "TCP.hpp"

#include <iostream>

namespace SiM {

    Connection::Connection(boost::asio::ip::tcp::socket sock) : m_sock(std::move(sock)), m_isRunning(false) {}

    auto Connection::run() -> void {
        if (m_sock.is_open()) {
            m_isRunning = true;
            m_read();
        } else {
            throw std::runtime_error("Connection lost");
        }
    }

    auto Connection::stop() -> void {
        m_sock.cancel();
        m_isRunning = false;
    }

    auto Connection::isRunning() const noexcept -> bool {
        return m_isRunning;
    }

    auto Connection::send(const std::string& text) -> void {
        if (text.size() > maxMessageSize) {
            throw std::runtime_error("Messages with size more than " + std::to_string(maxMessageSize) + " bytes are not supported");
        }
        if (m_isRunning && m_sock.is_open()) {
            m_sock.async_send(boost::asio::buffer(text), []([[maybe_unused]] const boost::system::error_code&, [[maybe_unused]] size_t) {});
        }
    }

    auto Connection::m_read() -> void {
        auto readHandler = [this]([[maybe_unused]] const boost::system::error_code& errorCode, size_t bytes) {
            constexpr char endLine = '\0';

            if (!errorCode) {
                m_acceptedMessage[bytes] = endLine;
                notifyAll(m_acceptedMessage.data());
            } else if (errorCode == boost::asio::error::operation_aborted) {
                return;
            } else {
                std::cerr << "Error happened " << errorCode << "\n";
            }

            m_read();
        };

        m_sock.async_read_some(boost::asio::buffer(m_acceptedMessage), readHandler);
    }

}  // namespace SiM
