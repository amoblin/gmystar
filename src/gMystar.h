#include "Mystar.h"
#include <gtkmm.h>
#include <libglademm.h>
#include <boost/lexical_cast.hpp>
using namespace Gtk;

class gMystar
{
    public:
        gMystar(int argc, char* argv[]);
        ~gMystar();
        static void help();
        static Mystar *mystar;
    protected:
        void on_quit_button_clicked();
        void on_connect_button_clicked();
        void on_disconnected_button_clicked();
        void on_tray_clicked();
        void show_message(const char *message);
        void show_window();
        void hide_window();
        static void *authen(void *);
        static void *test(void *);
        static void *change_ui(void *);

        Glib::RefPtr<Gnome::Glade::Xml> refXml;
        static Window *MainWindow;
        Entry *username_entry;
        Entry *password_entry;
        Entry *fakeAddress_entry;
        Entry *nickname_entry;
        CheckButton *autologin_checkbutton;
        static Button *connect_button;
        Button *disconnect_button;
        Button *quit_button;
        static Label *status_label;
        static Glib::RefPtr<Gtk::StatusIcon> status_icon;

        pthread_t authen_thread;
        pthread_t change_ui_thread;

        static int flag;

        int window_x;
        int window_y;
};
