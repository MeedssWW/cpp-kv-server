#include "kv/key_validator.h"

#include <algorithm>
#include <cctype>

namespace kv {

bool IsValidKey(const std::string& key) {
    if (key.empty() || key.size() > 128) {
        return false;
    }

    return std::all_of(key.begin(), key.end(), [](unsigned char ch) {
        return std::isalnum(ch) || ch == '_' || ch == '-' || ch == '.';
    });
}

}  // namespace kv
