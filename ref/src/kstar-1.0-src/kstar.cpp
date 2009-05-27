/***************************************************************************
 *   Copyright (C) 2008 by duanp   *
 *   duanp@163.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "kstar.h"

#include <qapplication.h>
#include <qvariant.h>
#include <qtabwidget.h>
#include <qtextedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qmessagebox.h>
#include <qtimer.h>
#include <qpopupmenu.h>
#include <qfile.h>
#include <qregexp.h>


#define CONF_FILE  "/etc/kstar.ini" 
#define COPY_FILE   "README"
#define GPL_FILE "/usr/share/apps/LICENSES/GPL_V2"
#define AUTO_FILE   "/etc/xdg/autostart/kstar.desktop"

#define CONF_ROOT "KStar"
#define MAXINTERFACES 10

static const char * const icon_xpm[] = {
"12 12 2 1",
"# c #000000",
". c None",
"............",
"....######..",
"....######..",
"....#....#..",
"..######.#..",
"..######.#..",
"..#....###..",
"..#....#....",
"..#....#....",
"..######....",
"............",
"............"};

bool KStar::event(QEvent * e)
{
	if(e->type()==QEvent::ShowMinimized)	
	{
		this->hide();
		tray->show();
		return true;
	}
	return QWidget::event(e);
}

KStar::KStar(QWidget *parent, const char *name)
    :KStarBase(parent, name)
{
	simpleCheckBox_stateChanged(0);	
	move(QPoint(400,400));	
	usernameLineEdit->setFocus();
	setTabOrder(usernameLineEdit,passwordLineEdit);
	setTabOrder(passwordLineEdit,nicComboBox);
	setTabOrder(nicComboBox,connectButton);
	setTabOrder(connectButton,disconnectButton);
	setTabOrder(disconnectButton,exitButton);
	setTabOrder(exitButton,rememberCheckBox);
	setTabOrder(rememberCheckBox,simpleCheckBox);	

	//Init UI
	if(!ReadNic())  ReadNicEnum();	
	InitConfig();	
	setConfigNic();	

	sa=new StarAuth(&starconfig,outputTextEdit);

	authTimer=new QTimer(this);
	echoTimer=new QTimer(this);
	
	connect(authTimer,SIGNAL(timeout()),this,SLOT(RunSA()));
	connect(echoTimer,SIGNAL(timeout()),this,SLOT(RunEcho()));

	menu=new QPopupMenu();
	menu->insertItem( trUtf8("显示&KStar窗口"), this, SLOT(showNormal()) );
	menu->insertItem( trUtf8("关于&A"), this, SLOT(aboutButton_clicked()) );
	menu->insertSeparator();
	menu->insertItem( trUtf8("退出&Q"), this, SLOT(exitButton_clicked()) );

       tray=new TrayIcon( QPixmap( (const char**)icon_xpm ), trUtf8("打开KStar认证程序！"), menu );
       connect(tray,SIGNAL(clicked(const QPoint&,int)),this,SLOT(showNormal()));
	

	disconnectButton->setEnabled(0);	
	ShowHelp();

        if(starconfig.isAutoConnect) {connectButton_clicked();}

}

void KStar::InitConfig()
{
		
	configfile=new CIniFile(CONF_FILE);	

	/*Init this char array is very Important!!!!!!!!! if not fatal Error!!*/
	starconfig.userName=new char [64];
	starconfig.passWord=new char [64];;
	starconfig.nic=new char [64];
	starconfig.authMode=0;
	starconfig.echoInterval=5;
	starconfig.isIntelligentReconnect=0;
	starconfig.isRemember=0;
	starconfig.isFakeInUse=0;
	starconfig.isAutoRun=0;
	starconfig.isAutoConnect=0;
	starconfig.isAutoMinimize=0;
	for(int i=0;i<4;i++) starconfig.fakeAddress[i]=0;	

	if(configfile->ExistSect(CONF_ROOT))
	{		
	
		if(configfile->ExistKey(CONF_ROOT,"UserName"))
		{	
			configfile->ReadString(CONF_ROOT,"UserName",starconfig.userName);
			usernameLineEdit->setText(starconfig.userName);
		}else { configfile->CreateKey(CONF_ROOT,"UserName","") ;}

		if(configfile->ExistKey(CONF_ROOT,"IsRemember"))
		{			
			if(configfile->ReadBool(CONF_ROOT,"IsRemember",0))
			{
				if(configfile->ExistKey(CONF_ROOT,"PassWord"))
				{configfile->ReadString(CONF_ROOT,"PassWord",starconfig.passWord);}
				else { configfile->CreateKey(CONF_ROOT,"PassWord","") ;}
		
				passwordLineEdit->setText(starconfig.passWord);
	
				starconfig.isRemember=1;
				rememberCheckBox->setChecked(1);
			}else {rememberCheckBox->setChecked(0);}
		}else { configfile->CreateKey(CONF_ROOT,"IsRemember","0") ;}

		if(configfile->ExistKey(CONF_ROOT,"IsAutoConnect"))
		{	
			if(configfile->ReadBool(CONF_ROOT,"IsAutoConnect",0))
			{
				starconfig.isAutoConnect=1;
				autoConnectCheckBox->setChecked(1);
			}else {autoConnectCheckBox->setChecked(0);}
		}else { configfile->CreateKey(CONF_ROOT,"IsAutoConnect","0") ;}

		if(configfile->ExistKey(CONF_ROOT,"IsIntelligentReconnect"))
		{	
			if(configfile->ReadBool(CONF_ROOT,"IsIntelligentReconnect",0))
			{
				starconfig.isIntelligentReconnect=1;
				intelligentCheckBox->setChecked(1);
			}else {intelligentCheckBox->setChecked(0);}
		}else { configfile->CreateKey(CONF_ROOT,"IsIntelligentReconnect","1") ;}

		if(configfile->ExistKey(CONF_ROOT,"IsAutoMinimize"))
		{	
			if(configfile->ReadBool(CONF_ROOT,"IsAutoMinimize",0))
			{
				starconfig.isAutoMinimize=1;
				autoMinimizeCheckBox->setChecked(1);
			}else {autoMinimizeCheckBox->setChecked(0);}
		}else { configfile->CreateKey(CONF_ROOT,"IsAutoMinimize","0") ;}

		if(configfile->ExistKey(CONF_ROOT,"IsAutoRun"))
		{	
			if(configfile->ReadBool(CONF_ROOT,"IsAutoRun",0))
			{
				starconfig.isAutoRun=1;
				autoStartCheckBox->setChecked(1);
			}else {autoStartCheckBox->setChecked(0);}
		}else { configfile->CreateKey(CONF_ROOT,"IsAutoRun","0") ;}


		char * str_address=new char[64];
		if(configfile->ExistKey(CONF_ROOT,"FakeAddress"))
		{					
			configfile->ReadString(CONF_ROOT,"FakeAddress",str_address);
			//cout<<str_address<<endl;
			QRegExp rx("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$");
			if(rx.search(str_address)!=-1)  // indexIn() in QT4
			 {    
				for(int i=0;i<4;i++) {
						starconfig.fakeAddress[i]=(unsigned char)(rx.cap(i+1).toUShort());
				}		
			 }
		}
		else { configfile->CreateKey(CONF_ROOT,"FakeAddress","") ;}
		fakeAddressLineEdit->setText(str_address);	
		fakeAddressLineEdit->setEnabled(0);


		if(configfile->ExistKey(CONF_ROOT,"IsFakeInUse"))
		{	
			if(configfile->ReadBool(CONF_ROOT,"IsFakeInUse",0))
			{
				starconfig.isFakeInUse=1;
				fakeAddressCheckBox->setChecked(1);
				fakeAddressLineEdit->setEnabled(1);
			}else {fakeAddressCheckBox->setChecked(0);}
		}else { configfile->CreateKey(CONF_ROOT,"IsFakeInUse","0.0.0.0") ;}

		/*echo    %    authMode   %   nic   */	
		if(configfile->ExistKey(CONF_ROOT,"EchoInterval"))
		{
			starconfig.echoInterval=configfile->ReadInteger(CONF_ROOT,"EchoInterval",0);
			if(starconfig.echoInterval>echoIntervalSpinBox->maxValue())
			{	
				starconfig.echoInterval=echoIntervalSpinBox->maxValue();		
			}
			else if (starconfig.echoInterval<echoIntervalSpinBox->minValue())
			{
				starconfig.echoInterval=echoIntervalSpinBox->minValue();
			}
			echoIntervalSpinBox->setValue(starconfig.echoInterval);
		}
		else { configfile->CreateKey(CONF_ROOT,"EchoInterval","0") ;}

		if(configfile->ExistKey(CONF_ROOT,"AuthMode"))
		{
			starconfig.authMode=configfile->ReadInteger(CONF_ROOT,"AuthMode",0);
			if(starconfig.authMode>(authModeComboBox->count()-1))
			{	
				starconfig.authMode=(authModeComboBox->count()-1);		
			}
			else if (starconfig.authMode<0)
			{
				starconfig.authMode=0;
			}
			authModeComboBox->setCurrentItem(starconfig.authMode);
		}
		else { configfile->CreateKey(CONF_ROOT,"AuthMode","0") ;}

		if(configfile->ExistKey(CONF_ROOT,"Nic"))
		{
			configfile->ReadString(CONF_ROOT,"Nic",starconfig.nic);			
			int idx;
			for(idx=0;idx<nicComboBox->count();idx++) 
				{
					 if((nicComboBox->text(idx)).contains(starconfig.nic)) break; 
				}
			if(idx<nicComboBox->count()) nicComboBox->setCurrentItem(idx);
		}
		else { configfile->CreateKey(CONF_ROOT,"Nic","eth0") ;}				

	}
	else
	{
		configfile->CreateSect(CONF_ROOT);
		configfile->CreateKey(CONF_ROOT,"UserName","") ;
		configfile->CreateKey(CONF_ROOT,"PassWord","") ;
 		configfile->CreateKey(CONF_ROOT,"IsRemember","0") ;
 		configfile->CreateKey(CONF_ROOT,"IsIntelligentReconnect","0") ;
		configfile->CreateKey(CONF_ROOT,"IsAutoConnect","0") ;
		configfile->CreateKey(CONF_ROOT,"IsAutoRun","0") ;
		configfile->CreateKey(CONF_ROOT,"IsAutoMinimize","0") ;
		configfile->CreateKey(CONF_ROOT,"IsFakeInUse","0") ;
		configfile->CreateKey(CONF_ROOT,"FakeAddress","0.0.0.0");
		configfile->CreateKey(CONF_ROOT,"EchoInterval","5") ;
		configfile->CreateKey(CONF_ROOT,"AuthMode","0") ;
		configfile->CreateKey(CONF_ROOT,"Nic","eth0") ;
	}
	
}

