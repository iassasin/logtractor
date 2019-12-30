//
// Created by assasin on 29.12.2019.
//

#ifndef LOGTRACTOR_APPENDER_FILE_HPP
#define LOGTRACTOR_APPENDER_FILE_HPP

#include "../appender.hpp"

#include <boost/asio.hpp>

class AppenderFile : public Appender {
private:
	boost::asio::io_context &io;
	std::string filename;
public:
	AppenderFile(boost::asio::io_context &context, std::string _filename)
		: Appender(), io(context), filename(std::move(_filename))
	{

	}

	void processMessage(std::shared_ptr<std::string> message) override;
};


#endif //LOGTRACTOR_APPENDER_FILE_HPP
