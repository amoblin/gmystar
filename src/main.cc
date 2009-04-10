#include "gMystar.h"

int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        if(!strcmp(argv[1],"--nogui"))
        {
            Mystar mystar;
        }
        else if(!strcmp(argv[1],"--help"))
        {
            gMystar::help();
        }
        else if(!strcmp(argv[1],"--test"))
        {
        }
        else
        {
            printf("usage: gMystar --help for more details");
        }
    }
    else
    {
        gMystar gmystar(argc, argv);
    }
    return 0;
}