void KStar::SaveConfig()
{
		char str[50]="";
		configfile->UpdateKey(CONF_ROOT,"UserName",starconfig.userName) ;
		sprintf(str,"%d",starconfig.isRemember);
 		configfile->UpdateKey(CONF_ROOT,"IsRemember",str) ;
		sprintf(str,"%d",starconfig.isIntelligentReconnect);
 		configfile->UpdateKey(CONF_ROOT,"IsIntelligentReconnect",str) ;
		sprintf(str,"%d",starconfig.isAutoConnect);
		configfile->UpdateKey(CONF_ROOT,"IsAutoConnect",str) ;
		sprintf(str,"%d",starconfig.isAutoRun);
		configfile->UpdateKey(CONF_ROOT,"IsAutoRun",str) ;
		sprintf(str,"%d",starconfig.isAutoMinimize);
		configfile->UpdateKey(CONF_ROOT,"IsAutoMinimize",str) ;
		sprintf(str,"%d",starconfig.isFakeInUse);
		configfile->UpdateKey(CONF_ROOT,"IsFakeInUse",str) ;
		 sprintf(str,"%d.%d.%d.%d",starconfig.fakeAddress[0],
				starconfig.fakeAddress[1],starconfig.fakeAddress[2],starconfig.fakeAddress[3]);
		configfile->UpdateKey(CONF_ROOT,"FakeAddress",str);
		sprintf(str,"%d",starconfig.echoInterval);
		configfile->UpdateKey(CONF_ROOT,"EchoInterval",str) ;
		sprintf(str,"%d",starconfig.authMode);
		configfile->UpdateKey(CONF_ROOT,"AuthMode",str) ;
		configfile->UpdateKey(CONF_ROOT,"Nic",starconfig.nic) ;	

		if(starconfig.isRemember)
		{
			configfile->UpdateKey(CONF_ROOT,"PassWord",starconfig.passWord) ;
		}else{
			configfile->UpdateKey(CONF_ROOT,"PassWord","") ;
		}

		if(starconfig.isAutoRun)
		{
			QString str;
			str+="\"[Desktop Entry]\n";
			str+="Name=KStar\n";
			str+="Comment=8021.1 Auth\n";				
			str+="Exec=";str+=qApp->argv()[0];str+="\n";
			str+="X-GNOME-Autostart-enabled=true\n";
			str+="Type=Application\r\n\"";
			
			if(system(QString("echo ")+str+">"+AUTO_FILE)!=0)  //正常为0
			cout<<"config add autorun result error!"<<endl;
		}
		else
		{	if(system(QString("rm -f ")+AUTO_FILE)!=0)  //正常为0
			cout<<"config del autorun result error!"<<endl;
		}	

}

