#include "detail/ClientCommands.hpp"
#include "detail/AsyncPrint.hpp"

namespace SiM::Logic::Client::Commands {

    auto UnparsedCommand::execute() -> void {
        print(std::cout, "Command unrecognized. Try again\n");
    }

    auto StopCommand::execute() -> void {
        m_application.stop();
    }

    SendMessageCommand::SendMessageCommand(Interfaces::AsioApplicationBase& application, Message message)
        : CommandBase(application), m_messageToSend(std::move(message)){};

    auto SendMessageCommand::execute() -> void {
        m_application.send(m_messageToSend);
    };

}  // namespace SiM::Logic::Client::Commands
