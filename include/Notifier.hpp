#pragma once

#include <algorithm>
#include <concepts>
#include <list>
#include <mutex>
#include <ranges>

namespace SiM {

    template <typename... Args>
    class Notifier {
     public:
        class Listener {
         public:
            virtual auto notify(const Args&... message) -> void = 0;
        };

     public:
        auto addListener(Listener* listener) -> Notifier&;
        auto removeListener(Listener* listener) -> Notifier&;

     protected:
        auto notifyAll(const Args&... args) -> void;

        template <typename Pred>
            requires std::is_invocable_r_v<bool, Pred, Listener*>
        auto notifyAllIf(Pred&& pred, const Args&... args) -> void;

     private:
        std::mutex m_listenersContainerModification;
        std::list<Listener*> m_listeners;
    };

}  // namespace SiM

#include "Notifier.inl"
