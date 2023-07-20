#pragma once

#include "logic/Interfaces.hpp"

#include <string_view>

namespace SiM::Logic::Server::Commands {

    namespace stop {

        constexpr std::string_view tag = "stop";

    }

}  // namespace SiM::Logic::Server::Commands

namespace SiM::Logic::Server::Commands::Detail {

    class StopServerCommand final : public Interfaces::CommandBase {
     public:
        StopServerCommand(Interfaces::AsioApplicationBase& application);
        auto execute() -> void override;
    };

}  // namespace SiM::Logic::Server::Commands::Detail
