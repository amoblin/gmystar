/****************************************************************************
** KStar meta object code from reading C++ file 'kstar.h'
**
** Created: Sun Feb 8 14:47:56 2009
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "kstar.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *KStar::className() const
{
    return "KStar";
}

QMetaObject *KStar::metaObj = 0;
static QMetaObjectCleanUp cleanUp_KStar( "KStar", &KStar::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString KStar::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KStar", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString KStar::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KStar", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* KStar::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = KStarBase::staticMetaObject();
    static const QUMethod slot_0 = {"RunSA", 0, 0 };
    static const QUMethod slot_1 = {"RunEcho", 0, 0 };
    static const QUMethod slot_2 = {"ShowHelp", 0, 0 };
    static const QUMethod slot_3 = {"aboutButton_clicked", 0, 0 };
    static const QUMethod slot_4 = {"exitButton_clicked", 0, 0 };
    static const QUMethod slot_5 = {"disconnectButton_clicked", 0, 0 };
    static const QUMethod slot_6 = {"connectButton_clicked", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"simpleCheckBox_stateChanged", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"rememberCheckBox_stateChanged", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"echoIntervalSpinBox_valueChanged", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"intelligentCheckBox_stateChanged", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"autoMinimizeCheckBox_stateChanged", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"autoConnectCheckBox_stateChanged", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"autoStartCheckBox_stateChanged", 1, param_slot_13 };
    static const QUMethod slot_14 = {"fakeAddressLineEdit_lostFocus", 0, 0 };
    static const QUParameter param_slot_15[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"fakeAddressCheckBox_stateChanged", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"passwordLineEdit_textChanged", 1, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_17 = {"usernameLineEdit_textChanged", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"fakeAddressLineEdit_textChanged", 1, param_slot_18 };
    static const QUParameter param_slot_19[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_19 = {"authModeComboBox_activated", 1, param_slot_19 };
    static const QUParameter param_slot_20[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_20 = {"nicComboBox_activated", 1, param_slot_20 };
    static const QMetaData slot_tbl[] = {
	{ "RunSA()", &slot_0, QMetaData::Public },
	{ "RunEcho()", &slot_1, QMetaData::Public },
	{ "ShowHelp()", &slot_2, QMetaData::Public },
	{ "aboutButton_clicked()", &slot_3, QMetaData::Public },
	{ "exitButton_clicked()", &slot_4, QMetaData::Public },
	{ "disconnectButton_clicked()", &slot_5, QMetaData::Public },
	{ "connectButton_clicked()", &slot_6, QMetaData::Public },
	{ "simpleCheckBox_stateChanged(int)", &slot_7, QMetaData::Public },
	{ "rememberCheckBox_stateChanged(int)", &slot_8, QMetaData::Public },
	{ "echoIntervalSpinBox_valueChanged(const QString&)", &slot_9, QMetaData::Public },
	{ "intelligentCheckBox_stateChanged(int)", &slot_10, QMetaData::Public },
	{ "autoMinimizeCheckBox_stateChanged(int)", &slot_11, QMetaData::Public },
	{ "autoConnectCheckBox_stateChanged(int)", &slot_12, QMetaData::Public },
	{ "autoStartCheckBox_stateChanged(int)", &slot_13, QMetaData::Public },
	{ "fakeAddressLineEdit_lostFocus()", &slot_14, QMetaData::Public },
	{ "fakeAddressCheckBox_stateChanged(int)", &slot_15, QMetaData::Public },
	{ "passwordLineEdit_textChanged(const QString&)", &slot_16, QMetaData::Public },
	{ "usernameLineEdit_textChanged(const QString&)", &slot_17, QMetaData::Public },
	{ "fakeAddressLineEdit_textChanged(const QString&)", &slot_18, QMetaData::Public },
	{ "authModeComboBox_activated(int)", &slot_19, QMetaData::Public },
	{ "nicComboBox_activated(int)", &slot_20, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"KStar", parentObject,
	slot_tbl, 21,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_KStar.setMetaObject( metaObj );
    return metaObj;
}

void* KStar::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "KStar" ) )
	return this;
    return KStarBase::qt_cast( clname );
}

bool KStar::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: RunSA(); break;
    case 1: RunEcho(); break;
    case 2: ShowHelp(); break;
    case 3: aboutButton_clicked(); break;
    case 4: exitButton_clicked(); break;
    case 5: disconnectButton_clicked(); break;
    case 6: connectButton_clicked(); break;
    case 7: simpleCheckBox_stateChanged((int)static_QUType_int.get(_o+1)); break;
    case 8: rememberCheckBox_stateChanged((int)static_QUType_int.get(_o+1)); break;
    case 9: echoIntervalSpinBox_valueChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 10: intelligentCheckBox_stateChanged((int)static_QUType_int.get(_o+1)); break;
    case 11: autoMinimizeCheckBox_stateChanged((int)static_QUType_int.get(_o+1)); break;
    case 12: autoConnectCheckBox_stateChanged((int)static_QUType_int.get(_o+1)); break;
    case 13: autoStartCheckBox_stateChanged((int)static_QUType_int.get(_o+1)); break;
    case 14: fakeAddressLineEdit_lostFocus(); break;
    case 15: fakeAddressCheckBox_stateChanged((int)static_QUType_int.get(_o+1)); break;
    case 16: passwordLineEdit_textChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 17: usernameLineEdit_textChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 18: fakeAddressLineEdit_textChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 19: authModeComboBox_activated((int)static_QUType_int.get(_o+1)); break;
    case 20: nicComboBox_activated((int)static_QUType_int.get(_o+1)); break;
    default:
	return KStarBase::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool KStar::qt_emit( int _id, QUObject* _o )
{
    return KStarBase::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool KStar::qt_property( int id, int f, QVariant* v)
{
    return KStarBase::qt_property( id, f, v);
}

bool KStar::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
