#pragma once

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/thread/concurrent_queues/sync_deque.hpp>

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace SiM {

    class Connection : public Notifier<std::string> {
     public:
        Connection(boost::asio::ip::tcp::socket&& sock) : m_sock(std::move(sock)) {}
        Connection(Connection&& other) : m_sock(std::move(other.m_sock)) {}

        auto run() -> void {
            m_isRunning = true;
            m_read();
        }

        auto stop() -> void { m_isRunning = false; }

        [[nodiscard]] auto isRunning() const noexcept -> bool { return m_isRunning; }

        auto send(const std::string& text) -> void {
            if (text.size() > maxMessageSize) {
                throw std::runtime_error("Messages with size more than " + std::to_string(maxMessageSize) + " bytes are not supported");
            }
            m_sock.async_send(boost::asio::buffer(text), []([[maybe_unused]] const boost::system::error_code&, [[maybe_unused]] size_t) {});
        }

        ~Connection() { m_sock.close(); }

     private:
        auto m_read() -> void {
            auto readHandler = [this]([[maybe_unused]] const boost::system::error_code&, [[maybe_unused]] size_t bytes) {
                m_acceptedMessage[bytes] = '\0';
                notifyAll(m_acceptedMessage);

                if (this->m_isRunning) {
                    m_read();
                }
            };

            m_sock.async_read_some(boost::asio::buffer(m_acceptedMessage), readHandler);
        }

     private:
        constexpr static int maxMessageSize = 1 << 12;

     private:
        boost::asio::ip::tcp::socket m_sock;
        std::atomic_bool m_isRunning{true};

        char m_acceptedMessage[maxMessageSize];
    };

    namespace Server {

        namespace Detail {

            class ServerLogic : public Notifier<std::string>::Listener {
             public:
                ServerLogic(Connection& connection) : m_connection(connection) {}

                auto notify(const std::string& message) -> void override {
                    std::cout << "Got message: " << message << "\n";
                    m_connection.send(message);
                }

             private:
                Connection& m_connection;
            };

            struct Connection {
                Connection(SiM::Connection&& conn, int idd) : con(std::move(conn)), logic(con), id(idd) {
                    con.addListener(&logic);
                    con.run();
                }

                Connection(const Connection&) = delete;
                Connection(Connection&& other) : con(std::move(other.con)), logic(con), id(other.id) {}

                SiM::Connection con;
                ServerLogic logic;
                int id;
            };

        }  // namespace Detail

        class Server {
         public:
            Server(boost::asio::io_context& context, int port)
                : m_endpoints(boost::asio::ip::tcp::v4(), port), m_acceptor(context, m_endpoints) {}

            auto runLogic() -> void { m_acceptConnections(); }

         private:
            auto m_acceptConnections() -> void {
                static int id = 0;

                auto handler = [this](const boost::system::error_code& errorCode, boost::asio::ip::tcp::socket sock) {
                    if (!errorCode) {
                        auto newId = ++id;
                        this->m_connections.emplace_back(SiM::Connection(std::move(sock)), newId);
                    }
                    this->m_acceptConnections();
                };

                m_acceptor.async_accept(handler);
            }

         private:
            std::list<Detail::Connection> m_connections;
            boost::asio::ip::tcp::endpoint m_endpoints;
            boost::asio::ip::tcp::acceptor m_acceptor;
        };

    }  // namespace Server

}  // namespace SiM
