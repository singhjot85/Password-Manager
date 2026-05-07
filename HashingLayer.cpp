#include "HashingLayer.h"

#include <chrono>
#include <iomanip>
#include <random>
#include <sstream>

using namespace std;

const string HashingLayer::HASH_VERSION = "pmv1";

string HashingLayer::hash(const string& sensitiveData) const {
    const string salt = generateSalt();
    return HASH_VERSION + "$" + salt + "$" + hashWithSalt(sensitiveData, salt);
}

bool HashingLayer::verify(const string& sensitiveData, const string& storedHash) const {
    string salt;
    string expectedHash;

    if (!parseHash(storedHash, salt, expectedHash)) {
        return false;
    }

    return constantTimeEquals(hashWithSalt(sensitiveData, salt), expectedHash);
}

string HashingLayer::generateSalt() const {
    random_device randomDevice;
    mt19937_64 generator(
        randomDevice() ^
        static_cast<unsigned long long>(
            chrono::high_resolution_clock::now().time_since_epoch().count()
        )
    );

    uniform_int_distribution<unsigned long long> distribution;
    stringstream saltStream;
    saltStream << hex << setfill('0') << setw(16) << distribution(generator)
               << setw(16) << distribution(generator);

    return saltStream.str();
}

string HashingLayer::hashWithSalt(const string& sensitiveData, const string& salt) const {
    // FNV-1a keeps this first hashing layer dependency-free. Swap this for
    // Argon2/bcrypt/PBKDF2 before production use.
    const unsigned long long fnvOffsetBasis = 14695981039346656037ULL;
    const unsigned long long fnvPrime = 1099511628211ULL;
    const string payload = salt + ":" + sensitiveData;

    unsigned long long hashValue = fnvOffsetBasis;
    for (size_t round = 0; round < 100000; ++round) {
        for (const unsigned char character : payload) {
            hashValue ^= character;
            hashValue *= fnvPrime;
        }

        hashValue ^= static_cast<unsigned long long>(round);
        hashValue *= fnvPrime;
    }

    stringstream hashStream;
    hashStream << hex << setfill('0') << setw(16) << hashValue;

    return hashStream.str();
}

bool HashingLayer::parseHash(const string& storedHash, string& salt, string& hashValue) const {
    const size_t versionSeparator = storedHash.find('$');
    if (versionSeparator == string::npos) {
        return false;
    }

    const size_t saltSeparator = storedHash.find('$', versionSeparator + 1);
    if (saltSeparator == string::npos || storedHash.find('$', saltSeparator + 1) != string::npos) {
        return false;
    }

    const string version = storedHash.substr(0, versionSeparator);
    if (version != HASH_VERSION) {
        return false;
    }

    salt = storedHash.substr(versionSeparator + 1, saltSeparator - versionSeparator - 1);
    hashValue = storedHash.substr(saltSeparator + 1);

    return !salt.empty() && !hashValue.empty();
}

bool HashingLayer::constantTimeEquals(const string& left, const string& right) const {
    if (left.length() != right.length()) {
        return false;
    }

    unsigned char difference = 0;
    for (size_t index = 0; index < left.length(); ++index) {
        difference |= static_cast<unsigned char>(left[index] ^ right[index]);
    }

    return difference == 0;
}
