#include "hello-app.hpp"
int main (int argc, char *argv[])
{
    /*    可以看到，这跟“最简单的Gtkmm程序”非常相似。HelloApp继承自    “Gtk::Main”类，也需要通过 argc 和 argv 两个参数进行实例化。    */
    HelloApp app(argc, argv);
    /*    在这个函数中调用Gtk::Main::run函数来实现与上面相同的功能。       这里不需要窗口对象作参数，因为它已封装在HelloApp类中了。    */
    app.show_window();
    return 0;
}
