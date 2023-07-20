#include "logic/ServerCommands.hpp"

namespace SiM::Logic::Server::Commands::Detail {

    StopServerCommand::StopServerCommand(Interfaces::AsioApplicationBase& application) : Interfaces::CommandBase(application){};

    auto StopServerCommand::execute() -> void {
        m_application.stop();
    }

}  // namespace SiM::Logic::Server::Commands::Detail