void KStar::statusMsg(char * str)
{
	statusTextLabel->setText(trUtf8(str));	
}

void KStar::outputMsg(char * str)
{	
	outputTextEdit->append(trUtf8(str));	
}

void KStar::connectButton_clicked()
{	
	//获取当前控件中的参数
/*	outputMsg(starconfig.userName);
	outputMsg(starconfig.passWord);
	outputMsg(starconfig.nic); */

	if(starconfig.userName==NULL
	||starconfig.passWord==NULL
	||starconfig.nic==NULL
	||(!strcmp(starconfig.userName,""))
	||(!strcmp(starconfig.passWord,"")))
	{
		QMessageBox::warning(this,trUtf8("输入错误"),trUtf8("用户名或者密码不能为空！"),1,0);
	}else{
		if(authTimer->isActive()){authTimer->stop();}
		statusMsg("正在连接...");
		sa->Init();
		authTimer->start(100,TRUE);  //TRUE for RunOnce
	}

}

void KStar::RunSA()
{	
	if(authTimer->isActive()) authTimer->stop(); 
	connectButton->setEnabled(0);
	sa->Run();
	if(sa->authState==auth_ok) {
		statusMsg("连接成功！");
		SaveConfig();
		if(starconfig.echoInterval>0){
		statusMsg("连接成功！定时激活中...");
		cout<<"Start Echo.............."<<endl;
		outputMsg("Start Echo..............");
		echoTimer->start(starconfig.echoInterval*1000,FALSE);
		}
		disconnectButton->setEnabled(1);

		if(starconfig.isAutoMinimize) {this->hide();}
	}
	else
	{
		
		statusMsg("连接失败！");
		if(starconfig.isIntelligentReconnect){
			cout<<"Reset Connecting"<<endl;
			outputMsg(" 重启认证...!");				
			authTimer->start(5000,TRUE);  //TRUE for RunOnce
		}else{
			connectButton->setEnabled(1);
		}
	
		
	}
}

