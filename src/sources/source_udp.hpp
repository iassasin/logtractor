//
// Created by assasin on 29.12.2019.
//

#ifndef LOGTRACTOR_SOURCE_UDP_HPP
#define LOGTRACTOR_SOURCE_UDP_HPP

#include <string>
#include <functional>
#include <boost/asio.hpp>

#include "../source.hpp"
#include "../logger.hpp"

class SourceUdp : public Source {
private:
	using udp = boost::asio::ip::udp;

	std::string listenAddress;
	int listenPort;

	udp::socket socket;
	udp::endpoint remote_endpoint;
	std::array<char, 65536> recv_buffer;
	Logger &logger;
public:
	SourceUdp(boost::asio::io_context &io_context, const std::string &_listenAddress, int port, Logger &_logger,
			decltype(callback) cbk)
			: Source(std::move(cbk)),
			listenAddress(_listenAddress),
			listenPort(port),
			socket(io_context, udp::endpoint(boost::asio::ip::address::from_string(listenAddress), port)),
			logger(_logger)
	{

	}

	void startReceive() override {
		socket.async_receive_from(boost::asio::buffer(recv_buffer), remote_endpoint,
			[this, ptr = shared_from_this()](const boost::system::error_code &error, std::size_t bytesTransferred) {
				if (!error) {
					auto message = std::make_shared<Message>(std::move(std::string(recv_buffer.data(), bytesTransferred)));
					callback(std::move(message));
					startReceive();
				} else {
					if (error.value() == boost::system::errc::operation_canceled || error.value() == boost::system::errc::bad_file_descriptor) {
						logger.info("Cancel listen from [", listenAddress, ':', listenPort, "] - ");
					} else {
						logger.error("Error reading message from [", listenAddress, ':', listenPort, "] - ", error);
						startReceive();
					}
				}
			}
		);
	}

	void stopReceive() override {
		boost::system::error_code error;
		socket.close(error);
	}
};


#endif //LOGTRACTOR_SOURCE_UDP_HPP
