#pragma once

#include <filesystem>
#include <string>

namespace kv {

struct Config {
    std::string host = "0.0.0.0";
    int port = 8080;
    std::filesystem::path data_file = "data/store.json";
};

Config LoadConfig(const std::filesystem::path& path);

}  // namespace kv
