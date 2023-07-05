#pragma once

#include <algorithm>
#include <list>
#include <ranges>

namespace SiM {

    template <typename... Args>
    class Notifier {
     public:
        class Listener {
         public:
            virtual auto notify(Args&&... message) -> void = 0;
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
        auto notifyAll(Args...&& args) -> void {
            for (auto& listener : m_listeners) {
                listener->notify(std::forward<Args>(args)...);
            }
        }

     private:
        std::list<Listener*> m_listeners;
    };

}  // namespace SiM
