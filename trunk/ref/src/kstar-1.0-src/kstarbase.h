/****************************************************************************
** Form interface generated from reading ui file 'kstarbase.ui'
**
** Created: 日  2月 8 13:03:00 2009
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef KSTARBASE_H
#define KSTARBASE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QLabel;
class QPushButton;
class QCheckBox;
class QLineEdit;
class QComboBox;
class QTabWidget;
class QTextEdit;
class QSpinBox;

class KStarBase : public QWidget
{
    Q_OBJECT

public:
    KStarBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~KStarBase();

    QGroupBox* basicGroupBox;
    QLabel* passwordTextLabel;
    QLabel* nicTextLabel;
    QPushButton* connectButton;
    QPushButton* disconnectButton;
    QPushButton* exitButton;
    QCheckBox* simpleCheckBox;
    QLabel* userNameTextLabel;
    QLineEdit* usernameLineEdit;
    QLineEdit* passwordLineEdit;
    QCheckBox* rememberCheckBox;
    QComboBox* nicComboBox;
    QLabel* statusTextLabel;
    QTabWidget* advanceTabWidget;
    QWidget* outputTab;
    QTextEdit* outputTextEdit;
    QWidget* optionTab;
    QLabel* echo2TextLabel;
    QLabel* echoTextLabel;
    QLabel* fakeAddressTextLabel;
    QCheckBox* autoStartCheckBox;
    QCheckBox* autoConnectCheckBox;
    QCheckBox* autoMinimizeCheckBox;
    QCheckBox* intelligentCheckBox;
    QCheckBox* fakeAddressCheckBox;
    QSpinBox* echoIntervalSpinBox;
    QLabel* authModeTextLabel;
    QLineEdit* fakeAddressLineEdit;
    QComboBox* authModeComboBox;
    QWidget* aboutTab;
    QPushButton* aboutButton;
    QTextEdit* helpTextEdit;

public slots:
    virtual void connectButton_clicked();
    virtual void disconnectButton_clicked();
    virtual void exitButton_clicked();
    virtual void aboutButton_clicked();
    virtual void simpleCheckBox_stateChanged(int);
    virtual void fakeAddressCheckBox_stateChanged(int);
    virtual void fakeAddressLineEdit_lostFocus();
    virtual void autoStartCheckBox_stateChanged(int);
    virtual void autoConnectCheckBox_stateChanged(int);
    virtual void autoMinimizeCheckBox_stateChanged(int);
    virtual void intelligentCheckBox_stateChanged(int);
    virtual void echoIntervalSpinBox_valueChanged(const QString&);
    virtual void rememberCheckBox_stateChanged(int);
    virtual void usernameLineEdit_textChanged(const QString&);
    virtual void passwordLineEdit_textChanged(const QString&);
    virtual void nicComboBox_textChanged(const QString&);
    virtual void authModeComboBox_textChanged(const QString&);
    virtual void fakeAddressLineEdit_textChanged(const QString&);
    virtual void nicComboBox_activated(int);
    virtual void authModeComboBox_activated(int);
    virtual void advanceTabWidget_selected(const QString&);

protected:

protected slots:
    virtual void languageChange();

};

#endif // KSTARBASE_H
