#include "core/PasswordManager.h"

#include <set>

using namespace std;

PasswordManager::PasswordManager() : storedHash("") {}

void PasswordManager::setStoredHash(const string& hash) {
    storedHash = hash;
}

string PasswordManager::getStoredHash() const {
    return storedHash;
}

bool PasswordManager::hasStoredHash() const {
    return !storedHash.empty();
}

bool PasswordManager::setNewPassword(const string& password) {
    if (!meetsPasswordPolicy(password)) {
        return false;
    }

    setStoredHash(hashingLayer.hash(password));
    return true;
}

bool PasswordManager::validatePassword(const string& password) {
    if (hashingLayer.verify(password, storedHash)) {
        return true;
    }

    if (matchesLegacyEncryptedPassword(password)) {
        setStoredHash(hashingLayer.hash(password));
        return true;
    }

    return false;
}

bool PasswordManager::meetsPasswordPolicy(const string& password) const {
    if (password.length() < 8) {
        return false;
    }

    bool hasLetter = false;
    bool hasDigit = false;
    bool hasSpecialCharacter = false;
    const set<char> allowedSpecialCharacters = {'<', '>', '?', '!', '@', '&'};

    for (const char character : password) {
        if (hasLetter && hasDigit && hasSpecialCharacter) {
            break;
        }

        if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z')) {
            hasLetter = true;
        }

        if (character >= '0' && character <= '9') {
            hasDigit = true;
        }

        if (allowedSpecialCharacters.find(character) != allowedSpecialCharacters.end()) {
            hasSpecialCharacter = true;
        }
    }

    return hasLetter && hasDigit && hasSpecialCharacter;
}

bool PasswordManager::matchesLegacyEncryptedPassword(const string& password) const {
    string legacyEncryptedPassword;
    for (const char character : password) {
        legacyEncryptedPassword += character ^ '2';
    }

    return legacyEncryptedPassword == storedHash;
}
