#include "PasswordManager.h"

string PasswordManager::encrypt(string Password){
    string encryptedPassword;
    for(auto x: Password) encryptedPassword+= x^'2';
    return encryptedPassword;
}