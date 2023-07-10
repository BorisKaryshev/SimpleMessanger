#include "TCP.hpp"
#include "LowLevelMessage.hpp"

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
        m_send(std::make_shared<Detail::Message>(text), std::make_shared<std::size_t>(1));
    }

    auto Connection::m_send(std::shared_ptr<Detail::Message> message, std::shared_ptr<std::size_t> count) -> void {
        if (message->serialize().size() > maxMessageSize) {
            throw std::runtime_error("Messages with size more than " + std::to_string(maxMessageSize) + " bytes are not supported");
        }

        if (m_isRunning) {
            auto handler = [this, message, count](const boost::system::error_code& errorCode, [[maybe_unused]] size_t) {
                if (errorCode) {
                    std::cout << "ERROR m_send(): " << errorCode.message() << "\n";

                    if (errorCode != boost::asio::error::operation_aborted) {
                        if ((*count) < 3) {
                            this->m_send(message, count);
                        }
                        ++(*count);
                    }
                }
            };

            m_sock.async_send(boost::asio::buffer(message->serialize()), handler);
        }
    }

    auto Connection::m_read() -> void {
        auto readHandler = [this](const boost::system::error_code& errorCode, size_t bytes) {
            if (!errorCode) {
                std::stringstream stream;
                stream.str(std::string(m_acceptedMessage.data(), bytes));

                while (stream.rdbuf()->in_avail() != 0) {
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
