//
// Created by assasin on 29.12.2019.
//

#ifndef LOGTRACTOR_APPENDER_HPP
#define LOGTRACTOR_APPENDER_HPP

#include "message.hpp"

#include <string>
#include <memory>
#include <functional>

class Appender : public std::enable_shared_from_this<Appender> {
protected:

public:
	virtual ~Appender() = default;
	virtual void processMessage(std::shared_ptr<Message> message) = 0;
};


#endif //LOGTRACTOR_APPENDER_HPP
