/* tipfoo<tipfoo@gmail#com> 原创作品，版权所有，出处： http://blog.csdn.net/tipfoo/  */
#include "hello-app.hpp"
#include <iostream>
/* 输出错误信息到控制台 */
#include <assert.h>
/* assert()断言 */
using namespace Gnome::Glade;
HelloApp::HelloApp(int argc, char *argv[]):
    Gtk::Main(argc,argv),
    /* 必须在初始化列表中调用父类的构造函数 */
    main_window(0)
    /* 为主窗口的指针赋初值 */
{
    try    {
        /* 从同目录下的GLADE文件创建“Gnome::Glade::Xml”对象 */
        ref_xml = Xml::create("helloapp.glade");
    }
    catch(const XmlError& ex)
    {
        /*   出错时错误信息输出到控制台，并返回 */ 
        std::cerr << ex.what() << std::endl;
        return;
    } 
    /*    取得主窗口的指针，存入main_window变量中，并确保成功。    main_window为0值表示失败，多是因为指定的控件名称不正确或不存在。    第一个参数就是Glade文件中定义的控件名称，是字符串型。    */
    ref_xml->get_widget("gMystar", main_window);

    assert(main_window);
}
HelloApp::~HelloApp()
{
    /* 由于采用了智能指针，我们不需要管理资源的释放 */
}
void HelloApp::show_window()
{
    /*     如果取得主窗口的指针成功，就调用父类的run()函数显示它，    并进入监听事件的循环状态，当主窗口关闭时返回。    */
    if (main_window)
    {
        run( *main_window ); 
    }
}
