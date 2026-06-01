#include "kv/file_store.h"
#include "kv/key_validator.h"

#include <cassert>
#include <filesystem>
#include <iostream>

namespace {

std::filesystem::path TestFile() {
    return std::filesystem::temp_directory_path() / "cpp_kv_server_store_tests.json";
}

void TestKeyValidation() {
    assert(kv::IsValidKey("user-1"));
    assert(kv::IsValidKey("user.name_1"));
    assert(!kv::IsValidKey(""));
    assert(!kv::IsValidKey("bad/key"));
    assert(!kv::IsValidKey("bad key"));
}

void TestPutGetDelete() {
    const auto path = TestFile();
    std::filesystem::remove(path);

    kv::FileStore store(path);
    store.Load();

    assert(store.Size() == 0);
    assert(!store.Get("name"));

    store.Put("name", "Pavel");
    assert(store.Size() == 1);
    assert(store.Get("name").value() == "Pavel");

    store.Put("name", "Codex");
    assert(store.Get("name").value() == "Codex");

    assert(store.Erase("name"));
    assert(!store.Get("name"));
    assert(!store.Erase("name"));

    std::filesystem::remove(path);
}

void TestPersistence() {
    const auto path = TestFile();
    std::filesystem::remove(path);

    {
        kv::FileStore store(path);
        store.Load();
        store.Put("language", "cpp");
    }

    {
        kv::FileStore store(path);
        store.Load();
        assert(store.Get("language").value() == "cpp");
    }

    std::filesystem::remove(path);
}

}  // namespace

int main() {
    TestKeyValidation();
    TestPutGetDelete();
    TestPersistence();

    std::cout << "All kv tests passed\n";
    return 0;
}
