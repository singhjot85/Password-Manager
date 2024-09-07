#include "PASSWORDMANAGER.h"
#include <bits/stdc++.h>
using namespace std;

PasswordManager::PasswordManager(){
    encryptedPass="";
}

bool PasswordManager::verifyPassword(string Pass){
    if(Pass.length() < 8) return false;
    bool hasLetter=false,hasDigit=false,hasChar=false;
    set<char> st={'<','>','?','!','@','&'};
    for(auto x: Pass){
        if(hasLetter && hasDigit && hasChar) break;
        if((x>'a' && x<'z')||(x>'A' && x<'Z')) hasLetter= true;
        if(x<'9' && x>'0') hasDigit= true;
        if(st.find(x)!=st.end()) hasChar=true;
    }
    return (hasLetter && hasChar && hasDigit);
}

void PasswordManager::setEncryptedPass(string s){
    encryptedPass=s;
}

string PasswordManager::getEncryptedPass(){
    return encryptedPass;
}

bool PasswordManager::setNewPass(string s){
    if(verifyPassword(s)){
        setEncryptedPass(encrypt(s));
        return true;
    }
return false;
}

bool PasswordManager::validatePass(string s){
    if(encrypt(s)==encryptedPass) return true;
return false;
}
