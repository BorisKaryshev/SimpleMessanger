#include "TCP.hpp"
#include "detail/AsyncPrint.hpp"
#include "detail/LowLevelMessage.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

using SiM::Detail::Message;

namespace SiM {

    Connection::Connection(boost::asio::ip::tcp::socket sock) : m_sock(std::move(sock)), m_isRunning(false), m_acceptedMessage() {}
    Connection::Connection([[maybe_unused]] Detail::RunAtConstruction_t /*unused*/, boost::asio::ip::tcp::socket sock)
        : Connection(std::move(sock)) {
        run();
    }
    Connection::Connection([[maybe_unused]] Detail::RunManually_t /*unused*/, boost::asio::ip::tcp::socket sock)
        : Connection(std::move(sock)) {}

    auto Connection::run() -> void {
        std::lock_guard lock(m_socketInteractionMutex);
        if (m_sock.is_open()) {
            m_isRunning = true;
            m_read();
        } else {
            throw std::runtime_error("Connection lost");
        }
    }

    auto Connection::close() -> void {
        std::lock_guard lock(m_socketInteractionMutex);
        if (m_sock.is_open()) {
            m_sock.close();
        }
        m_isRunning = false;
    }

    auto Connection::isRunning() const noexcept -> bool {
        return m_isRunning;
    }

    auto Connection::send(const std::string& text) -> void {
        auto message = std::make_shared<Message>(text);

        if (message->serialize().size() > maxMessageSize) {
            throw std::runtime_error("Messages with size more than " + std::to_string(maxMessageSize) + " bytes are not supported");
        }

        if (m_isRunning) {
            auto handler = [this, message](const boost::system::error_code& errorCode, [[maybe_unused]] size_t bytes) {
                if (errorCode) {
                    std::cout << "ERROR m_send(): " << errorCode.message() << "\n";
                }
            };

            m_sock.async_write_some(boost::asio::buffer(message->serialize()), handler);
        }
    }

    auto Connection::m_read() -> void {
        auto readHandler = [this](const boost::system::error_code& errorCode, size_t bytes) {
            if (!errorCode) {
                std::stringstream stream(std::string(m_acceptedMessage.data(), bytes));

                while (stream.rdbuf()->in_avail() != 0 && bytes > 0) {
                    Message message;
                    stream >> message;
                    notifyAll(message.text());
                }
            }

            if (errorCode == boost::asio::error::operation_aborted || errorCode == boost::asio::error::bad_descriptor ||
                errorCode == boost::asio::error::misc_errors::eof) {  // Connection closed
                close();
                return;
            }

            if (errorCode) {
                std::cerr << "ERROR m_read(): " << errorCode.message() << "\n";
                close();
                return;
            }

            m_read();
        };

        m_sock.async_read_some(boost::asio::buffer(m_acceptedMessage), readHandler);
    }

}  // namespace SiM

namespace SiM::Launch {

    const SiM::Detail::RunAtConstruction_t RunAtConstruction;
    const SiM::Detail::RunManually_t RunManually;

}  // namespace SiM::Launch
