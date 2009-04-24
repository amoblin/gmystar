#include "Mystar.h"
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

void Mystar::checkAndSetConfig()
{
    TiXmlDocument mystarconf(filename);
    if(mystarconf.LoadFile())
    {
        cout<<"找到配置文件。。。\n";
    }
    else
    {
        init_file();
        return;
    }
    TiXmlElement* mystarroot = mystarconf.RootElement();
    if(mystarroot == NULL)
        return;
    TiXmlElement* UsersElement = mystarroot->FirstChildElement();
    if(UsersElement==NULL)
        return;
    TiXmlElement* UserElement = UsersElement->FirstChildElement();
    if(UserElement==NULL)
        return;
    TiXmlElement *nickname = UserElement->FirstChildElement();
    TiXmlElement *username = nickname->NextSiblingElement();
    TiXmlElement *password = username->NextSiblingElement();
    TiXmlElement *fakeAddress = password->NextSiblingElement();

    user.set_nickname(nickname->GetText());
    user.set_username(username->GetText());
    user.set_password(password->GetText());
    user.set_fakeAddress(fakeAddress->GetText());

    TiXmlElement *config = UsersElement->NextSiblingElement();

    TiXmlElement *autologinElement = config->FirstChildElement();
    TiXmlElement *authen = autologinElement->NextSiblingElement();
    cout<<"success!"<<endl;
    TiXmlElement *nicElement = authen->NextSiblingElement();
    cout<<"success!"<<endl;
    TiXmlElement *echo = nicElement->NextSiblingElement();

    strncpy(nic, nicElement->GetText(), sizeof(nic)-1);
    nic[sizeof(nic)-1]=0;

    if(!strcmp(autologinElement->GetText(),"true"))
        autologin = true;
    else
        autologin = false;
    //autologin=autologinElement->GetText();
    cout<<"autologin is:"<<autologin<<endl;
#ifdef DEBUG
    printf("nic is: %s\n",nic);
#endif

    echoInterval = atoi(echo->GetText());
#ifdef DEBUG
    printf("echoInterval is: %d\n",echoInterval);
#endif


}
void Mystar::init_file()
{
    //创建一个XML的文档对象。
    TiXmlDocument *myDocument = new TiXmlDocument();
    TiXmlDeclaration *title = new TiXmlDeclaration("1.0","utf-8","yes");
    myDocument->LinkEndChild(title);

    TiXmlComment *comment = new TiXmlComment("update the information below");
    myDocument->LinkEndChild(comment);

    //创建一个根元素并连接。
    TiXmlElement *RootElement = new TiXmlElement("mystar"); myDocument->LinkEndChild(RootElement);
    //创建一个users元素并连接。
    TiXmlElement *UsersElement = new TiXmlElement("users");
    RootElement->LinkEndChild(UsersElement);
    //创建一个user元素并连接。
    TiXmlElement *userElement = new TiXmlElement("user");
    UsersElement->LinkEndChild(userElement);
    //设置user元素的属性。
    userElement->SetAttribute("active", "true");

    TiXmlElement *tempElement = new TiXmlElement("nickname");
    userElement->LinkEndChild(tempElement);
    TiXmlText *tempContent = new TiXmlText("your nick name");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("name");
    userElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("your user name");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("password");
    userElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("your password");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("fakeAddress");
    userElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("your IP address");
    tempElement->LinkEndChild(tempContent);

    TiXmlElement *configElement = new TiXmlElement("config");
    RootElement->LinkEndChild(configElement);

    tempElement = new TiXmlElement("autologin");
    configElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("false");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("authenticationMode");
    configElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("0");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("nic");
    configElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("eth0");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("echoInterval");
    configElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("4");
    tempElement->LinkEndChild(tempContent);

    tempElement = new TiXmlElement("intelligentReconnect");
    configElement->LinkEndChild(tempElement);
    tempContent = new TiXmlText("0");
    tempElement->LinkEndChild(tempContent);

    myDocument->SaveFile(filename);
    cout<<"初始化配置文件到"<<filename<<endl;
}
int Mystar::save_file()
{
    TiXmlDocument mystarconf(filename);
    if(mystarconf.LoadFile())
    {

        TiXmlElement* mystarroot = mystarconf.RootElement();
        if(mystarroot == NULL)
            return 1;
        TiXmlElement* UsersElement = mystarroot->FirstChildElement();
        if(UsersElement==NULL)
            return 1;
        TiXmlElement* UserElement = UsersElement->FirstChildElement();
        if(UserElement==NULL)
            return 1;
        TiXmlElement *nickname = UserElement->FirstChildElement();
        TiXmlElement *username = nickname->NextSiblingElement();
        TiXmlElement *password = username->NextSiblingElement();
        TiXmlElement *fakeAddress = password->NextSiblingElement();

        TiXmlText *tempContent = new TiXmlText(user.get_nickname());
        nickname->ReplaceChild(nickname->FirstChild(), *tempContent);

        tempContent = new TiXmlText(user.get_username());
        username->ReplaceChild(username->FirstChild(), *tempContent);

        tempContent = new TiXmlText(user.get_password());
        password->ReplaceChild(password->FirstChild(), *tempContent);

        tempContent = new TiXmlText(user.get_fakeAddress());
        fakeAddress->ReplaceChild(fakeAddress->FirstChild(), *tempContent);

        TiXmlElement *config = UsersElement->NextSiblingElement();
        TiXmlElement *autologinElement = config->FirstChildElement();

        if(autologin)
            tempContent = new TiXmlText("true");
        else
            tempContent = new TiXmlText("false");
        autologinElement->ReplaceChild(autologinElement->FirstChild(), *tempContent);

        cout<<"save to "<<filename<<endl;
        mystarconf.SaveFile();
        return 0;
    }
}
