#pragma once

namespace SiM::Detail {

    struct RunAtConstruction_t {};
    struct RunManually_t {};

}  // namespace SiM::Detail

namespace SiM::Launch {

    extern const SiM::Detail::RunAtConstruction_t RunAtConstruction;
    extern const SiM::Detail::RunManually_t RunManually;

}  // namespace SiM::Launch
