#include "database/FilePasswordRepository.h"

#include <fstream>
#include <stdexcept>
#include <utility>

using namespace std;

FilePasswordRepository::FilePasswordRepository(string path) : filePath(std::move(path)) {}

optional<string> FilePasswordRepository::loadPasswordHash() {
    ifstream file(filePath);
    if (!file.good()) {
        return nullopt;
    }

    string passwordHash;
    getline(file, passwordHash);

    if (passwordHash.empty()) {
        return nullopt;
    }

    return passwordHash;
}

void FilePasswordRepository::savePasswordHash(const string& passwordHash) {
    ofstream file(filePath, ios::trunc);
    if (!file.good()) {
        throw runtime_error("Unable to write password hash to " + filePath);
    }

    file << passwordHash;
}

string FilePasswordRepository::name() const {
    return "file:" + filePath;
}
