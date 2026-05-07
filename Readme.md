This project is a PasswordManager utility that lets you store hashed sensitive data.

## Basic Workflow:
- User enters sensitive data.
- Data goes through hashing layer, the application doesn't store raw data.
- When the user re-requests the data, the application hashes the input again
  with the stored salt and compares the result.
- Database layer to persist the hashes for user.

## Current Scope:
- Application Layer: CLI only
- Hashing Layer: Salted one-way hashing with a versioned storage format
- Database Layer: Relational Database (PostgreSQL)

## Hashing Layer

The hashing layer lives in `HashingLayer.h` and `HashingLayer.cpp`.

- New values are stored as `pmv1$<salt>$<hash>`.
- The salt is generated per value, so the same password should not produce the
  same stored string twice.
- Validation parses the stored hash, re-hashes the provided input with the
  stored salt, and compares the computed hash with the stored hash.
- Legacy XOR-encrypted values from the previous implementation are upgraded to
  the new salted hash format after a successful validation.
- The first implementation is dependency-free to keep the Docker image light.
  It should be replaced with Argon2, bcrypt, or PBKDF2 before production use.

## Infrastructure

The project now includes a lightweight Docker setup for local development and
containerized runs.

### Docker

- `Dockerfile` uses a multi-stage Alpine build.
- The builder stage installs only `g++` and `make` to compile the CLI.
- The runtime stage keeps only Alpine plus `libstdc++`, then runs the compiled
  `password-manager` binary as a non-root user.
- Runtime data is stored in `/data`, so the current `password.txt` persistence
  file is kept outside the image through a Docker volume.

### Docker Compose

- `compose.yaml` defines the CLI `app` service.
- `compose.yaml` also defines a `db` service using `postgres:16-alpine` for the
  planned relational persistence layer. The CLI does not depend on it yet, so
  containerized CLI runs stay lightweight.
- Compose volumes keep password data and PostgreSQL data persistent between
  container restarts.

### Make Targets

- `make build` compiles the local binary to `bin/password-manager`.
- `make build CXX=/path/to/compiler` compiles with an explicit compiler when
  auto-detection is not desired.
- `make run` builds and runs the CLI locally.
- `make clean` removes local build output.
- `make docker-build` builds the lightweight Docker image.
- `make compose-build` builds the Compose app image.
- `make compose-run` runs the CLI through Docker Compose.
- `make compose-up` starts the PostgreSQL service.
- `make compose-down` stops Compose services and networks.
- `make compose-logs` follows Compose service logs.
