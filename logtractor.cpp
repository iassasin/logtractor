#include <string>

#include <boost/asio.hpp>
#include <boost/di.hpp>

#include "src/config.hpp"
#include "src/services/logtractor_service.hpp"
#include "src/logger.hpp"

namespace di = boost::di;

int main() {
	Logger logger;

	try {
		auto config = loadConfig("./logtractor.yml");

		boost::asio::io_context io_context;

		auto injector = di::make_injector(
			di::bind<Config>.to(config),
			di::bind<boost::asio::io_context>.to(io_context),
			di::bind<Logger>.to(logger)
		);

		auto logtractor = std::move(injector.create<LogtractorService>());
		logtractor.init();

		io_context.run();
	} catch (std::exception &e) {
		logger.error("Fatal error: ", e);
	}

	return 0;
}