#ifndef HASHING_LAYER_H
#define HASHING_LAYER_H

#include <string>

class HashingLayer {
public:
    std::string hash(const std::string& sensitiveData) const;
    bool verify(const std::string& sensitiveData, const std::string& storedHash) const;

private:
    static const std::string HASH_VERSION;

    std::string generateSalt() const;
    std::string hashWithSalt(const std::string& sensitiveData, const std::string& salt) const;
    bool parseHash(const std::string& storedHash, std::string& salt, std::string& hashValue) const;
    bool constantTimeEquals(const std::string& left, const std::string& right) const;
};

#endif // HASHING_LAYER_H
