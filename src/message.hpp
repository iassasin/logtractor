//
// Created by assasin on 01.01.2020.
//

#ifndef LOGTRACTOR_MESSAGE_HPP
#define LOGTRACTOR_MESSAGE_HPP

#include <string>
#include <exception>
#include <type_traits>
#include <stdexcept>

class Message {
private:
	std::string type;
	std::string content;
public:
	Message() = default;
	Message(std::string msg) : content(std::move(msg)) {}

	template <typename T>
	T &as() {
		if constexpr (std::is_same_v<std::string, T>) {
			return content;
		}

		throw std::bad_cast();
	}

	std::string toString() {
		return content;
	}

	template <typename T>
	void setMessage(T &&msg) {
		using DT = std::remove_reference_t<T>;

		if constexpr (std::is_same_v<DT, std::string>) {
			content = std::forward<decltype(msg)>(msg);
			return;
		}

		throw std::runtime_error("Unknown type for message");
	}
};

#endif //LOGTRACTOR_MESSAGE_HPP
