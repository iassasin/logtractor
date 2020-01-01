//
// Created by assasin on 29.12.2019.
//

#include "appender_file.hpp"

#include <iostream>

void AppenderFile::processMessage(std::shared_ptr<Message> message) {
	writeQueue.push(message);

	if (!writeInProgress) {
		writeInProgress = true;
		processWrite();
	}
}

void AppenderFile::processWrite() {
	auto msg = writeQueue.front();
	writeQueue.pop();

	auto &strMsg = msg->as<std::string>();

	boost::asio::async_write(file, boost::asio::buffer(strMsg),
		[this, ptr = shared_from_this(), msg] (const boost::system::error_code &error, std::size_t bytesWritten) {
			auto &strMsg = msg->as<std::string>();

			if (error) {
				logger.error("[AppenderFile] Error: ", error, ". For message: ", strMsg);
			}

			if (bytesWritten != strMsg.size()) {
				logger.warn("[AppenderFile] Written ", bytesWritten, " / ", strMsg.size(), " bytes");
			}

			if (!writeQueue.empty()) {
				processWrite();
			} else {
				writeInProgress = false;
			}
		}
	);
}