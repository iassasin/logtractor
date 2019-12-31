//
// Created by assasin on 29.12.10.
//

#ifndef LOGTRACTOR_LOGGER_HPP
#define LOGTRACTOR_LOGGER_HPP

#include <type_traits>
#include <iostream>
#include <boost/system/error_code.hpp>
#include <exception>

class Logger {
private:
	template<typename T>
	void logArg(std::ostream &stream, T &&arg) {
		using DT = std::remove_reference_t<T>;

		if constexpr (std::is_base_of_v<boost::system::error_code, DT>) {
			const boost::system::error_code &error = arg;
			stream << error.message() << " [" << error.category().name() << ":" << error.value() << "]";
		} else if constexpr (std::is_base_of_v<std::exception, DT>) {
			const std::exception &ex = arg;
			stream << "[exception] " << ex.what();
		} else {
			stream << arg;
		}
	}
public:
	template<typename ...Args>
	void log(Args && ...args) {
		(logArg(std::cout, std::forward<decltype(args)>(args)), ...);
		std::cout << std::endl;
	}

	template<typename ...Args>
	void error(Args && ...args) {
		std::cerr << "[ERROR] ";
		(logArg(std::cerr, std::forward<decltype(args)>(args)), ...);
		std::cerr << std::endl;
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
