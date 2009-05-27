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
#ifndef KSTAR_H
#define KSTAR_H

#include "kstarbase.h"
#include "myinifile.h"
#include "mystar.h"
#include "trayicon.h"

class KStar: public KStarBase {
Q_OBJECT
public:
    KStar(QWidget *parent = 0, const char *name = 0);
    void statusMsg( char * str);
    void outputMsg( char *str);

	CIniFile  *    configfile;
	StarConfig   starconfig;
	void InitConfig();
	void SaveConfig();
	bool ReadNic();
	void ReadNicEnum();
	void setConfigNic();

    	QPopupMenu *menu;
	TrayIcon * tray;
	QTimer *  authTimer;
	QTimer *  echoTimer;
	StarAuth *  sa;

public slots:
	void RunSA();
	void RunEcho();	
	void ShowHelp();

public slots:
    virtual void aboutButton_clicked();
public slots:
    virtual void exitButton_clicked();
public slots:
    virtual void disconnectButton_clicked();
public slots:
    virtual void connectButton_clicked();
    virtual void simpleCheckBox_stateChanged(int);
    virtual void rememberCheckBox_stateChanged(int);
    virtual void echoIntervalSpinBox_valueChanged(const QString&);
    virtual void intelligentCheckBox_stateChanged(int);
    virtual void autoMinimizeCheckBox_stateChanged(int);
    virtual void autoConnectCheckBox_stateChanged(int);
    virtual void autoStartCheckBox_stateChanged(int);
    virtual void fakeAddressLineEdit_lostFocus();
    virtual void fakeAddressCheckBox_stateChanged(int);
    virtual void passwordLineEdit_textChanged(const QString&);
    virtual void usernameLineEdit_textChanged(const QString&);
    virtual void fakeAddressLineEdit_textChanged(const QString&);
    virtual void authModeComboBox_activated(int);
    virtual void nicComboBox_activated(int);

private:
    bool event(QEvent *);

};

#endif
