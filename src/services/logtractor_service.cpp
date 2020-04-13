//
// Created by assasin on 28.12.2019.
//

#include "logtractor_service.hpp"
#include "../sources/source_udp.hpp"
#include "../appenders/appender_file.hpp"

#include <exception>

void LogtractorService::init() {
	for (auto &&[appenderName, appender] : config.appenders) {
		if (appender.type == "file") {
			auto appConf = std::get<AppenderFileConfig>(appender.config);

			appenders.insert({appenderName, std::make_shared<AppenderFile>(io, appConf.filename, logger)});

			logger.info("Added appender '", appenderName, "' with type '", appender.type, "'");
		} else {
			logger.error("Unknown appender type: '", appender.type, "'");
			throw std::runtime_error("Unknown appender type");
		}
	}

	for (auto &&[sourceName, source] : config.sources) {
		if (source.type == "udp") {
			auto srcConf = std::get<SourceUdpConfig>(source.config);

			for (auto &&appenderName : srcConf.appenders) {
				if (appenders.find(appenderName) == appenders.end()) {
					logger.error("Unknown appender '", appenderName, "' for source '", sourceName, "'");
					throw std::runtime_error("Unknown appender");
				}
			}

			auto sourceObj = std::make_shared<SourceUdp>(io, srcConf.address, srcConf.port, logger,
				[this, srcConf] (const std::shared_ptr<Message> &message) {
					for (auto &&appenderName : srcConf.appenders) {
						auto &appender = *appenders[appenderName];
						appender.processMessage(message);
					}
				}
			);

			sourceObj->startReceive();

			sources.insert({sourceName, std::move(sourceObj)});

			logger.info("Added source '", sourceName, "' with type '", source.type, "'");
		} else {
			logger.error("Unknown source type: '", source.type, "'");
			throw std::runtime_error("Unknown source type");
		}
	}
}

void LogtractorService::reinit() {
	for (auto &[_, source] : sources) {
		source->stopReceive();
	}

	appenders.clear();
	sources.clear();
	init();
}