//
// Created by assasin on 28.12.2019.
//

#include "logtractor_service.hpp"

void LogtractorService::initSources() {
	for (auto &&[sourceName, source] : config.sources) {

		logger.info("Added source '", sourceName, "'");
	}
}