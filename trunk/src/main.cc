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
        else if(!strcmp(argv[1],"-s"))
        {
        }
        else
        {
            cout<<"usage: gMystar --help for more details"<<endl;
        }
    }
    else
    {
        //gMystar gmystar(argc, argv);
        Gtk::Main kit(argc, argv);
        gMystar gmystar;
        kit.run();
    }
    return 0;
}
