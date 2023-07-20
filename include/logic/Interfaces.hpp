#pragma once

#include <memory>
#include <string_view>

namespace SiM {

    class Message;

}

namespace SiM::Logic::Interfaces {

    class AsioApplicationBase {
     public:
        virtual auto send(const Message& message) -> void = 0;
        virtual auto stop() -> void = 0;

        virtual ~AsioApplicationBase() = default;
    };

    class CommandBase {
     public:
        CommandBase(AsioApplicationBase& application) : m_application(application) {}
        CommandBase(const CommandBase& other) : CommandBase(other.m_application) {}

        virtual auto execute() -> void{};
        virtual ~CommandBase() = default;

     protected:
        AsioApplicationBase& m_application;
    };

    class CommandParserBase {
     public:
        CommandParserBase(AsioApplicationBase& application) : m_application(application) {}
        CommandParserBase(const CommandParserBase& other) : CommandParserBase(other.m_application) {}

        virtual auto parseCommand(std::string_view command) -> std::shared_ptr<CommandBase> = 0;
        virtual ~CommandParserBase() = default;

     protected:
        AsioApplicationBase& m_application;
    };

}  // namespace SiM::Logic::Interfaces

namespace SiM::Logic::Constants {

    constexpr std::string_view serverName = "server";
    constexpr char Delimeter = ' ';

}  // namespace SiM::Logic::Constants
