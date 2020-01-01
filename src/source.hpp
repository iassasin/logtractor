//
// Created by assasin on 29.12.2019.
//

#ifndef LOGTRACTOR_SOURCE_HPP
#define LOGTRACTOR_SOURCE_HPP

#include "message.hpp"

#include <memory>
#include <functional>

class Source : public std::enable_shared_from_this<Source> {
protected:
	std::function<void(std::shared_ptr<Message>)> callback;

	explicit Source(decltype(callback) &&cbk) : enable_shared_from_this(), callback(std::move(cbk)) {}
public:
	virtual ~Source() = default;
	virtual void startReceive() = 0;
};


#endif //LOGTRACTOR_SOURCE_HPP
