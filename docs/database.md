# Database Layer

The database layer is intentionally behind a small repository interface:
`PasswordRepository`.

Current implementations:

- `FilePasswordRepository`: local development and simple CLI use.
- `PostgresPasswordRepository`: production-style persistence through
  PostgreSQL/libpq.

PostgreSQL schema:

```sql
CREATE TABLE IF NOT EXISTS password_hashes (
    id SMALLINT PRIMARY KEY CHECK (id = 1),
    password_hash TEXT NOT NULL,
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);
```

The single-row `id = 1` shape matches the current single-user CLI scope. When
user accounts are introduced, replace this table with a user-owned credentials
table and migrate `password_hash` behind a user identifier.
