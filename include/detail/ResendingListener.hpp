#pragma once

#include "Notifier.hpp"

namespace SiM::Logic::Server {

    class Server;

}

namespace SiM::Logic::Server::Detail {

    /**
     * @brief
     *
     * @todo Make thread safe
     */

    class ResendingListener final : public SiM::Notifier<std::string>::Listener {
     public:
        ResendingListener(Server& server);
        auto notify(const std::string& serializedMessuage) -> void override;

     private:
        Server& m_server;
    };

}  // namespace SiM::Logic::Server::Detail
