# C++ Key-Value Server

[![C++ CI](https://github.com/MeedssWW/cpp-kv-server/actions/workflows/cpp-ci.yml/badge.svg?branch=main)](https://github.com/MeedssWW/cpp-kv-server/actions/workflows/cpp-ci.yml)

C++ Key-Value Server is a small REST service for storing string values by key. It is built as a portfolio-oriented backend project: C++20, CMake, HTTP API, JSON persistence, tests, CI, and Docker.

## Features

- REST API for creating, reading, listing, and deleting keys.
- JSON request and response bodies.
- File-based persistent storage.
- Thread-safe storage access.
- Configurable host, port, and data file path.
- Unit tests for storage and key validation.
- GitHub Actions build, test, and sanitizer checks.
- Docker image for one-command startup.

## Tech Stack

- C++20
- CMake
- cpp-httplib
- nlohmann/json
- CTest
- Docker
- GitHub Actions

## API

### Health Check

```http
GET /health
```

Response:

```json
{
  "status": "ok"
}
```

### Put Value

```http
PUT /kv/name
Content-Type: application/json

{
  "value": "Pavel"
}
```

Response:

```json
{
  "key": "name",
  "value": "Pavel"
}
```

### Get Value

```http
GET /kv/name
```

Response:

```json
{
  "key": "name",
  "value": "Pavel"
}
```

### List Values

```http
GET /kv
```

### Delete Value

```http
DELETE /kv/name
```

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Test

```bash
ctest --test-dir build --output-on-failure
```

## Run

```bash
./build/kv_server config.example.json
```

Then try:

```bash
curl http://localhost:8080/health
curl -X PUT http://localhost:8080/kv/name \
  -H "Content-Type: application/json" \
  -d '{"value":"Pavel"}'
curl http://localhost:8080/kv/name
```

## Docker

```bash
docker build -t cpp-kv-server .
docker run --rm -p 8080:8080 cpp-kv-server
```

## Project Structure

```text
.
|-- include/kv/          # Public project headers
|-- src/                 # Server, config, and storage implementation
|-- tests/               # CTest-based tests
|-- examples/            # HTTP request examples
|-- .github/workflows/   # CI pipeline
|-- CMakeLists.txt
|-- Dockerfile
`-- README.md
```

## What This Project Demonstrates

- Building a small backend service in modern C++.
- Separating HTTP, storage, configuration, and validation logic.
- Using third-party libraries through CMake FetchContent.
- Writing tests for core logic.
- Adding CI and sanitizer checks.
- Packaging a C++ service with Docker.
