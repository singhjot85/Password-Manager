#ifndef PASSWORD_MANAGER_DATABASE_PASSWORD_REPOSITORY_H
#define PASSWORD_MANAGER_DATABASE_PASSWORD_REPOSITORY_H

#include <optional>
#include <string>

class PasswordRepository {
public:
    virtual ~PasswordRepository() = default;

    virtual std::optional<std::string> loadPasswordHash() = 0;
    virtual void savePasswordHash(const std::string& passwordHash) = 0;
    virtual std::string name() const = 0;
};

#endif // PASSWORD_MANAGER_DATABASE_PASSWORD_REPOSITORY_H
