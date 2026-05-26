FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    gcc \
    make \
    python3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN make build

CMD ["./check_python"]
