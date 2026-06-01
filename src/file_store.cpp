#include "kv/file_store.h"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

namespace kv {

FileStore::FileStore(std::filesystem::path path)
    : path_(std::move(path)) {
}

void FileStore::Load() {
    std::lock_guard lock(mutex_);

    data_.clear();
    if (!std::filesystem::exists(path_)) {
        return;
    }

    std::ifstream input(path_);
    if (!input) {
        throw std::runtime_error("failed to open data file: " + path_.string());
    }

    nlohmann::json json;
    input >> json;
    data_ = json.get<std::unordered_map<std::string, std::string>>();
}

void FileStore::Flush() const {
    std::lock_guard lock(mutex_);

    if (path_.has_parent_path()) {
        std::filesystem::create_directories(path_.parent_path());
    }

    std::ofstream output(path_);
    if (!output) {
        throw std::runtime_error("failed to write data file: " + path_.string());
    }

    output << nlohmann::json(data_).dump(2) << '\n';
}

bool FileStore::Put(std::string key, std::string value) {
    {
        std::lock_guard lock(mutex_);
        data_[std::move(key)] = std::move(value);
    }

    Flush();
    return true;
}

std::optional<std::string> FileStore::Get(const std::string& key) const {
    std::lock_guard lock(mutex_);

    if (auto it = data_.find(key); it != data_.end()) {
        return it->second;
    }

    return std::nullopt;
}

bool FileStore::Erase(const std::string& key) {
    bool removed = false;
    {
        std::lock_guard lock(mutex_);
        removed = data_.erase(key) > 0;
    }

    if (removed) {
        Flush();
    }

    return removed;
}

std::unordered_map<std::string, std::string> FileStore::Snapshot() const {
    std::lock_guard lock(mutex_);
    return data_;
}

std::size_t FileStore::Size() const {
    std::lock_guard lock(mutex_);
    return data_.size();
}

}  // namespace kv
