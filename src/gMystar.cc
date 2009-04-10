#include "gMystar.h"
#include <pthread.h>
/* static member */
Mystar *gMystar::mystar;
int gMystar::flag;
Glib::RefPtr<Gtk::StatusIcon> gMystar::status_icon;
Window *gMystar::MainWindow;
Button *gMystar::connect_button;
Label *gMystar::status_label;

gMystar::gMystar(int argc, char *argv[])
{
    mystar = new Mystar();
    Main kit(argc, argv);
    flag = 1;

    //tray
    status_icon = StatusIcon::create_from_file("/etc/gMystar/disconnect.png");
    if(status_icon)
    {
        status_icon->set_tooltip("用gtkmm和glade做的锐捷客户端.");
        status_icon->signal_activate().connect(sigc::mem_fun(*this, &gMystar::on_tray_clicked));
    }
    else
    {
        cout<<"create tray icon error!\n";
    }

    refXml = Gnome::Glade::Xml::create("/etc/gMystar/ui.glade");
    //refXml = Gnome::Glade::Xml::create("./gMystar.glade");
    MainWindow= NULL;
    MainWindow= refXml->get_widget("gMystar", MainWindow);

    nickname_entry= NULL;
    nickname_entry = refXml->get_widget("nickname", nickname_entry);
    nickname_entry->set_text(mystar->user.get_nickname());

    username_entry= NULL;
    username_entry= refXml->get_widget("username", username_entry);
    username_entry->set_text(mystar->user.get_username());

    password_entry= NULL;
    password_entry= refXml->get_widget("password", password_entry);
    password_entry->set_text(mystar->user.get_password());

    fakeAddress_entry = NULL;
    fakeAddress_entry = refXml->get_widget("fakeAddress", fakeAddress_entry);
    fakeAddress_entry->set_text(mystar->user.get_fakeAddress());

    autologin_checkbutton = NULL;
    autologin_checkbutton = refXml->get_widget("autologin_checkbutton",autologin_checkbutton);

    connect_button = NULL;
    connect_button = refXml->get_widget("connect_button", connect_button);
    if(connect_button)
    {
        connect_button->signal_clicked().connect(sigc::mem_fun(*this, &gMystar::on_connect_button_clicked));
    }

    disconnect_button = NULL;
    disconnect_button = refXml->get_widget("disconnect_button", disconnect_button);
    if(disconnect_button)
    {
        disconnect_button->signal_clicked().connect(sigc::mem_fun(*this, &gMystar::on_disconnected_button_clicked));
    }

    quit_button = NULL;
    quit_button = refXml->get_widget("quit_button", quit_button);
    if(quit_button)
    {
        quit_button->signal_clicked().connect(sigc::mem_fun(*this, &gMystar::on_quit_button_clicked));
    }

    status_label = NULL;
    status_label = refXml->get_widget("status",status_label);

    MainWindow->set_focus(*connect_button);
    MainWindow->show();

    if(mystar->autologin)
        on_connect_button_clicked();

    //kit.run(*MainWindow); //这样的话在hide时就退出程序了
    kit.run();          //这样的话在hide时不退出程序
}
gMystar::~gMystar()
{
}
void gMystar::on_quit_button_clicked()
{
    MainWindow->hide();//这样只是隐藏了，如何结束呢？
}
void gMystar::on_disconnected_button_clicked()
{
    //*flag=0;
    //cout<<"我看你还循环不？哼！"<<endl;
    //Mystar *my = new Mystar
    if(!pthread_cancel(authen_thread))
        cout<<"取消认证。。。\n";
}
void gMystar::on_connect_button_clicked()   //这里不用加static关键字
{
    cout<<"用于连接的账号信息是:"<<endl;
    char str[20];
    strcpy(str,nickname_entry->get_text().c_str());
    mystar->user.set_nickname(str);

    strcpy(str,username_entry->get_text().c_str());
    mystar->user.set_username(str);

    strcpy(str,password_entry->get_text().c_str());
    mystar->user.set_password(str);

    strcpy(str,fakeAddress_entry->get_text().c_str());
    mystar->user.set_fakeAddress(str);
    //show_message("begin to authen\n");
    cout<<"begin to authen\n";
    status_label->set_label("searching for server...");

    int res;    //线程id
    //认证线程
    res = pthread_create(&authen_thread, NULL,gMystar::test, NULL);
    if(res!=0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    int res2 = pthread_create(&change_ui_thread, NULL,gMystar::change_ui, NULL);
    if(res2 != 0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
}
void gMystar::help()
{
    printf("to use no gui mode, please type --nogui\n");
}
void gMystar::show_message(const char *message)
{
    //show_message(message);
    status_label->set_text(message);
}
void gMystar::on_tray_clicked()
{
    if(MainWindow->is_visible())
        hide_window();
    else
        show_window();
}
void gMystar::show_window()
{
    MainWindow->move(window_x, window_y);
    cout<<"resume the position("<<window_x<<","<<window_y<<")"<<endl;
    MainWindow->show();
}
void gMystar::hide_window()
{
    MainWindow->get_position(window_x, window_y);
    cout<<"now the position is("<<window_x<<","<<window_y<<")"<<endl;
    MainWindow->hide();
}
void *gMystar::authen(void *a)
{
}
void *gMystar::test(void *a)
{
    mystar->authen(&flag);
}
void *gMystar::change_ui(void *a)
{
    int flag = true;
    while(flag)
    {
        switch(mystar->get_status())
        {
            case 0:
                //modify_file();
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                status_icon->set_from_file("/etc/gMystar/connect.png");
                status_label->set_label("keep sending echo...");
                connect_button->set_label("断开连接");
                //connect_button->signal_clicked().connect()::disconnect();
                //connect_button->signal_clicked().connect(sigc::mem_fun(*gMystar, &gMystar::on_disconnected_button_clicked));
                MainWindow->hide();

                cout<<"*******************";
                mystar->save_file();
                flag=false;
                break;
            default:
                break;
        }
    }
}
