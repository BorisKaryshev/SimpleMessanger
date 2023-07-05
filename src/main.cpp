#include <iostream>

[[nodiscard]] auto subMain() -> int {
    std::cout << "Hello, world!\n";
    return 0;
}

auto main() -> int {
    return subMain();
}
