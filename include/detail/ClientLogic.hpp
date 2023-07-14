#pragma once

#include "Message.hpp"
#include "Notifier.hpp"
#include "detail/AsyncPrint.hpp"
#include "logic/Client.hpp"
#include "logic/Interfaces.hpp"

namespace SiM::Logic::Client::Detail {

    class SendMessageCommand final : public Interfaces::CommandBase {
     public:
        SendMessageCommand(Interfaces::AsioApplicationBase& application, Message message);
        auto execute() -> void override;

     private:
        const Message m_messageToSend;
    };

    class CLientPrintAllMessages final : public SiM::Notifier<std::string>::Listener {
     public:
        auto notify(const std::string& serializedMessage) -> void override;
    };

    class ClientCommandParser final : public Interfaces::CommandParserBase {
     public:
        ClientCommandParser(Interfaces::AsioApplicationBase& application);
        auto parseCommand(std::string_view command) -> std::shared_ptr<Interfaces::CommandBase> override;

     private:
        mutable Message::IdType m_messageId;
    };

}  // namespace SiM::Logic::Client::Detail
