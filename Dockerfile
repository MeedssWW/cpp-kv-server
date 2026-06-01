FROM ubuntu:24.04 AS build

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    cmake \
    g++ \
    git \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --config Release \
    && ctest --test-dir build --output-on-failure

FROM ubuntu:24.04

WORKDIR /app
COPY --from=build /app/build/kv_server /app/kv_server
COPY config.example.json /app/config.json

EXPOSE 8080

CMD ["/app/kv_server", "/app/config.json"]
