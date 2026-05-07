#include "PasswordManager.h"

#include <set>
using namespace std;

PasswordManager::PasswordManager(){
    hashedPass="";
}

bool PasswordManager::verifyPassword(const string& Pass){
    if(Pass.length() < 8) return false;
    bool hasLetter=false,hasDigit=false,hasChar=false;
    set<char> st={'<','>','?','!','@','&'};
    for(auto x: Pass){
        if(hasLetter && hasDigit && hasChar) break;
        if((x>='a' && x<='z')||(x>='A' && x<='Z')) hasLetter= true;
        if(x<='9' && x>='0') hasDigit= true;
        if(st.find(x)!=st.end()) hasChar=true;
    }
    return (hasLetter && hasChar && hasDigit);
}

bool PasswordManager::matchesLegacyEncryptedPass(const string& s){
    string legacyEncryptedPass;
    for(auto x: s) legacyEncryptedPass+= x^'2';
    return legacyEncryptedPass == hashedPass;
}

void PasswordManager::setStoredHash(const string& s){
    hashedPass=s;
}

string PasswordManager::getStoredHash(){
    return hashedPass;
}

bool PasswordManager::setNewPass(const string& s){
    if(verifyPassword(s)){
        setStoredHash(hashingLayer.hash(s));
        return true;
    }
return false;
}

bool PasswordManager::validatePass(const string& s){
    if(hashingLayer.verify(s, hashedPass)) return true;

    if(matchesLegacyEncryptedPass(s)){
        setStoredHash(hashingLayer.hash(s));
        return true;
    }

    return false;
}
