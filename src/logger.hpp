//
// Created by assasin on 29.12.10.
//

#ifndef LOGTRACTOR_LOGGER_HPP
#define LOGTRACTOR_LOGGER_HPP

#include <iostream>

class Logger {
public:
	template<typename ...Args>
	void log(Args && ...args) {
		(std::cout << ... << std::forward<decltype(args)>(args)) << std::endl;
	}

	template<typename ...Args>
	void error(Args && ...args) {
		std::cerr << "[ERROR] " << (std::forward<decltype(args)>(args), ...) << std::endl;
	}

	template<typename ...Args>
	void warn(Args && ...args) {
		log("[WARNING] ", std::forward<decltype(args)>(args)...);
	}

	template<typename ...Args>
	void info(Args && ...args) {
		log("[INFO] ", std::forward<decltype(args)>(args)...);
	}

	template<typename ...Args>
	void debug(Args && ...args) {
		log("[DEBUG] ", std::forward<decltype(args)>(args)...);
	}
};

#endif //LOGTRACTOR_LOGGER_HPP
