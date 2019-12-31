//
// Created by assasin on 29.12.2019.
//

#include "appender_file.hpp"

#include <iostream>

void AppenderFile::processMessage(std::shared_ptr<std::string> message) {
	writeQueue.push(message);

	if (!writeInProgress) {
		writeInProgress = true;
		processWrite();
	}
}

void AppenderFile::processWrite() {
	auto msg = writeQueue.front();
	writeQueue.pop();

	boost::asio::async_write(file, boost::asio::buffer(*msg),
		[this, ptr = shared_from_this(), msg] (const boost::system::error_code &error, std::size_t bytesWritten) {
			if (error) {
				logger.error("[AppenderFile] Error: ", error, ". For message: ", *msg);
			}

			if (bytesWritten != msg->size()) {
				logger.warn("[AppenderFile] Written ", bytesWritten, " / ", msg->size(), " bytes");
			}

			if (!writeQueue.empty()) {
				processWrite();
			} else {
				writeInProgress = false;
			}
		}
	);
}