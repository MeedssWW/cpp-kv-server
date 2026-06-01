#include "kv/config.h"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

namespace kv {

Config LoadConfig(const std::filesystem::path& path) {
    Config config;

    if (path.empty() || !std::filesystem::exists(path)) {
        return config;
    }

    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("failed to open config file: " + path.string());
    }

    nlohmann::json json;
    input >> json;

    if (json.contains("host")) {
        config.host = json.at("host").get<std::string>();
    }
    if (json.contains("port")) {
        config.port = json.at("port").get<int>();
    }
    if (json.contains("data_file")) {
        config.data_file = json.at("data_file").get<std::string>();
    }

    return config;
}

}  // namespace kv
