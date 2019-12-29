//
// Created by assasin on 28.12.2019.
//

#ifndef LOGTRACTOR_LOGTRACTOR_SERVICE_HPP
#define LOGTRACTOR_LOGTRACTOR_SERVICE_HPP

#include <boost/asio.hpp>

#include "../config.hpp"
#include "../logger.hpp"

class LogtractorService {
private:
	boost::asio::io_context &io;
	Config &config;
	Logger &logger;

	void initSources();
public:
	LogtractorService(boost::asio::io_context &context, Config &_config, Logger &_logger)
		: io(context), config(_config), logger(_logger)
	{
		initSources();
	}
};


#endif //LOGTRACTOR_LOGTRACTOR_SERVICE_HPP
