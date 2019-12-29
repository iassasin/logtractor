#include <ctime>
#include <iostream>
#include <string>
#include <array>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/di.hpp>

#include "src/config.hpp"
#include "src/services/logtractor_service.hpp"
#include "src/logger.hpp"

using boost::asio::ip::udp;
namespace asio = boost::asio;
using namespace std::placeholders;
namespace di = boost::di;

std::string make_daytime_string() {
	using namespace std; // For time_t, time and ctime;
	time_t now = time(nullptr);
	return ctime(&now);
}

class udp_server {
public:
	udp_server(boost::asio::io_context &io_context)
			: socket_(io_context, udp::endpoint(udp::v4(), 30013)) {
		start_receive();
	}

private:
	void start_receive() {
		socket_.async_receive_from(
				boost::asio::buffer(recv_buffer_), remote_endpoint_,
				std::bind(&udp_server::handle_receive, this, _1, _2));
	}

	void handle_receive(const boost::system::error_code &error, std::size_t bytes_transferred) {
		if (!error) {
			auto message = std::make_shared<std::string>(make_daytime_string());

			socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
					std::bind(&udp_server::handle_send, this, message, _1, _2));

			start_receive();
		}
	}

	void handle_send(std::shared_ptr<std::string> message,
			const boost::system::error_code & error,
			std::size_t bytes_transferred) {
	}

	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	std::array<char, 1> recv_buffer_;
};

int main() {
	try {
		auto config = loadConfig("./logtractor.yml");

		boost::asio::io_context io_context;
		Logger logger;

		auto injector = di::make_injector(
			di::bind<Config>.to(config),
			di::bind<boost::asio::io_context>.to(io_context),
			di::bind<Logger>.to(logger)
		);

		auto logtractor = std::move(injector.create<LogtractorService>());

		io_context.run();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}