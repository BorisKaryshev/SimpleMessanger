#pragma once

#include <algorithm>
#include <concepts>
#include <list>
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
        auto addListener(Listener* listener) -> void { m_listeners.push_front(listener); }
        auto removeListener(Listener* listener) -> void {
            auto iterator = std::ranges::find(m_listeners, listener);
            if (iterator != m_listeners.end()) {
                m_listeners.erase(iterator);
            }
        }

     protected:
        auto notifyAll(const Args&... args) -> void {
            std::ranges::for_each(m_listeners,
                                  [... args = static_cast<const Args&>(args)](Listener* listener) { listener->notify(args...); });
        }

        template <typename Pred>
            requires std::is_invocable_r_v<bool, Pred, Listener*>
        auto notifyAllIf(Pred&& pred, const Args&... args) -> void {
            std::ranges::for_each(m_listeners | std::views::filter(pred),
                                  [... args = static_cast<const Args&>(args)](Listener* listener) { listener->notify(args...); });
        }

     private:
        std::list<Listener*> m_listeners;
    };

}  // namespace SiM
