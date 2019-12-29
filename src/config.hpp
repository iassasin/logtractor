//
// Created by assasin on 28.12.2019.
//

#ifndef LOGTRACTOR_CONFIG_HPP
#define LOGTRACTOR_CONFIG_HPP

#include <string>
#include <variant>
#include <map>
#include <vector>

//appenders

struct AppenderFileConfig {
	std::string filename;
};

struct AppenderConfig {
	std::string type;
	std::variant<std::monostate, AppenderFileConfig> config;
};

// sources

struct SourceUdpConfig {
	std::string address;
	int port;
	std::vector<std::string> appenders;
};

struct SourceConfig {
	std::string type;
	std::variant<std::monostate, SourceUdpConfig> config;
};

// base config

struct Config {
	std::map<std::string, SourceConfig> sources;
	std::map<std::string, AppenderConfig> appenders;
};

Config loadConfig(const std::string &filename);

#endif //LOGTRACTOR_CONFIG_HPP
