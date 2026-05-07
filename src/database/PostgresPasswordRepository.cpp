#include "database/PostgresPasswordRepository.h"

#include <stdexcept>
#include <utility>

using namespace std;

PostgresPasswordRepository::PostgresPasswordRepository(string connection)
    : connectionString(std::move(connection))
#ifdef USE_POSTGRES
    , connection(nullptr)
#endif
{
#ifdef USE_POSTGRES
    connect();
    ensureSchema();
#else
    throw runtime_error("PostgreSQL support is not compiled. Rebuild with USE_POSTGRES=1.");
#endif
}

PostgresPasswordRepository::~PostgresPasswordRepository() {
#ifdef USE_POSTGRES
    if (connection != nullptr) {
        PQfinish(connection);
    }
#endif
}

optional<string> PostgresPasswordRepository::loadPasswordHash() {
#ifdef USE_POSTGRES
    PGresult* result = PQexec(
        connection,
        "SELECT password_hash FROM password_hashes WHERE id = 1"
    );
    ensureSuccessfulResult(result, PGRES_TUPLES_OK, "load password hash");

    optional<string> passwordHash = nullopt;
    if (PQntuples(result) > 0 && !PQgetisnull(result, 0, 0)) {
        passwordHash = string(PQgetvalue(result, 0, 0));
    }

    PQclear(result);
    return passwordHash;
#else
    return nullopt;
#endif
}

void PostgresPasswordRepository::savePasswordHash(const string& passwordHash) {
#ifdef USE_POSTGRES
    const char* values[1] = {passwordHash.c_str()};
    PGresult* result = PQexecParams(
        connection,
        "INSERT INTO password_hashes (id, password_hash, updated_at) "
        "VALUES (1, $1, NOW()) "
        "ON CONFLICT (id) DO UPDATE SET password_hash = EXCLUDED.password_hash, updated_at = NOW()",
        1,
        nullptr,
        values,
        nullptr,
        nullptr,
        0
    );

    ensureSuccessfulResult(result, PGRES_COMMAND_OK, "save password hash");
    PQclear(result);
#else
    (void)passwordHash;
#endif
}

string PostgresPasswordRepository::name() const {
    return "postgres";
}

#ifdef USE_POSTGRES
void PostgresPasswordRepository::connect() {
    connection = PQconnectdb(connectionString.c_str());
    if (connection == nullptr) {
        throw runtime_error("Unable to allocate PostgreSQL connection");
    }

    if (PQstatus(connection) != CONNECTION_OK) {
        const string error = PQerrorMessage(connection);
        PQfinish(connection);
        connection = nullptr;
        throw runtime_error("Unable to connect to PostgreSQL: " + error);
    }
}

void PostgresPasswordRepository::ensureSchema() {
    PGresult* result = PQexec(
        connection,
        "CREATE TABLE IF NOT EXISTS password_hashes ("
        "id SMALLINT PRIMARY KEY CHECK (id = 1), "
        "password_hash TEXT NOT NULL, "
        "updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW()"
        ")"
    );

    ensureSuccessfulResult(result, PGRES_COMMAND_OK, "ensure password_hashes schema");
    PQclear(result);
}

void PostgresPasswordRepository::ensureSuccessfulResult(
    PGresult* result,
    ExecStatusType expectedStatus,
    const string& action
) {
    if (result == nullptr) {
        throw runtime_error("Unable to " + action + ": PostgreSQL returned no result");
    }

    if (PQresultStatus(result) == expectedStatus) {
        return;
    }

    const string error = PQerrorMessage(connection);
    PQclear(result);
    throw runtime_error("Unable to " + action + ": " + error);
}
#endif
