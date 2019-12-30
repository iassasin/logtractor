#include <ctime>
#include <iostream>
#include <string>
#include <array>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/di.hpp>

#include "src/config.hpp"
#include "src/services/logtractor_service.hpp"
#include "src/logger.hpp"

namespace di = boost::di;

int main() {
	try {
		auto config = loadConfig("./logtractor.yml");

		boost::asio::io_context io_context;
		Logger logger;

		auto injector = di::make_injector(
			di::bind<Config>.to(config),
			di::bind<boost::asio::io_context>.to(io_context),
			di::bind<Logger>.to(logger)
		);

		auto logtractor = std::move(injector.create<LogtractorService>());
		logtractor.init();

		io_context.run();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}