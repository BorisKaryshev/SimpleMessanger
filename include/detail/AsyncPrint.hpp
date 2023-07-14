#pragma once

#include <iostream>
#include <sstream>

namespace PrintHelper {

    template <typename T>
    concept Printable = requires(T x) { std::declval<std::ostream>() << x; };

}

namespace {

    template <PrintHelper::Printable... Args>
    auto print(std::ostream& outStream, const Args&... args) -> void {
        std::stringstream stream;
        (stream << ... << args);
        outStream << stream.str();
    }

}  // namespace
