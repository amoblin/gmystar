/*这两行和最后一行为了防止重复包含头文件*/
#ifndef _HELLO_APP_HPP_ 
#define _HELLO_APP_HPP_
#include <gtkmm.h>
#include <libglademm/xml.h>
/* 访问Glade文件所需的头文件 */
class HelloApp: public Gtk::Main
                /* 继承“Gtk::Main”类 */
{
    public:
        HelloApp(int argc, char *argv[]);
        ~HelloApp();
        void show_window();
    protected:
        /* 通过它访问Glade文件的内容，是一种智能指针，能自动释放占用的资源 */
        Glib::RefPtr<Gnome::Glade::Xml> ref_xml;
        Gtk::Window* main_window;
        /* 存储主窗口的指针 */
};
#endif
// _HELLO_APP_HPP
