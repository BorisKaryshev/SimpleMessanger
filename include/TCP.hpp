#pragma once

#include "Notifier.hpp"

#include <boost/asio.hpp>

namespace SiM {

    /**
     * @brief Tcp connection. Notifies all listeners on receiving message.
     * @details Is thread safe. Is noncopyable and nonmovable.
     */
    class Connection : public Notifier<std::string> {
     public:
        Connection(boost::asio::ip::tcp::socket sock);

        Connection(const Connection&) = delete;
        auto operator=(const Connection&) -> Connection& = delete;

        auto run() -> void;
        auto stop() -> void;
        [[nodiscard]] auto isRunning() const noexcept -> bool;

        auto send(const std::string& text) -> void;

     private:
        auto m_read() -> void;

     private:
        constexpr static size_t kiloByte = 1 << 10;
        constexpr static size_t maxMessageSize = kiloByte << 2;

     private:
        boost::asio::ip::tcp::socket m_sock;
        std::atomic_bool m_isRunning;
        
        std::array<char, maxMessageSize> m_acceptedMessage;
    };

}  // namespace SiM
