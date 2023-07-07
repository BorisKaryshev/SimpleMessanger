#pragma once

#include "Notifier.hpp"

namespace SiM {

    template <typename... Args>

    inline auto Notifier<Args...>::addListener(typename Notifier<Args...>::Listener* listener) -> Notifier<Args...>& {
        std::lock_guard lock{m_listenersContainerModification};

        m_listeners.push_front(listener);
        return *this;
    }

    template <typename... Args>
    inline auto Notifier<Args...>::removeListener(typename Notifier<Args...>::Listener* listener) -> Notifier<Args...>& {
        std::lock_guard lock{m_listenersContainerModification};

        m_listeners.remove(listener);
        return *this;
    }

    template <typename... Args>
    inline auto Notifier<Args...>::notifyAll(const Args&... args) -> void {
        std::ranges::for_each(m_listeners, [... args = static_cast<const Args&>(args)](typename Notifier<Args...>::Listener* listener) {
            listener->notify(args...);
        });
    }

    template <typename... Args>
    template <typename Pred>
        requires std::is_invocable_r_v<bool, Pred, typename Notifier<Args...>::Listener*>
    inline auto Notifier<Args...>::notifyAllIf(Pred&& pred, const Args&... args) -> void {
        std::ranges::for_each(
            m_listeners | std::views::filter(pred),
            [... args = static_cast<const Args&>(args)](typename Notifier<Args...>::Listener* listener) { listener->notify(args...); });
    }

};  // namespace SiM
