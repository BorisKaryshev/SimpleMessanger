#include "TCP.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <fstream>
#include <list>
#include <optional>
#include <string>
#include <thread>

namespace {

    constexpr int port = 7354;

}  // namespace

namespace {

    std::ofstream logger("log.txt");

    template <typename... Args>
    auto print(const Args&... args) -> void {
        std::stringstream stream;
        (stream << ... << args);
        logger << stream.str();
    }

    class StoreMessages : public SiM::Connection, public SiM::Notifier<std::string>::Listener {
     public:
        explicit StoreMessages(boost::asio::ip::tcp::socket sock) : SiM::Connection(SiM::Launch::RunManually, std::move(sock)) {
            addListener(this);
            run();
        }

        auto notify(std::string message) -> void override {
            std::lock_guard lock(m_messagesListOperations);
            print("Got message: |", message, "|\n");
            m_messages.emplace_back(std::move(message));
        }

        [[nodiscard]] auto getMessages() const -> const std::list<std::string>& {
            std::lock_guard lock(m_messagesListOperations);
            return m_messages;
        }

     private:
        mutable std::mutex m_messagesListOperations;
        std::list<std::string> m_messages;
    };

    class AcceptConnection {
     public:
        explicit AcceptConnection(boost::asio::io_context& context)
            : m_endpoint(boost::asio::ip::tcp::v4(), port), m_acceptor(context, m_endpoint), m_connection(std::nullopt) {
            auto handler = [this](const boost::system::error_code& errorCode, boost::asio::ip::tcp::socket sock) {
                if (errorCode) {
                    print("ERROR AcceptConnections ", __FILE__, ":", __LINE__, " : ", errorCode.message(), "\n");
                } else if (!m_connection) {
                    m_connection.emplace(std::move(sock));
                }
            };

            m_acceptor.async_accept(handler);
        }

        [[nodiscard]] auto acceptedMessages() const noexcept -> std::list<std::string> {
            if (m_connection) {
                return m_connection->getMessages();
            }
            return {};
        }

     private:
        boost::asio::ip::tcp::endpoint m_endpoint;
        boost::asio::ip::tcp::acceptor m_acceptor;

        std::optional<StoreMessages> m_connection;
    };

    auto workingThread(boost::asio::io_context& context) -> void {
        context.run();
    }

    auto testSendingAndReceivingMessage(const std::list<std::string>& messagesToSend) -> std::list<std::string> {
        static std::mutex testMutex;
        std::lock_guard lock(testMutex);

        boost::asio::io_context context;

        AcceptConnection server(context);

        std::thread worker{[&context] { workingThread(context); }};

        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port);
        boost::asio::ip::tcp::socket sock(context);
        sock.connect(endpoint);

        SiM::Connection connection(SiM::Launch::RunAtConstruction, std::move(sock));

        print("Messages to send\n");
        std::ranges::for_each(messagesToSend, [](const std::string& message) { print("\t|", message, "|\n"); });

        for (const auto& message : messagesToSend) {
            print("Sending message: |", message, "|\n");
            connection.send(message);
        }

        connection.close();
        worker.join();

        return server.acceptedMessages();
    }

}  // namespace

using std::string_literals::operator""s;

using namespace std::chrono_literals;

TEST(ClientServerInteraction, SimpleMessages) {
    std::list<std::string> messages{"Hi, this is the simplest message"s, "More text"s, std::string(1000, 'a')};

    EXPECT_EQ(messages, testSendingAndReceivingMessage(messages));
}

TEST(ClientServerInteraction, EmptyMessages) {
    std::list<std::string> messages{""s, "Hi, this is the simplest message"s, ""s, "More text"s, ""s, std::string(1000, 'a')};

    EXPECT_EQ(messages, testSendingAndReceivingMessage(messages));
}
