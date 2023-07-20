#pragma once

#include <algorithm>
#include <concepts>
#include <forward_list>
#include <mutex>
#include <ranges>

#include <iostream>
#include <sstream>
#include <thread>

namespace SiM {

    /**
     * @details Notifier is thread safe.
     */
    template <typename... Args>
    class Notifier {
     public:
        class Listener {
         public:
            virtual constexpr auto notify(const Args&... message) -> void = 0;

            virtual ~Listener() = default;
        };

     public:
        auto addListener(Listener* listener) -> Notifier& {
            std::lock_guard lock{m_listenersContainerModification};

            m_listeners.push_front(listener);
            return *this;
        }

        auto removeListener(Listener* listener) -> Notifier& {
            std::lock_guard lock{m_listenersContainerModification};

            m_listeners.remove(listener);
            return *this;
        }

     protected:
        auto notifyAll(Args... args) -> void {
            std::lock_guard lock(m_listenersContainerModification);
            std::ranges::for_each(m_listeners, [... args = static_cast<Args&&>(args)](typename Notifier<Args...>::Listener* listener) {
                listener->notify(args...);
            });
        }

        template <typename Pred>
            requires std::is_invocable_r_v<bool, Pred, typename Notifier<Args...>::Listener*>
        auto notifyAllIf(Pred&& pred, const Args&... args) -> void {
            std::lock_guard lock(m_listenersContainerModification);
            std::ranges::for_each(
                m_listeners | std::views::filter(pred),
                [... args = static_cast<const Args&>(args)](typename Notifier<Args...>::Listener* listener) { listener->notify(args...); });
        }

     private:
        std::mutex m_listenersContainerModification;
        std::forward_list<Listener*> m_listeners;
    };

}  // namespace SiM
