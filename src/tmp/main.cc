#include <gtkmm.h>
using namespace Gtk;
 
int main(int argc, char *argv[])
{
    Main kit(argc, argv);
 
    Window window;
    window.set_title("Beispiel");
    window.set_border_width(15);
 
    Label hello("Hallo Welt!");
    Button quit_button(Stock::QUIT);
    quit_button.signal_clicked().connect(sigc::ptr_fun(&Gtk::Main::quit));
 
    VBox box(false, 15);
    box.add(hello);
    box.add(quit_button);
 
    window.add(box);
    window.show_all();
    Main::run(window);
    return 0;
}
