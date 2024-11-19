#include "logic/Client.hpp"

// update

namespace {

    constexpr std::size_t AddressPositionInArguments = 1;
    constexpr std::size_t PortPositionInArguments = 2;

}  // namespace

auto subMain(std::vector<std::string> arguments) -> int {
    if (arguments.size() <= PortPositionInArguments || arguments.size() <= AddressPositionInArguments) {
        std::cout << "Not all required arguments given\n";
        return 1;
    }

    auto address = boost::asio::ip::address::from_string(arguments[AddressPositionInArguments]);
    int port = std::stoi(arguments[PortPositionInArguments]);

    SiM::Client client(address, port);
    client.run();

    return 0;
}

auto main(int argv, char* argc[]) -> int {
    return subMain({argc, argc + argv});
}
