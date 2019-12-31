//
// Created by assasin on 29.12.2019.
//

#ifndef LOGTRACTOR_APPENDER_FILE_HPP
#define LOGTRACTOR_APPENDER_FILE_HPP

#include "../appender.hpp"
#include "../logger.hpp"

#include <fcntl.h>
#include <boost/asio.hpp>
#include <exception>
#include <queue>

class AppenderFile : public Appender {
private:
	boost::asio::io_context &io;
	std::string filename;
	Logger &logger;

	boost::asio::posix::stream_descriptor file;
	std::queue<std::shared_ptr<std::string>> writeQueue;
	bool writeInProgress;

	void processWrite();
public:
	AppenderFile(boost::asio::io_context &context, std::string _filename, Logger &_logger)
		: Appender(), io(context), filename(std::move(_filename)), file(context), writeInProgress(false),
		logger(_logger)
	{
		int fd = open(filename.c_str(), O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

		if (fd == -1) {
			throw std::runtime_error("Can't open file for appender [ERRNO: " + std::to_string(errno) + "]: " + filename);
		}

		file.assign(fd);
	}

	void processMessage(std::shared_ptr<std::string> message) override;
};


#endif //LOGTRACTOR_APPENDER_FILE_HPP
