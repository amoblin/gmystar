/****************************************************************************
** Form implementation generated from reading ui file 'kstarbase.ui'
**
** Created: Sun Feb 8 14:47:50 2009
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "kstarbase.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qtabwidget.h>
#include <qtextedit.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a KStarBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
KStarBase::KStarBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "KStarBase" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 492, 221 ) );
    setMaximumSize( QSize( 492, 441 ) );

    basicGroupBox = new QGroupBox( this, "basicGroupBox" );
    basicGroupBox->setGeometry( QRect( 10, 10, 471, 170 ) );

    passwordTextLabel = new QLabel( basicGroupBox, "passwordTextLabel" );
    passwordTextLabel->setGeometry( QRect( 40, 70, 40, 20 ) );

    nicTextLabel = new QLabel( basicGroupBox, "nicTextLabel" );
    nicTextLabel->setGeometry( QRect( 30, 110, 40, 20 ) );

    connectButton = new QPushButton( basicGroupBox, "connectButton" );
    connectButton->setGeometry( QRect( 340, 30, 111, 30 ) );

    disconnectButton = new QPushButton( basicGroupBox, "disconnectButton" );
    disconnectButton->setGeometry( QRect( 340, 70, 111, 30 ) );

    exitButton = new QPushButton( basicGroupBox, "exitButton" );
    exitButton->setGeometry( QRect( 340, 110, 111, 30 ) );

    simpleCheckBox = new QCheckBox( basicGroupBox, "simpleCheckBox" );
    simpleCheckBox->setGeometry( QRect( 200, 140, 81, 21 ) );
    simpleCheckBox->setChecked( TRUE );

    userNameTextLabel = new QLabel( basicGroupBox, "userNameTextLabel" );
    userNameTextLabel->setGeometry( QRect( 30, 30, 50, 20 ) );

    usernameLineEdit = new QLineEdit( basicGroupBox, "usernameLineEdit" );
    usernameLineEdit->setGeometry( QRect( 90, 30, 220, 24 ) );

    passwordLineEdit = new QLineEdit( basicGroupBox, "passwordLineEdit" );
    passwordLineEdit->setGeometry( QRect( 90, 70, 220, 24 ) );
    passwordLineEdit->setEchoMode( QLineEdit::Password );

    rememberCheckBox = new QCheckBox( basicGroupBox, "rememberCheckBox" );
    rememberCheckBox->setGeometry( QRect( 100, 140, 81, 21 ) );
    rememberCheckBox->setChecked( TRUE );

    nicComboBox = new QComboBox( FALSE, basicGroupBox, "nicComboBox" );
    nicComboBox->setGeometry( QRect( 90, 110, 220, 24 ) );

    statusTextLabel = new QLabel( this, "statusTextLabel" );
    statusTextLabel->setGeometry( QRect( 10, 190, 330, 22 ) );
    QFont statusTextLabel_font(  statusTextLabel->font() );
    statusTextLabel_font.setFamily( "文鼎PL中楷Uni" );
    statusTextLabel_font.setPointSize( 14 );
    statusTextLabel->setFont( statusTextLabel_font ); 

    advanceTabWidget = new QTabWidget( this, "advanceTabWidget" );
    advanceTabWidget->setGeometry( QRect( 10, 230, 471, 200 ) );

    outputTab = new QWidget( advanceTabWidget, "outputTab" );

    outputTextEdit = new QTextEdit( outputTab, "outputTextEdit" );
    outputTextEdit->setGeometry( QRect( 8, 13, 450, 140 ) );
    outputTextEdit->setReadOnly( TRUE );
    advanceTabWidget->insertTab( outputTab, QString::fromLatin1("") );

    optionTab = new QWidget( advanceTabWidget, "optionTab" );

    echo2TextLabel = new QLabel( optionTab, "echo2TextLabel" );
    echo2TextLabel->setGeometry( QRect( 390, 20, 50, 20 ) );

    echoTextLabel = new QLabel( optionTab, "echoTextLabel" );
    echoTextLabel->setGeometry( QRect( 220, 20, 80, 20 ) );

    fakeAddressTextLabel = new QLabel( optionTab, "fakeAddressTextLabel" );
    fakeAddressTextLabel->setGeometry( QRect( 230, 110, 30, 30 ) );

    autoStartCheckBox = new QCheckBox( optionTab, "autoStartCheckBox" );
    autoStartCheckBox->setGeometry( QRect( 20, 20, 83, 20 ) );

    autoConnectCheckBox = new QCheckBox( optionTab, "autoConnectCheckBox" );
    autoConnectCheckBox->setGeometry( QRect( 20, 50, 83, 20 ) );

    autoMinimizeCheckBox = new QCheckBox( optionTab, "autoMinimizeCheckBox" );
    autoMinimizeCheckBox->setGeometry( QRect( 20, 80, 150, 20 ) );

    intelligentCheckBox = new QCheckBox( optionTab, "intelligentCheckBox" );
    intelligentCheckBox->setGeometry( QRect( 20, 110, 120, 20 ) );

    fakeAddressCheckBox = new QCheckBox( optionTab, "fakeAddressCheckBox" );
    fakeAddressCheckBox->setGeometry( QRect( 370, 90, 70, 20 ) );

    echoIntervalSpinBox = new QSpinBox( optionTab, "echoIntervalSpinBox" );
    echoIntervalSpinBox->setGeometry( QRect( 320, 20, 50, 22 ) );
    echoIntervalSpinBox->setMaxValue( 60 );

    authModeTextLabel = new QLabel( optionTab, "authModeTextLabel" );
    authModeTextLabel->setGeometry( QRect( 220, 56, 80, 20 ) );

    fakeAddressLineEdit = new QLineEdit( optionTab, "fakeAddressLineEdit" );
    fakeAddressLineEdit->setGeometry( QRect( 289, 113, 150, 21 ) );

    authModeComboBox = new QComboBox( FALSE, optionTab, "authModeComboBox" );
    authModeComboBox->setGeometry( QRect( 310, 60, 130, 21 ) );
    advanceTabWidget->insertTab( optionTab, QString::fromLatin1("") );

    aboutTab = new QWidget( advanceTabWidget, "aboutTab" );

    aboutButton = new QPushButton( aboutTab, "aboutButton" );
    aboutButton->setGeometry( QRect( 360, 140, 91, 21 ) );

    helpTextEdit = new QTextEdit( aboutTab, "helpTextEdit" );
    helpTextEdit->setGeometry( QRect( 8, 13, 451, 121 ) );
    helpTextEdit->setReadOnly( TRUE );
    advanceTabWidget->insertTab( aboutTab, QString::fromLatin1("") );
    languageChange();
    resize( QSize(492, 441).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( aboutButton, SIGNAL( clicked() ), this, SLOT( aboutButton_clicked() ) );
    connect( authModeComboBox, SIGNAL( activated(int) ), this, SLOT( authModeComboBox_activated(int) ) );
    connect( autoConnectCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( autoConnectCheckBox_stateChanged(int) ) );
    connect( autoMinimizeCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( autoMinimizeCheckBox_stateChanged(int) ) );
    connect( autoStartCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( autoStartCheckBox_stateChanged(int) ) );
    connect( connectButton, SIGNAL( clicked() ), this, SLOT( connectButton_clicked() ) );
    connect( disconnectButton, SIGNAL( clicked() ), this, SLOT( disconnectButton_clicked() ) );
    connect( echoIntervalSpinBox, SIGNAL( valueChanged(const QString&) ), this, SLOT( echoIntervalSpinBox_valueChanged(const QString&) ) );
    connect( exitButton, SIGNAL( clicked() ), this, SLOT( exitButton_clicked() ) );
    connect( fakeAddressCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( fakeAddressCheckBox_stateChanged(int) ) );
    connect( fakeAddressLineEdit, SIGNAL( lostFocus() ), this, SLOT( fakeAddressLineEdit_lostFocus() ) );
    connect( fakeAddressLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( fakeAddressLineEdit_textChanged(const QString&) ) );
    connect( intelligentCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( intelligentCheckBox_stateChanged(int) ) );
    connect( nicComboBox, SIGNAL( activated(int) ), this, SLOT( nicComboBox_activated(int) ) );
    connect( passwordLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( passwordLineEdit_textChanged(const QString&) ) );
    connect( rememberCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( rememberCheckBox_stateChanged(int) ) );
    connect( simpleCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( simpleCheckBox_stateChanged(int) ) );
    connect( usernameLineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( usernameLineEdit_textChanged(const QString&) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
KStarBase::~KStarBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KStarBase::languageChange()
{
    setCaption( tr( "KStar for Linux" ) );
    basicGroupBox->setTitle( trUtf8( "\xe5\x9f\xba\xe6\x9c\xac\xe4\xbf\xa1\xe6\x81\xaf" ) );
    passwordTextLabel->setText( trUtf8( "\xe5\xaf\x86\xe7\xa0\x81\x3a" ) );
    nicTextLabel->setText( trUtf8( "\xe7\xbd\x91\xe7\xbb\x9c\xe5\x8d\xa1\x3a" ) );
    connectButton->setText( trUtf8( "\xe8\xbf\x9e\xe6\x8e\xa5\xe8\xae\xa4\xe8\xaf\x81" ) );
    disconnectButton->setText( trUtf8( "\xe6\x96\xad\xe5\xbc\x80\xe8\xbf\x9e\xe6\x8e\xa5" ) );
    exitButton->setText( trUtf8( "\xe9\x80\x80\xe5\x87\xba\xe7\xb3\xbb\xe7\xbb\x9f" ) );
    simpleCheckBox->setText( trUtf8( "\xe7\xb2\xbe\xe7\xae\x80\xe6\xa8\xa1\xe5\xbc\x8f" ) );
    userNameTextLabel->setText( trUtf8( "\xe7\x94\xa8\xe6\x88\xb7\xe5\x90\x8d\x3a" ) );
    rememberCheckBox->setText( trUtf8( "\xe8\xae\xb0\xe4\xbd\x8f\xe5\xaf\x86\xe7\xa0\x81" ) );
    statusTextLabel->setText( trUtf8( "\xe6\x9c\xaa\xe8\xbf\x9e\xe6\x8e\xa5" ) );
    advanceTabWidget->changeTab( outputTab, trUtf8( "\xe8\xbe\x93\xe5\x87\xba" ) );
    echo2TextLabel->setText( trUtf8( "\xe8\xae\xbe\x30\xe7\xa6\x81\xe7\x94\xa8" ) );
    echoTextLabel->setText( trUtf8( "\x45\x63\x68\x6f\xe5\x91\xa8\xe6\x9c\x9f\x28\x73\x29\x3a" ) );
    fakeAddressTextLabel->setText( trUtf8( "\xe4\xbc\xaa\x49\x50\x3a" ) );
    autoStartCheckBox->setText( trUtf8( "\xe5\xbc\x80\xe6\x9c\xba\xe8\x87\xaa\xe5\x8a\xa8\xe8\xbf\x90\xe8\xa1\x8c" ) );
    autoConnectCheckBox->setText( trUtf8( "\xe8\xbf\x90\xe8\xa1\x8c\xe8\x87\xaa\xe5\x8a\xa8\xe8\xbf\x9e\xe6\x8e\xa5" ) );
    autoMinimizeCheckBox->setText( trUtf8( "\xe8\xbf\x9e\xe6\x8e\xa5\xe6\x88\x90\xe5\x8a\x9f\xe8\x87\xaa\xe5\x8a\xa8\xe6\x9c\x80"
    "\xe5\xb0\x8f\xe5\x8c\x96" ) );
    intelligentCheckBox->setText( trUtf8( "\xe6\x99\xba\xe8\x83\xbd\xe9\x87\x8d\xe8\xbf\x9e" ) );
    fakeAddressCheckBox->setText( trUtf8( "\xe5\x90\xaf\xe7\x94\xa8\xe4\xbc\xaa\x49\x50" ) );
    authModeTextLabel->setText( trUtf8( "\xe8\xae\xa4\xe8\xaf\x81\xe6\xa8\xa1\xe5\xbc\x8f\x3a" ) );
    authModeComboBox->clear();
    authModeComboBox->insertItem( trUtf8( "\xe6\xa0\x87\xe5\x87\x86\xe6\xa8\xa1\xe5\xbc\x8f" ) );
    authModeComboBox->insertItem( trUtf8( "\xe5\xae\x9e\xe8\xbe\xbe\xe6\xa8\xa1\xe5\xbc\x8f" ) );
    advanceTabWidget->changeTab( optionTab, trUtf8( "\xe8\xbf\x90\xe8\xa1\x8c\xe9\x80\x89\xe9\xa1\xb9" ) );
    aboutButton->setText( trUtf8( "\xe5\x85\xb3\xe4\xba\x8e" ) );
    advanceTabWidget->changeTab( aboutTab, trUtf8( "\xe5\xb8\xae\xe5\x8a\xa9" ) );
}

void KStarBase::connectButton_clicked()
{
    qWarning( "KStarBase::connectButton_clicked(): Not implemented yet" );
}

void KStarBase::disconnectButton_clicked()
{
    qWarning( "KStarBase::disconnectButton_clicked(): Not implemented yet" );
}

void KStarBase::exitButton_clicked()
{
    qWarning( "KStarBase::exitButton_clicked(): Not implemented yet" );
}

void KStarBase::aboutButton_clicked()
{
    qWarning( "KStarBase::aboutButton_clicked(): Not implemented yet" );
}

void KStarBase::simpleCheckBox_stateChanged(int)
{
    qWarning( "KStarBase::simpleCheckBox_stateChanged(int): Not implemented yet" );
}

void KStarBase::fakeAddressCheckBox_stateChanged(int)
{
    qWarning( "KStarBase::fakeAddressCheckBox_stateChanged(int): Not implemented yet" );
}

void KStarBase::fakeAddressLineEdit_lostFocus()
{
    qWarning( "KStarBase::fakeAddressLineEdit_lostFocus(): Not implemented yet" );
}

void KStarBase::autoStartCheckBox_stateChanged(int)
{
    qWarning( "KStarBase::autoStartCheckBox_stateChanged(int): Not implemented yet" );
}

void KStarBase::autoConnectCheckBox_stateChanged(int)
{
    qWarning( "KStarBase::autoConnectCheckBox_stateChanged(int): Not implemented yet" );
}

void KStarBase::autoMinimizeCheckBox_stateChanged(int)
{
    qWarning( "KStarBase::autoMinimizeCheckBox_stateChanged(int): Not implemented yet" );
}

void KStarBase::intelligentCheckBox_stateChanged(int)
{
    qWarning( "KStarBase::intelligentCheckBox_stateChanged(int): Not implemented yet" );
}

void KStarBase::echoIntervalSpinBox_valueChanged(const QString&)
{
    qWarning( "KStarBase::echoIntervalSpinBox_valueChanged(const QString&): Not implemented yet" );
}

void KStarBase::rememberCheckBox_stateChanged(int)
{
    qWarning( "KStarBase::rememberCheckBox_stateChanged(int): Not implemented yet" );
}

void KStarBase::usernameLineEdit_textChanged(const QString&)
{
    qWarning( "KStarBase::usernameLineEdit_textChanged(const QString&): Not implemented yet" );
}

void KStarBase::passwordLineEdit_textChanged(const QString&)
{
    qWarning( "KStarBase::passwordLineEdit_textChanged(const QString&): Not implemented yet" );
}

void KStarBase::nicComboBox_textChanged(const QString&)
{
    qWarning( "KStarBase::nicComboBox_textChanged(const QString&): Not implemented yet" );
}

void KStarBase::authModeComboBox_textChanged(const QString&)
{
    qWarning( "KStarBase::authModeComboBox_textChanged(const QString&): Not implemented yet" );
}

void KStarBase::fakeAddressLineEdit_textChanged(const QString&)
{
    qWarning( "KStarBase::fakeAddressLineEdit_textChanged(const QString&): Not implemented yet" );
}

void KStarBase::nicComboBox_activated(int)
{
    qWarning( "KStarBase::nicComboBox_activated(int): Not implemented yet" );
}

void KStarBase::authModeComboBox_activated(int)
{
    qWarning( "KStarBase::authModeComboBox_activated(int): Not implemented yet" );
}

void KStarBase::advanceTabWidget_selected(const QString&)
{
    qWarning( "KStarBase::advanceTabWidget_selected(const QString&): Not implemented yet" );
}

