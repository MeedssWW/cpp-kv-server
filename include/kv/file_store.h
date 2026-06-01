#pragma once

#include <filesystem>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

namespace kv {

class FileStore {
public:
    explicit FileStore(std::filesystem::path path);

    void Load();
    void Flush() const;

    bool Put(std::string key, std::string value);
    std::optional<std::string> Get(const std::string& key) const;
    bool Erase(const std::string& key);
    std::unordered_map<std::string, std::string> Snapshot() const;
    std::size_t Size() const;

private:
    std::filesystem::path path_;
    mutable std::mutex mutex_;
    std::unordered_map<std::string, std::string> data_;
};

}  // namespace kv
