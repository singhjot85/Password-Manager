#include<bits/stdc++.h>
#include "PasswordManager.h"
using namespace std;

void prompt(){
    cout<<"Password Utilities\n";
    cout<<"A. Change Password\n";
    cout<<"B. Validate Password\n";
    cout<<"C. Quit";
    cout<<"Enter your choice: ";
}

int main(){
    bool loop=true;
    while(loop){
        PasswordManager *P1= new PasswordManager();
        prompt();
        char in; cin>>in;
        switch (in){
        case 'A':
            cout<<"Enter your Password: ";
            string inPass;
            getline(cin,inPass);
            if(P1->setNewPass(inPass)) cout<<"Password set sucessfully\n";
            else cout<<"Password format invalid\n";
            break;
        case 'B':
            break;
        default:
            cout<<"Enter valid input\n";
            break;
        }


    }
    return 0;
}