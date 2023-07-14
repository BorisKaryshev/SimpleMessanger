#include "detail/ClientLogic.hpp"
#include "detail/ClientCommands.hpp"
#include "logic/Client.hpp"

namespace SiM::Logic::Client::Detail {

    auto ClientPrintAllMessages::notify(const std::string& serializedMessage) -> void {
        Message message(serializedMessage);
        print(std::cout, message.from(), ": ", message.text());
    }

    ClientCommandParser::ClientCommandParser(Interfaces::AsioApplicationBase& application)
        : CommandParserBase(application), m_messageId(0) {}

    auto ClientCommandParser::parseCommand(std::string_view command) -> std::shared_ptr<Interfaces::CommandBase> {
        if (Commands::getCommandType(command) == Commands::send::tag) {
            std::string_view receiverLoginView = Commands::send::getReceiverLogin(command);
            std::string receiverLogin(receiverLoginView.data(), receiverLoginView.size());
            std::string text = Commands::dropFirstNTokens(command, 2).data();

            Message messageToSend(++m_messageId, m_clientLogin, receiverLogin, text);

            return std::make_shared<Commands::SendMessageCommand>(m_application, messageToSend);
        }

        if (Commands::getCommandType(command) == Commands::stop::tag) {
            return std::make_shared<Commands::StopCommand>(m_application);
        }

        return std::make_shared<Commands::UnparsedCommand>(m_application);
    }

    auto ClientCommandParser::setLogin(const std::string& login) -> void {
        m_clientLogin = login;
    }

}  // namespace SiM::Logic::Client::Detail