#include "kv/http_server.h"

#include "kv/key_validator.h"

#include <iostream>

#include <httplib.h>
#include <nlohmann/json.hpp>

namespace kv {

namespace {

void SetJson(httplib::Response& response, const nlohmann::json& body, int status = 200) {
    response.status = status;
    response.set_content(body.dump(), "application/json");
}

}  // namespace

int RunServer(const Config& config, FileStore& store) {
    httplib::Server server;

    server.Get("/health", [](const httplib::Request&, httplib::Response& response) {
        SetJson(response, {{"status", "ok"}});
    });

    server.Get("/kv", [&store](const httplib::Request&, httplib::Response& response) {
        SetJson(response, {{"items", store.Snapshot()}});
    });

    server.Get(R"(/kv/([A-Za-z0-9._-]+))", [&store](const httplib::Request& request, httplib::Response& response) {
        const std::string key = request.matches[1];
        if (auto value = store.Get(key)) {
            SetJson(response, {{"key", key}, {"value", *value}});
            return;
        }

        SetJson(response, {{"error", "key not found"}, {"key", key}}, 404);
    });

    server.Put(R"(/kv/([A-Za-z0-9._-]+))", [&store](const httplib::Request& request, httplib::Response& response) {
        const std::string key = request.matches[1];
        if (!IsValidKey(key)) {
            SetJson(response, {{"error", "invalid key"}}, 400);
            return;
        }

        try {
            const auto json = nlohmann::json::parse(request.body);
            if (!json.contains("value") || !json.at("value").is_string()) {
                SetJson(response, {{"error", "request body must contain string field 'value'"}}, 400);
                return;
            }

            const std::string value = json.at("value").get<std::string>();
            store.Put(key, value);
            SetJson(response, {{"key", key}, {"value", value}}, 201);
        } catch (const nlohmann::json::exception&) {
            SetJson(response, {{"error", "invalid json"}}, 400);
        }
    });

    server.Delete(R"(/kv/([A-Za-z0-9._-]+))", [&store](const httplib::Request& request, httplib::Response& response) {
        const std::string key = request.matches[1];
        if (store.Erase(key)) {
            SetJson(response, {{"deleted", true}, {"key", key}});
            return;
        }

        SetJson(response, {{"deleted", false}, {"error", "key not found"}, {"key", key}}, 404);
    });

    std::cout << "kv_server listening on " << config.host << ':' << config.port << '\n';
    if (!server.listen(config.host, config.port)) {
        std::cerr << "failed to listen on " << config.host << ':' << config.port << '\n';
        return 1;
    }

    return 0;
}

}  // namespace kv
