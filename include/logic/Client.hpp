#pragma once

#include "Interfaces.hpp"
#include "Message.hpp"
#include "Notifier.hpp"
#include "TCP.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace SiM {

    class Connection;

}

namespace SiM::Logic::Client::Commands {

    constexpr auto getCommandType(std::string_view str) -> std::string_view {
        auto end = str.find(SiM::Logic::Constants::Delimeter);
        return std::string_view(str.data(), end);
    }

    /**
     * @brief
     * @param str - string_view to basic string
     * @param n - number of first tokens to be dropped
     * @return - string_view to string without first n tokens
     *
     * @todo exception safety
     */
    constexpr auto dropFirstNTokens(std::string_view str, std::size_t n) -> std::string_view {
        if (n == 0) {
            return str;
        }

        auto begin = str.find_first_not_of(SiM::Logic::Constants::Delimeter);
        auto end = str.find_first_of(SiM::Logic::Constants::Delimeter, begin);

        for (; n > 0; --n) {
            begin = str.find_first_not_of(SiM::Logic::Constants::Delimeter, end);
            end = str.find_first_of(SiM::Logic::Constants::Delimeter, begin);
        }

        return {str.data() + begin};
    }

    namespace send {

        constexpr std::string_view tag = "send";
        constexpr std::string_view initialLogin = "__InitialLogin";

    }  // namespace send

    namespace send {

        /**
         * @brief Mask on string_view, returning string_view on login - part of initial string_view
         * @todo Exception safety
         */
        constexpr auto getReceiverLogin(std::string_view str) -> std::string_view {
            auto begin = str.find(Constants::Delimeter);
            auto end = str.find(Constants::Delimeter, begin);
            return std::string_view(str.data() + begin, end);
        }

    }  // namespace send

}  // namespace SiM::Logic::Client::Commands

namespace SiM::Logic::Client {

    class Client : public Interfaces::AsioApplicationBase, private SiM::Notifier<Message> {
     public:
        Client(boost::asio::ip::address address, unsigned short port);

        auto send(const Message& message) -> void override;
        auto stop() -> void override;

        auto run() -> void;

     private:
        auto m_sendLoginOnServer() -> void;

     private:
        boost::asio::io_context m_context;
        std::optional<Connection> m_connection;

        std::atomic_bool m_isRunning;
    };

}  // namespace SiM::Logic::Client