void KStar::RunEcho()
{
	cout<<"Echo......"<<endl;
	sa->RunEcho();
}

void KStar::disconnectButton_clicked()
{
	if(authTimer->isActive()){ authTimer->stop() ;  } 
	if(echoTimer->isActive()){ echoTimer->stop() ;  } 	

	sa->RunBye();
	statusMsg("连接已断开！");
	outputMsg("Loged out successflly!");		
	connectButton->setEnabled(1);	
	disconnectButton->setEnabled(0);	
}

void KStar::exitButton_clicked()
{
	SaveConfig();
	disconnectButton_clicked();
	qApp->quit();
}

void KStar::aboutButton_clicked()
{
	QString abt="KStar Version 1.0  based on mystar \nPowered by Febird(duanp) 2008\n Email: duanp@163.com ";
	QMessageBox::about(this,trUtf8("关于KStar"),trUtf8(abt));
}

void KStar::simpleCheckBox_stateChanged(int)
{
	if(simpleCheckBox->isChecked())
	{
		advanceTabWidget->hide();
		this->setFixedSize(QSize(492,221));		
	}
       else
	{	
		advanceTabWidget->show();
		this->setFixedSize(QSize(492,441));		
	}
	
}

void KStar::fakeAddressCheckBox_stateChanged(int ck)
{
		starconfig.isFakeInUse=ck;
		fakeAddressLineEdit->setEnabled(ck);

}

