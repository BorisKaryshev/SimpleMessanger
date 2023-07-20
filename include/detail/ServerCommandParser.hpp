#pragma once

#include "logic/Interfaces.hpp"

namespace SiM::Logic::Server::Detail {

    class ServerCommandParser final : public Interfaces::CommandParserBase {
     public:
        ServerCommandParser(Interfaces::AsioApplicationBase& application);
        [[nodiscard]] auto parseCommand(std::string_view command) -> std::shared_ptr<Interfaces::CommandBase> override;
    };

};  // namespace SiM::Logic::Server::Detail
