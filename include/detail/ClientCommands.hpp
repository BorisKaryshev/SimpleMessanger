#pragma once

#include "Message.hpp"
#include "logic/Interfaces.hpp"

#include <string_view>

namespace SiM::Logic::Client::Commands {

    /**
     * @brief Mask returning command type, represented by first token.
     *
     * @todo Exception safety
     */
    constexpr auto getCommandType(std::string_view str, char delimeter = SiM::Logic::Constants::Delimeter) -> std::string_view {
        auto end = str.find(delimeter);
        if (end == std::string_view::npos) {
            return str;
        }
        return std::string_view(str.data(), end);
    }

    /**
     * @brief Drops first N tokens from string, separated by delimeter
     * @param str string_view to basic string
     * @param n number of first tokens to be dropped
     * @return string_view to string without first n tokens
     *
     * @todo exception safety
     */
    constexpr auto dropFirstNTokens(std::string_view str, std::size_t n, char delimeter = SiM::Logic::Constants::Delimeter)
        -> std::string_view {
        if (n == 0) {
            return str;
        }

        auto begin = str.find_first_not_of(delimeter);
        auto end = str.find_first_of(delimeter, begin);

        for (; n > 0; --n) {
            begin = str.find_first_not_of(delimeter, end);
            end = str.find_first_of(delimeter, begin);
        }

        return {str.data() + begin};
    }

}  // namespace SiM::Logic::Client::Commands

namespace SiM::Logic::Client::Commands {

    namespace send {

        constexpr std::string_view tag = "send";
        constexpr std::string_view initialLogin = "__InitialLogin";

        /**
         * @brief Mask on string_view, returning string_view on login - part of initial string_view
         * @todo Exception safety
         */
        constexpr auto getReceiverLogin(std::string_view str, char delimeter = SiM::Logic::Constants::Delimeter) -> std::string_view {
            auto begin = str.find(delimeter);
            auto end = str.find_first_of(delimeter, begin + 1);
            return std::string_view(str.data() + begin + 1, end - begin - 1);
        }

    }  // namespace send

    namespace stop {

        constexpr std::string_view tag = "stop";

    }

}  // namespace SiM::Logic::Client::Commands

namespace SiM::Logic::Client::Commands {

    class SendMessageCommand final : public Interfaces::CommandBase {
     public:
        SendMessageCommand(Interfaces::AsioApplicationBase& application, Message message);
        auto execute() -> void override;

     private:
        const Message m_messageToSend;
    };

    class StopCommand final : public Interfaces::CommandBase {
     public:
        using Interfaces::CommandBase::CommandBase;

        auto execute() -> void override;
    };

    class UnparsedCommand final : public Interfaces::CommandBase {
     public:
        using Interfaces::CommandBase::CommandBase;

        auto execute() -> void override;
    };

}  // namespace SiM::Logic::Client::Commands
