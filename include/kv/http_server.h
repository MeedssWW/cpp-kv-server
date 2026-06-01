#pragma once

#include "kv/config.h"
#include "kv/file_store.h"

namespace kv {

int RunServer(const Config& config, FileStore& store);

}  // namespace kv
