#include "kv/config.h"
#include "kv/file_store.h"
#include "kv/http_server.h"

#include <exception>
#include <filesystem>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        std::filesystem::path config_path;
        if (argc > 1) {
            config_path = argv[1];
        }

        kv::Config config = kv::LoadConfig(config_path);
        kv::FileStore store(config.data_file);
        store.Load();

        return kv::RunServer(config, store);
    } catch (const std::exception& error) {
        std::cerr << "fatal error: " << error.what() << '\n';
        return 1;
    }
}
