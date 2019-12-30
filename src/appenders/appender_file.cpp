//
// Created by assasin on 29.12.2019.
//

#include "appender_file.hpp"

#include <iostream>

void AppenderFile::processMessage(std::shared_ptr<std::string> message) {
	std::cout << "[appender-file] " << *message << std::endl;
}