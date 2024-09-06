#ifndef PassswordManager_H
#define PasswordManger_H

#include <bits/stdc++.h>
using namespace std;

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


#endif // PasswordManager_H