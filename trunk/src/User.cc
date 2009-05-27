/****************************************************
 *  User.cc
 *
 *  Copyright (C) 2009  csip(amoblin@gmail.com)
 *
 *  ChangeLog:
 *
 *  Description:
 *	Warning: this file should be in UTF-8.
 *  
 *****************************************************/
#include "User.h"
User::User(char *username, char *password, char *nickname, char *fakeAddress)
{
    set_username(username);
    set_password(password);
    set_nickname(nickname);
    set_fakeAddress(fakeAddress);
}
User::User()
{
    memset(username,'\0',32);
    memset(password,'\0',32);
    memset(nickname,'\0',32);
    memset(fakeAddress,'\0',32);
}
void User::set_username(const char *username)
{
    if( (username == NULL) || (username[0] == 0) )
        cout<<"invalid name found in gMystarrc!\n"<<endl;
    else
    {
        strcpy(this->username,username);
        cout<<"username is "<<username<<endl;
    }
}
void User::set_password(const char *password)
{
    if( (password == NULL) || (password[0] == 0) )
        cout<<"invalid password found in gMystarrc!\n";
    else
    {
        strcpy(this->password,password);
        cout<<"password is "<<password<<endl;
        cout<<"here!"<<endl;
    }
}
void User::set_nickname(const char *nickname)
{
    if( (nickname== NULL) || (nickname[0] == 0) )
        cout<<"invalid nickname found in gMystarrc!\n";
    else
    {
        strcpy(this->nickname,nickname);
        cout<<"nickname is "<<nickname<<endl;
    }
}
void User::set_fakeAddress(const char *fakeAddress)
{
    if( (fakeAddress == NULL) || (fakeAddress[0] == 0) )
        cout<<"invalid fakeAddress found in gMystarrc!\n";
    else
    {
        strcpy(this->fakeAddress,fakeAddress);
        cout<<"fakeAddress is "<<fakeAddress<<endl;
    }
}
char *User::get_username()
{
    return username;
}
char *User::get_password()
{
    return password;
}
char *User::get_nickname()
{
    return nickname;
}
char *User::get_fakeAddress()
{
    return fakeAddress;
}
