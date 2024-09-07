#include <bits/stdc++.h>
#include "PASSWORDMANAGER.h"
using namespace std;

void prompt() {
    cout << "Password Utilities\n";
    cout << "A. Change Password\n";
    cout << "B. Validate Password\n";
    cout << "C. Quit\n";
    cout << "Enter your choice: ";
}

void prompt2() {
    cout << "Criteria for a valid password:\n";
    cout << "\tIt is at least 8 characters long\n";
    cout << "\tIt contains at least one letter\n";
    cout << "\tIt contains at least one digit\n";
    cout << "\tIt contains at least one of these six characters: <, >, ?, !, @, &\n";
    cout << "Enter a new password: ";
}

int main() {
    PasswordManager the_password_manager; 

    string fileName = "password.txt";
    ifstream file(fileName);

    if (file.good()) {
        string encryptedPassword;
        getline(file, encryptedPassword);
        the_password_manager.setEncryptedPass(encryptedPassword);
        file.close();
    } else {
        string defaultPassword = "abc123!!!";
        the_password_manager.setNewPass(defaultPassword);
    }

    bool loop = true;
    while (loop) {
        prompt();
        char input; cin >> input;
        cin.ignore();

        if (input == 'A' || input == 'a') {
            while (true) {
                prompt2();
                string newPassword;
                getline(cin, newPassword);

                if (the_password_manager.setNewPass(newPassword)) {
                    ofstream file(fileName);
                    file << the_password_manager.getEncryptedPass();
                    file.close();
                    cout << "Password changed successfully\n";
                    break;
                } 
                else cout << "Invalid Password\n";

            }
        } 
        else if (input == 'B' || input == 'b') {
            while (true) {
                cout << "Password to validate: ";
                string inputPassword;
                getline(cin, inputPassword);

                if (the_password_manager.validatePass(inputPassword)) {
                    cout << "Password is valid\n";
                    break; 
                } 
                else cout << "Password invalid\n";
            }
        } 
        else if (input == 'C' || input == 'c') {
            ofstream file(fileName);
            file << the_password_manager.getEncryptedPass();
            file.close();
            cout << "Exiting and saving the password...\n";
            loop = false; 
        } 
        else cout << "Enter a valid input.\n";
    }

    return 0;
}
