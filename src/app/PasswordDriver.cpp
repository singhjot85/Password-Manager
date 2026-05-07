#include "core/PasswordManager.h"
#include "database/FilePasswordRepository.h"
#include "database/PostgresPasswordRepository.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

using namespace std;

namespace {
string getEnvironmentValue(const string& key, const string& fallback = "") {
    const char* value = getenv(key.c_str());
    if (value == nullptr || string(value).empty()) {
        return fallback;
    }

    return value;
}

string buildPostgresConnectionString() {
    const string explicitConnectionString = getEnvironmentValue("DATABASE_URL");
    if (!explicitConnectionString.empty()) {
        return explicitConnectionString;
    }

    return "host=" + getEnvironmentValue("POSTGRES_HOST", "localhost") +
           " port=" + getEnvironmentValue("POSTGRES_PORT", "5432") +
           " dbname=" + getEnvironmentValue("POSTGRES_DB", "password_manager") +
           " user=" + getEnvironmentValue("POSTGRES_USER", "password_manager") +
           " password=" + getEnvironmentValue("POSTGRES_PASSWORD", "password_manager");
}

unique_ptr<PasswordRepository> createPasswordRepository() {
    const string repositoryType = getEnvironmentValue("PASSWORD_REPOSITORY", "file");

    if (repositoryType == "postgres") {
        return unique_ptr<PasswordRepository>(
            new PostgresPasswordRepository(buildPostgresConnectionString())
        );
    }

    return unique_ptr<PasswordRepository>(
        new FilePasswordRepository(getEnvironmentValue("PASSWORD_FILE", "password.txt"))
    );
}

void prompt() {
    cout << "\nPassword Utilities\n";
    cout << "A. Change Password\n";
    cout << "B. Validate Password\n";
    cout << "C. Quit\n";
    cout << "Enter your choice: ";
}

void passwordPolicyPrompt() {
    cout << "Criteria for a valid password:\n";
    cout << "\tIt is at least 8 characters long\n";
    cout << "\tIt contains at least one letter\n";
    cout << "\tIt contains at least one digit\n";
    cout << "\tIt contains at least one of these six characters: <, >, ?, !, @, &\n";
    cout << "Enter a new password: ";
}
}

int main() {
    try {
        PasswordManager passwordManager;
        unique_ptr<PasswordRepository> repository = createPasswordRepository();

        const optional<string> storedPasswordHash = repository->loadPasswordHash();
        if (storedPasswordHash.has_value()) {
            passwordManager.setStoredHash(storedPasswordHash.value());
        } else {
            passwordManager.setNewPassword("abc123!!!");
            repository->savePasswordHash(passwordManager.getStoredHash());
        }

        bool loop = true;
        while (loop) {
            prompt();
            char input;
            cin >> input;
            cin.ignore();

            if (input == 'A' || input == 'a') {
                while (true) {
                    passwordPolicyPrompt();
                    string newPassword;
                    getline(cin, newPassword);

                    if (passwordManager.setNewPassword(newPassword)) {
                        repository->savePasswordHash(passwordManager.getStoredHash());
                        cout << "Password changed successfully\n";
                        break;
                    }

                    cout << "Invalid Password\n";
                }
            } else if (input == 'B' || input == 'b') {
                cout << "Password to validate: ";
                string inputPassword;
                getline(cin, inputPassword);

                if (passwordManager.validatePassword(inputPassword)) {
                    repository->savePasswordHash(passwordManager.getStoredHash());
                    cout << "Password is valid\n";
                } else {
                    cout << "Password invalid\n";
                }
            } else if (input == 'C' || input == 'c') {
                repository->savePasswordHash(passwordManager.getStoredHash());
                cout << "Exiting and saving the password...\n";
                loop = false;
            } else {
                cout << "Enter a valid input.\n";
            }
        }
    } catch (const exception& error) {
        cerr << "Application error: " << error.what() << "\n";
        return 1;
    }

    return 0;
}
