#include "detail/ClientCommands.hpp"

namespace SiM::Logic::Client::Commands {

    StopCommand::StopCommand(Interfaces::AsioApplicationBase& application) : CommandBase(application) {}
    auto StopCommand::execute() -> void {
        m_application.stop();
    }

    SendMessageCommand::SendMessageCommand(Interfaces::AsioApplicationBase& application, Message message)
        : CommandBase(application), m_messageToSend(std::move(message)){};

    auto SendMessageCommand::execute() -> void {
        m_application.send(m_messageToSend);
    };

}  // namespace SiM::Logic::Client::Commands