void KStar::fakeAddressLineEdit_lostFocus()
{
	//check validable
	QRegExp rx("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$");
	if(rx.search(fakeAddressLineEdit->text())!=-1)  // indexIn() in QT4
	{    
		for(int i=0;i<4;i++) {
			starconfig.fakeAddress[i]=(unsigned char)(rx.cap(i+1).toUShort());
		}		
	}
	else
	{
		if(QMessageBox::warning(this,trUtf8("提示"),trUtf8("IP输入错误!"),
						trUtf8("返回修改"),trUtf8("取消"))==0) // first button
		{
			fakeAddressLineEdit->setFocus();
		}else
		{
			fakeAddressLineEdit->setText("");
			fakeAddressLineEdit->setEnabled(0);
			fakeAddressCheckBox->setChecked(0);
		}
		
	}
}

void KStar::autoStartCheckBox_stateChanged(int ck)
{
		starconfig.isAutoRun=ck;		
}

void KStar::autoConnectCheckBox_stateChanged(int ck)
{
		starconfig.isAutoConnect=ck;
}

void KStar::autoMinimizeCheckBox_stateChanged(int ck)
{
		starconfig.isAutoMinimize=ck;
}

void KStar::intelligentCheckBox_stateChanged(int ck)
{
		starconfig.isIntelligentReconnect=ck;
}

void KStar::echoIntervalSpinBox_valueChanged(const QString&)
{
	starconfig.echoInterval=echoIntervalSpinBox->value();
}

void KStar::rememberCheckBox_stateChanged(int)
{
	if(rememberCheckBox->isChecked())
	{
		starconfig.isRemember=1;

	}else{
		starconfig.isRemember=0;
		passwordLineEdit->setText("");
	}	
}

void KStar::usernameLineEdit_textChanged(const QString& s)
{
	starconfig.userName=strdup(s);
}

void KStar::passwordLineEdit_textChanged(const QString&  s)
{
	starconfig.passWord=strdup(s);
}



void KStar::setConfigNic()
{
	QString   str=nicComboBox->text(nicComboBox->currentItem());
	starconfig.nic=strdup(str.left(str.find(',',0,TRUE)));
}

void KStar::fakeAddressLineEdit_textChanged(const QString&)
{
}

void KStar::nicComboBox_activated(int)
{
	setConfigNic();	
	sa->Init();
}

