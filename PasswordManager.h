#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include "HashingLayer.h"

#include <string>

class PasswordManager{
private:
    std::string hashedPass;
    HashingLayer hashingLayer;

    bool verifyPassword(const std::string& s);
    bool matchesLegacyEncryptedPass(const std::string& s);
public:
    PasswordManager();
    void setStoredHash(const std::string& s);
    std::string getStoredHash();
    bool setNewPass(const std::string& s);
    bool validatePass(const std::string& s);
};


#endif // PASSWORDMANAGER_H
