#include "detail/ClientLogic.hpp"

namespace SiM::Logic::Client::Detail {

    SendMessageCommand::SendMessageCommand(Interfaces::AsioApplicationBase& application, Message message) : CommandBase(application), m_messageToSend(std::move(message)){};

    auto SendMessageCommand::execute() -> void {
        m_application.send(m_messageToSend);
    };

    auto CLientPrintAllMessages::notify(const std::string& serializedMessage) -> void {
        Message message(serializedMessage);
        print(std::cout, message.from(), ": ", message.text());
    }

    ClientCommandParser::ClientCommandParser(Interfaces::AsioApplicationBase& application) : CommandParserBase(application), m_messageId(0) {}

    auto ClientCommandParser::parseCommand(std::string_view command) -> std::shared_ptr<Interfaces::CommandBase> {
        if (Commands::getCommandType(command) == Commands::send::tag) {
            std::string receiverLogin = Commands::send::getReceiverLogin(command).data();
            std::string text = Commands::dropFirstNTokens(command, 2).data();

            Message messageToSend(++m_messageId, Commands::send::initialLogin.data(), receiverLogin, text);

            return std::make_shared<SendMessageCommand>(m_application, messageToSend);
        }

        return std::make_shared<Interfaces::CommandBase>(m_application);
    }

}  // namespace SiM::Logic::Client::Detail