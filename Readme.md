# Password Manager

This project is a CLI password manager utility that stores sensitive values as
salted hashes and persists them through a repository layer.

## Architecture

- `src/app`: CLI entrypoint and runtime wiring.
- `src/core`: password policy and password validation workflow.
- `src/crypto`: hashing layer.
- `src/database`: persistence implementations.
- `include`: public headers grouped by layer.
- `db/init`: PostgreSQL schema initialization scripts.
- `docs`: project documentation.

## Basic Workflow

- User enters sensitive data.
- Data goes through the hashing layer, so the application does not store raw
  input.
- When the user validates a password, the application hashes the input again
  with the stored salt and compares the result.
- The database layer persists the stored hash.

## Hashing Layer

- New values are stored as `pmv1$<salt>$<hash>`.
- The salt is generated per value, so the same password should not produce the
  same stored string twice.
- Legacy XOR-encrypted values from the previous implementation are upgraded to
  the new salted hash format after a successful validation.
- The current implementation is dependency-free to keep the first production
  image lightweight. Replace it with Argon2, bcrypt, or PBKDF2 before handling
  real credentials.

## Database Layer

The app uses a `PasswordRepository` interface.

- `FilePasswordRepository` is the default for local runs and writes to
  `password.txt`.
- `PostgresPasswordRepository` is used when `PASSWORD_REPOSITORY=postgres` and
  the app is built with `USE_POSTGRES=1`.
- PostgreSQL stores the current hash in the `password_hashes` table.

## Local Commands

- `make build` builds the file-backed CLI to `bin/password-manager`.
- `make run` builds and runs the local file-backed CLI.
- `make build USE_POSTGRES=1` builds with PostgreSQL support.
- `make clean` removes local build output.

## Docker And Compose

- `make docker-build` builds the production-style image.
- `make compose-build` builds the Compose app image.
- `make compose-run` starts PostgreSQL and runs the CLI against it.
- `make compose-up` starts PostgreSQL in the background.
- `make compose-down` stops Compose services and networks.
- `make compose-logs` follows service logs.

## Runtime Configuration

- `PASSWORD_REPOSITORY=file` uses `FilePasswordRepository`.
- `PASSWORD_FILE=password.txt` controls the local file path.
- `PASSWORD_REPOSITORY=postgres` uses PostgreSQL.
- `DATABASE_URL` can provide the full PostgreSQL connection string.
- `POSTGRES_HOST`, `POSTGRES_PORT`, `POSTGRES_DB`, `POSTGRES_USER`, and
  `POSTGRES_PASSWORD` are used when `DATABASE_URL` is not set.
