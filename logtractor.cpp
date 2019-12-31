#include <string>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/di.hpp>
#include <boost/program_options.hpp>

#include "src/config.hpp"
#include "src/services/logtractor_service.hpp"
#include "src/logger.hpp"

namespace di = boost::di;
namespace po = boost::program_options;

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

		io_context.run();
	} catch (std::exception &e) {
		logger.error("Fatal error: ", e);
		return 1;
	}

	return 0;
}