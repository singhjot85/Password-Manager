#ifndef PASSWORD_MANAGER_DATABASE_FILE_PASSWORD_REPOSITORY_H
#define PASSWORD_MANAGER_DATABASE_FILE_PASSWORD_REPOSITORY_H

#include "database/PasswordRepository.h"

#include <string>

class FilePasswordRepository : public PasswordRepository {
public:
    explicit FilePasswordRepository(std::string filePath);

    std::optional<std::string> loadPasswordHash() override;
    void savePasswordHash(const std::string& passwordHash) override;
    std::string name() const override;

private:
    std::string filePath;
};

#endif // PASSWORD_MANAGER_DATABASE_FILE_PASSWORD_REPOSITORY_H
