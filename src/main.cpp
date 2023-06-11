#include <iostream>

[[nodiscard]] auto subMain() -> int {
    std::cout << "Hello from main\n";
    return 0;
}

[[nodiscard]] auto tmp() -> int {
    return 0;
}

auto main() -> int {
    // countTime(subMain);
    return tmp();
}
