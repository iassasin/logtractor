#include <string>
#include <iostream>
#include <functional>
#include <map>
#include <csignal>

#include <boost/asio.hpp>
#include <boost/di.hpp>
#include <boost/program_options.hpp>

#include "src/config.hpp"
#include "src/services/logtractor_service.hpp"
#include "src/logger.hpp"

namespace di = boost::di;
namespace po = boost::program_options;

static std::map<int, std::function<void(int)>> signals;

static void signalHandler(int signum) {
	auto handler = signals.find(signum);
	if (handler != signals.end()) {
		handler->second(signum);
	}
}

static void handleSignal(Logger &logger, int signum, std::function<void(int)> f) {
	signals.emplace(signum, std::move(f));

	if (signal(signum, signalHandler) == SIG_ERR) {
		logger.error("Can't set signal ", SIGUSR1, ". Aborting.");
		throw std::runtime_error("Signal set failed");
	}
}

int main(int argc, char **argv) {
	Logger logger;

	try {
		// TODO: replace boost::program_options with something else, it's x2 binary size (500k -> 1M)
		po::variables_map vm;

		po::options_description optionsDescription("Options");
		optionsDescription.add_options()
				("help,h", "print this message")
				("config", po::value<std::string>()->default_value("logtractor.yml"), "set config file");

		po::store(po::parse_command_line(argc, argv, optionsDescription), vm);
		po::notify(vm);

		if (vm.count("help")) {
			std::cout << "logtractor [options]\n" << std::endl;
			std::cout << optionsDescription << std::endl;
			return 0;
		}

		auto config = loadConfig(vm["config"].as<std::string>());

		boost::asio::io_context io_context;

		auto injector = di::make_injector(
			di::bind<Config>.to(config),
			di::bind<boost::asio::io_context>.to(io_context),
			di::bind<Logger>.to(logger)
		);

		auto logtractor = std::move(injector.create<LogtractorService>());
		logtractor.init();

		handleSignal(logger, SIGUSR1, [&] (int) {
			logger.info("USR1 signal received. Reloading config");
			config = std::move(loadConfig(vm["config"].as<std::string>()));
			logtractor.reinit();
		});

		io_context.run();
	} catch (std::exception &e) {
		logger.error("Fatal error: ", e);
		return 1;
	}

	return 0;
}