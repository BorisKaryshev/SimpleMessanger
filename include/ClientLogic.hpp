#pragma once

#include "Message.hpp"
#include "TCP.hpp"

#include <iostream>
#include <optional>
#include <thread>
#include <unordered_map>

namespace SiM {


    namespace Logic::Client {

        constexpr std::string_view serverLogin = "server";

        namespace CommandTags {

            constexpr char Delimeter = ' ';



        };  // namespace CommandTags

    }  // namespace Logic::Client

}  // namespace SiM
