#include "Message.hpp"
#include "TCP.hpp"
#include "detail/AsyncPrint.hpp"
#include "detail/LowLevelMessage.hpp"
#include "logic/Client.hpp"

#include <boost/asio.hpp>

#include <forward_list>

class Print final : public SiM::Notifier<std::string>::Listener {
 public:
    auto notify(const std::string& serializedMessage) -> void override {
        SiM::Message message(serializedMessage);
        print(std::cout, "Got message from '", message.from(), "'", " to '", message.to(), "': ", message.text(), "\n");
    }
};

namespace {

    constexpr unsigned short port = 8080;

}

class Acceptor {
 public:
    Acceptor(boost::asio::io_context& context) : m_ep(boost::asio::ip::tcp::v4(), port), m_ac(context, m_ep) { m_accept(); }

 private:
    auto m_accept() -> void {
        auto handler = [this]([[maybe_unused]] const boost::system::error_code&, boost::asio::ip::tcp::socket sock) {
            m_connection.emplace_front(SiM::Launch::RunManually, std::move(sock));
            m_connection.front().addListener(std::addressof(m_printer));
            m_connection.front().run();

            m_accept();
        };

        m_ac.async_accept(handler);
    }

 private:
    boost::asio::ip::tcp::endpoint m_ep;
    boost::asio::ip::tcp::acceptor m_ac;
    Print m_printer;

    std::forward_list<SiM::Connection> m_connection;
};

[[nodiscard]] auto subMain() -> int {
#if defined(SERVER)
    boost::asio::io_context context;

    Acceptor ac{context};
    context.run();
#elif defined(CLIENT)
    SiM::Client client(boost::asio::ip::address::from_string("127.0.0.1"), port);
    client.run();

#endif

    return 0;
}

auto main() -> int {
    return subMain();
}
