#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;
class User  //用户信息
{
    public:
        User(char *username, char *password, char *nickname, char *fakeAddress);
        User();
        void set_username(const char *username);
        void set_password(const char *password);
        void set_nickname(const char *nickname);
        void set_fakeAddress(const char *fakeAddress);
        char *get_username();
        char *get_password();
        char *get_nickname();
        char *get_fakeAddress();
    private:
            char username[32];
            char password[32];
            char nickname[32];
            char fakeAddress[32];
};
