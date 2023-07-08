#include "TCP.hpp"
#include "LowLevelMessage.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

using SiM::Detail::Message;

namespace SiM {

    Connection::Connection(boost::asio::ip::tcp::socket sock) : m_sock(std::move(sock)), m_isRunning(false), m_acceptedMessage() {}

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

        auto handler = [this](const boost::system::error_code& ec, [[maybe_unused]] size_t) {
            if (ec) {
                std::cout << "Unable to send message: " << ec << "\n";
            }
        };

        if (m_isRunning) {
            Message message(text);
            m_sock.async_send(boost::asio::buffer(message.serialize()), handler);
        }
    }

    auto Connection::m_read() -> void {
        auto readHandler = [this](const boost::system::error_code& errorCode, size_t bytes) {
            if (!errorCode) {
                std::stringstream stream;
                stream << std::string(m_acceptedMessage.data(), bytes);
                while (stream.rdbuf()->in_avail()) {
                    Message message;
                    stream >> message;
                    notifyAll(message.text());
                }

            } else if (errorCode == boost::asio::error::operation_aborted) {
                return;
            } else {
                std::cerr << "Error happened " << errorCode << "\n";
                return;
            }
            m_read();
        };

        m_sock.async_read_some(boost::asio::buffer(m_acceptedMessage), readHandler);
    }

}  // namespace SiM
