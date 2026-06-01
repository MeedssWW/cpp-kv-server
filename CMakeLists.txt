cmake_minimum_required(VERSION 3.20)

project(cpp_kv_server VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    cpp_httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.18.3
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(cpp_httplib nlohmann_json)

add_library(kv_core
    src/config.cpp
    src/file_store.cpp
    src/key_validator.cpp
)

target_include_directories(kv_core PUBLIC include)
target_link_libraries(kv_core PUBLIC nlohmann_json::nlohmann_json)

target_compile_options(kv_core PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_executable(kv_server
    src/main.cpp
    src/http_server.cpp
)

target_link_libraries(kv_server PRIVATE kv_core httplib::httplib)

target_compile_options(kv_server PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

enable_testing()

add_executable(kv_tests
    tests/store_tests.cpp
)

target_link_libraries(kv_tests PRIVATE kv_core)

target_compile_options(kv_tests PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_test(NAME kv_tests COMMAND kv_tests)
