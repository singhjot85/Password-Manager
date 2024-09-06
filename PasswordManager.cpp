#include "PasswordManager.h"
#include <bits/stdc++.h>
using namespace std;

class PasswordManager{
private:
    string encryptedPass;
    PasswordManager(){
        encryptedPass="";
    }
    string encrypt(string Password){
        string encryptedPassword;
        for(auto x: Password) encryptedPassword+= x^'2';
        return encryptedPassword;
    }
    bool verifyPassword(string Pass){
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
public:
    void setEncryptedPass(string s){
        encryptedPass=s;
    }
    string getEncryptedPass(){
        return encryptedPass;
    }
    bool setNewPass(string s){
        if(verifyPassword(s)){
            setEncryptedPass(encrypt(s));
            return true;
        }
    return false;
    }
    bool validatePass(string s){
        if(encrypt(s)==encryptedPass) return true;
    return false;
    }
};