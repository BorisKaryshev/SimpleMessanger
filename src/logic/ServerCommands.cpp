#include "logic/ServerCommands.hpp"
#include "detail/AsyncPrint.hpp"

namespace SiM::Logic::Server::Commands::Detail {

    StopServerCommand::StopServerCommand(Interfaces::AsioApplicationBase& application) : Interfaces::CommandBase(application){};

    auto StopServerCommand::execute() -> void {
        m_application.stop();
    }

    auto UnparsedCommand::execute() -> void {
        print(std::cout, "Command unrecognized. Try again\n");
    }

}  // namespace SiM::Logic::Server::Commands::Detail