void KStar::authModeComboBox_activated(int)
{
	starconfig.authMode=authModeComboBox->currentItem();
}

void KStar::ReadNicEnum()
{
	nicComboBox->insertItem("eth0,[ try it !! ]");
	nicComboBox->insertItem("eth1,[ try it !! ]");
	nicComboBox->insertItem("eth3,[ try it !! ]");
	nicComboBox->insertItem("eth4,[ try it !! ]");
}

bool KStar::ReadNic()
{
	register int fd, interface=0;
	struct ifreq buf[MAXINTERFACES];
//      struct arpreq arp;
	struct ifconf ifc;
//	char mac[32] = "";


	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) {
		ifc.ifc_len = sizeof(buf);
		ifc.ifc_buf = (caddr_t) buf;
		if (!ioctl(fd, SIOCGIFCONF, (char *) &ifc))
		{
			interface = ifc.ifc_len / sizeof(struct ifreq);		
		}
	}


	if(interface<1)	return 0;	

	for(int i=0;i<interface;i++) {
	
		/*Jugde whether the net card status is promisc */	
               /*if (!(ioctl(fd, SIOCGIFFLAGS, (char *) &buf[interface]))) {
                    if (buf[interface].ifr_flags & IFF_PROMISC) {
                        printf("the interface is PROMISC");
                        retn ;
                    }
                } else {
                    char str[256] = ""; 
                    sprintf(str, "cpm: ioctl device %s",
                            buf[interface].ifr_name);
                    perror(str);
                }*/
 
		/*Jugde whether the net card status is up */
             /*  if (buf[i].ifr_flags & IFF_UP) {
                     ; // nic  is up  then get  name and ip
                } else {
                    continue;
                }*/

	//Get Nics ' Name and IP  and  load  them to the Combox
		char nicstr[256] = "";		
		/*Get IP of the net card */
                if (!(ioctl(fd, SIOCGIFADDR, (char *) &buf[i]))) {
                    sprintf(nicstr,"%s,[%s]",buf[i].ifr_name,
					inet_ntoa(((struct sockaddr_in*)(&buf[i].ifr_addr))->sin_addr));
                } else {
                    sprintf(nicstr,"%s,[cant get ip!]",buf[i].ifr_name);
                }

		nicComboBox->insertItem(nicstr);

                /*Get HW ADDRESS of the net card */
                /*if (!(ioctl(fd, SIOCGIFHWADDR, (char *) &buf[interface]))) {
                    printf("HW address is:");
 
                    sprintf(mac, "%02x%02x%02x%02x%02x%02x",
                            (unsigned char) buf[i].ifr_hwaddr.
                            sa_data[0],
                            (unsigned char) buf[i].ifr_hwaddr.
                            sa_data[1],
                            (unsigned char) buf[i].ifr_hwaddr.
                            sa_data[2],
                            (unsigned char) buf[i].ifr_hwaddr.
                            sa_data[3],
                            (unsigned char) buf[i].ifr_hwaddr.
                            sa_data[4],
                            (unsigned char) buf[i].ifr_hwaddr.
                            sa_data[5]); // 利用sprintf转换成char * 
                    printf("%s\n", mac);
 
                    printf("\n");
                }
 
                else {
                    char str[256];
 
                    sprintf(str, "cpm: ioctl device %s",
                            buf[interface].ifr_name); 
                    perror(str);
                }  */


	}
	//close(fd); 
	return 1;
}

void KStar::ShowHelp()
{
    QFile f(qApp->applicationDirPath()+"/"+COPY_FILE);
    if(!f.exists()) { f.setName(GPL_FILE);}
    if ( f.open(IO_ReadOnly) ) {
        QTextStream t( &f );
        QString s;
        while ( !t.eof() ) {
            s = t.readLine();
	     helpTextEdit->append(s);
        }
        f.close();	
    }	
}



