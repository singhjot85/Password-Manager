#include <bits/stdc++.h>
using namespace std;

#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H


class PasswordManager{
private:
    string encryptedPass;
    string encrypt(string s);
    bool verifyPassword(string s);
public:
    PasswordManager();
    void setEncryptedPass(string s);
    string getEncryptedPass();
    bool setNewPass(string s);
    bool validatePass(string s);
};


#endif // PASSWORDMANAGER_H