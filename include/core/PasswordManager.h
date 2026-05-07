#ifndef PASSWORD_MANAGER_CORE_PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_CORE_PASSWORD_MANAGER_H

#include "crypto/HashingLayer.h"

#include <string>

class PasswordManager {
public:
    PasswordManager();

    void setStoredHash(const std::string& storedHash);
    std::string getStoredHash() const;
    bool hasStoredHash() const;
    bool setNewPassword(const std::string& password);
    bool validatePassword(const std::string& password);

private:
    std::string storedHash;
    HashingLayer hashingLayer;

    bool meetsPasswordPolicy(const std::string& password) const;
    bool matchesLegacyEncryptedPassword(const std::string& password) const;
};

#endif // PASSWORD_MANAGER_CORE_PASSWORD_MANAGER_H
