#include "detail/ServerCommandParser.hpp"
#include "logic/ServerCommands.hpp"

namespace SiM::Logic::Server::Detail {

    ServerCommandParser::ServerCommandParser(Interfaces::AsioApplicationBase& application) : Interfaces::CommandParserBase(application) {}

    [[nodiscard]] auto ServerCommandParser::parseCommand(std::string_view command) -> std::shared_ptr<Interfaces::CommandBase> {
        if (command == Commands::stop::tag) {
            return std::make_shared<Commands::Detail::StopServerCommand>(m_application);
        }

        return std::make_shared<Interfaces::CommandBase>(m_application);
    }

};  // namespace SiM::Logic::Server::Detail
