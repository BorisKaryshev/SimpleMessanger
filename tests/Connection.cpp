#include "TCP.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <list>
#include <optional>
#include <string>
#include <thread>

namespace {

    constexpr int port = 8080;
    constexpr std::string_view stopLiteral = "stop";

}  // namespace

namespace {

    template <typename... Args>
    auto print(const Args&... args) -> void {
        std::stringstream stream;
        (stream << ... << args);
        std::cout << stream.str();
    }

    class Store : public SiM::Notifier<std::string>::Listener, public SiM::Connection {
     public:
        Store(boost::asio::ip::tcp::socket sock) : SiM::Connection(std::move(sock)), m_messages() { addListener(this); }

        auto notify(const std::string& message) -> void override {
            std::lock_guard lock(m_listPushAndPop);
            std::cout << "Got message: " << message << "\n";
            m_messages.push_back(message);
        }

        [[nodiscard]] auto lastMessage() const -> std::string {
            std::lock_guard lock(m_listPushAndPop);
            return m_messages.back();
        }

        [[nodiscard]] auto messages() const noexcept -> const std::list<std::string>& { return m_messages; }

     private:
        mutable std::mutex m_listPushAndPop;
        std::list<std::string> m_messages;
    };

    class Server {
     public:
        Server(boost::asio::io_context& context) : m_endpoint(boost::asio::ip::tcp::v4(), port), m_acceptor(context, m_endpoint) {}

        auto accept() -> void {
            auto helper = [this]([[maybe_unused]] const boost::system::error_code& ec, boost::asio::ip::tcp::socket sock) {
                if (ec) {
                    print("Error while accepting connection: ", ec, "\n");
                } else if (!m_connection.has_value()) {
                    m_connection.emplace(std::move(sock));
                    m_connection->run();
                }
            };

            m_acceptor.async_accept(helper);
        }

        auto acceptedMessages() -> std::list<std::string> {
            if (m_connection) {
                return m_connection->messages();
            }
            return {};
        }

        auto stopConnection() -> void {
            if (m_connection) {
                m_connection->stop();
            }
        }

        [[nodiscard]] auto isRunning() const noexcept -> bool {
            if (m_connection) {
                return m_connection->isRunning();
            }
            return false;
        }
        auto waitForStopLiteral() -> void {
            while (m_connection && m_connection->lastMessage() != stopLiteral) {
            }
            m_connection->stop();
        }

     private:
        boost::asio::ip::tcp::endpoint m_endpoint;
        boost::asio::ip::tcp::acceptor m_acceptor;

        std::optional<Store> m_connection;
    };

    auto workingThread(boost::asio::io_context& context) -> void {
        context.run();
    }

}  // namespace

using std::string_literals::operator""s;

TEST(First, Some) {
    boost::asio::io_context clientContext;
    boost::asio::io_context serverContext;

    Server server(serverContext);
    server.accept();

    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), port);
    boost::asio::ip::tcp::socket sock(clientContext);
    sock.connect(ep);
    SiM::Connection con(std::move(sock));
    con.run();

    std::list<std::string> messages{"First"s,
                                    "second"s,
                                    "Third"s,
                                    "Another long one message with some info. Hope it will be useful to you, dear friend"s,
                                    std::string(2000, 'a'),
                                    std::string(stopLiteral)};

    std::jthread th{[&serverContext] { workingThread(serverContext); }};

    for (const auto& message : messages) {
        print("Sending message: ", message, "\n");
        con.send(message);
    }

    con.stop();
    server.waitForStopLiteral();

    ASSERT_EQ(messages, server.acceptedMessages());
}
