#pragma once

#include "Message.hpp"
#include "Notifier.hpp"
#include "detail/AsyncPrint.hpp"
#include "logic/Interfaces.hpp"

namespace SiM::Logic::Client::Detail {

    class ClientPrintAllMessages final : public SiM::Notifier<SiM::Message>::Listener {
     public:
        auto notify(const SiM::Message& serializedMessage) -> void override;
    };

    class ClientCommandParser final : public Interfaces::CommandParserBase {
     public:
        ClientCommandParser(Interfaces::AsioApplicationBase& application);
        auto parseCommand(std::string_view command) -> std::shared_ptr<Interfaces::CommandBase> override;

     private:
        mutable Message::IdType m_messageId;
    };

}  // namespace SiM::Logic::Client::Detail
