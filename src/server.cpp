#include "logic/Server.hpp"

namespace {

    constexpr std::size_t PortPositionInArguments = 1;

}

auto subMain(std::vector<std::string> arguments) -> int {
    if (arguments.size() <= PortPositionInArguments) {
        std::cout << "Not all required arguments given\n";
        return 1;
    }
    SiM::Server server(std::stoi(arguments[PortPositionInArguments]));
    server.run();

    return 0;
}

auto main(int argv, char* argc[]) -> int {
    return subMain({argc, argc + argv});
}
