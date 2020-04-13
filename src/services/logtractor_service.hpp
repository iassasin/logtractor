//
// Created by assasin on 28.12.2019.
//

#ifndef LOGTRACTOR_LOGTRACTOR_SERVICE_HPP
#define LOGTRACTOR_LOGTRACTOR_SERVICE_HPP

#include <unordered_map>
#include <memory>

#include <boost/asio.hpp>

#include "../config.hpp"
#include "../source.hpp"
#include "../appender.hpp"
#include "../logger.hpp"

class LogtractorService {
private:
	boost::asio::io_context &io;
	Config &config;
	Logger &logger;

	std::unordered_map<std::string, std::shared_ptr<Source>> sources;
	std::unordered_map<std::string, std::shared_ptr<Appender>> appenders;
public:
	LogtractorService(boost::asio::io_context &context, Config &_config, Logger &_logger)
		: io(context), config(_config), logger(_logger)
	{

	}

	void init();
	void reinit();
};


#endif //LOGTRACTOR_LOGTRACTOR_SERVICE_HPP
