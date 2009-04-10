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
        printf("invalid name found in mystar.conf!\n");
    else
    {
        strcpy(this->username,username);
        printf("username is %s\n",username);
    }
}
void User::set_password(const char *password)
{
    if( (password == NULL) || (password[0] == 0) )
        printf("invalid password found in mystar.conf!\n");
    else
    {
        strcpy(this->password,password);
        printf("password is %s\n", password);
    }
}
void User::set_nickname(const char *nickname)
{
    if( (nickname== NULL) || (nickname[0] == 0) )
        printf("invalid nickname found in mystar.conf!\n");
    else
    {
        strcpy(this->nickname,nickname);
        printf("nickname is %s\n", nickname);
    }
}
void User::set_fakeAddress(const char *fakeAddress)
{
    if( (fakeAddress == NULL) || (fakeAddress[0] == 0) )
        printf("invalid fakeAddress found in mystar.conf!\n");
    else
    {
        strcpy(this->fakeAddress,fakeAddress);
        //printf("fakeAddress is %s\n", fakeAddress);
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
    //printf("nickname is %s\n", this->nickname);
    return nickname;
}
char *User::get_fakeAddress()
{
    //printf("fakeAddress is %s\n", fakeAddress);
    return fakeAddress;
}
