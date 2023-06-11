#include <iostream>

[[nodiscard]] auto subMain() -> int {
    std::cout << "Hello from SiM\n";
    return 0;
}

auto main() -> int {
    return subMain();
}
