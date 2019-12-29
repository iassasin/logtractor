//
// Created by assasin on 28.12.2019.
//

#include "config.hpp"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace YAML {

template<>
struct convert<SourceConfig> {
	static Node encode(const SourceConfig &src) {
		return {};
	}

	static bool decode(const Node &node, SourceConfig &target) {
		if (!node.IsMap()) {
			return false;
		}

		target.type = node["type"].as<std::string>();

		if (target.type == "udp") {
			SourceUdpConfig config;

			config.appenders = node["appenders"].as<decltype(config.appenders)>();
			config.address = node["address"].as<std::string>();
			config.port = node["port"].as<int>();

			target.config = config;
		} else {
			return false;
		}

		return true;
	}
};

template<>
struct convert<AppenderConfig> {
	static Node encode(const AppenderConfig &src) {
		return {};
	}

	static bool decode(const Node &node, AppenderConfig &target) {
		if (!node.IsMap()) {
			return false;
		}

		target.type = node["type"].as<std::string>();

		if (target.type == "file") {
			AppenderFileConfig config;
			config.filename = node["filename"].as<std::string>();
			target.config = config;
		} else {
			return false;
		}

		return true;
	}
};

template<>
struct convert<Config> {
	static Node encode(const Config &src) {
		return {};
	}

	static bool decode(const Node &node, Config &target) {
		if (!node.IsMap()) {
			return false;
		}

		if (node["sources"]) {
			target.sources = node["sources"].as< decltype(target.sources)>();
		}

		if (node["appenders"]) {
			target.appenders = node["appenders"].as<decltype(target.appenders)>();
		}

		return true;
	}
};

}

Config loadConfig(const std::string &filename) {
	std::ifstream file(filename);
	auto node = YAML::Load(file);
	return node.as<Config>();
}