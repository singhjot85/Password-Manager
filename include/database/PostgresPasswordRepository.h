#ifndef PASSWORD_MANAGER_DATABASE_POSTGRES_PASSWORD_REPOSITORY_H
#define PASSWORD_MANAGER_DATABASE_POSTGRES_PASSWORD_REPOSITORY_H

#include "database/PasswordRepository.h"

#include <memory>
#include <string>

#ifdef USE_POSTGRES
#include <libpq-fe.h>
#endif

class PostgresPasswordRepository : public PasswordRepository {
public:
    explicit PostgresPasswordRepository(std::string connectionString);
    ~PostgresPasswordRepository() override;

    std::optional<std::string> loadPasswordHash() override;
    void savePasswordHash(const std::string& passwordHash) override;
    std::string name() const override;

private:
    std::string connectionString;

#ifdef USE_POSTGRES
    PGconn* connection;

    void connect();
    void ensureSchema();
    void ensureSuccessfulResult(PGresult* result, ExecStatusType expectedStatus, const std::string& action);
#endif
};

#endif // PASSWORD_MANAGER_DATABASE_POSTGRES_PASSWORD_REPOSITORY_H
