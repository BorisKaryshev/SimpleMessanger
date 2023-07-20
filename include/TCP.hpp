#pragma once

#include "Notifier.hpp"
#include "detail/LaunchMode.hpp"
#include "detail/LowLevelMessage.hpp"

#include <boost/asio.hpp>

#include <mutex>
#include <ranges>

namespace SiM {

    /**
     * @brief Tcp connection. Notifies all listeners on receiving message.
     * @details Is thread safe. Is noncopyable and nonmovable.
     */
    class Connection : public Notifier<std::string> {
     public:
        Connection(Detail::RunAtConstruction_t, boost::asio::ip::tcp::socket sock);
        Connection(Detail::RunManually_t, boost::asio::ip::tcp::socket sock);

        Connection(const Connection&) = delete;
        auto operator=(const Connection&) -> Connection& = delete;

        Connection(Connection&&) = default;
        auto operator=(Connection&&) -> Connection& = default;

        auto run() -> void;
        [[nodiscard]] auto isRunning() const noexcept -> bool;

        auto close() -> void;
        auto send(const std::string& text) -> void;

     private:
        Connection(boost::asio::ip::tcp::socket sock);
        auto m_read() -> void;

     private:
        constexpr static size_t kiloByte = 1 << 10;
        constexpr static size_t maxMessageSize = kiloByte << 2;

     private:
        boost::asio::ip::tcp::socket m_sock;
        std::atomic_bool m_isRunning;
        mutable std::mutex m_socketInteractionMutex;

        std::array<char, maxMessageSize> m_acceptedMessage;

    };  // namespace SiM

}  // namespace SiM
