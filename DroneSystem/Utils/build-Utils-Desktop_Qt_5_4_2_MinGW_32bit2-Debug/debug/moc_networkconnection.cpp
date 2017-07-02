/****************************************************************************
** Meta object code from reading C++ file 'networkconnection.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Utils/networkconnection.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkconnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CNetworkConnection_t {
    QByteArrayData data[1];
    char stringdata[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CNetworkConnection_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CNetworkConnection_t qt_meta_stringdata_CNetworkConnection = {
    {
QT_MOC_LITERAL(0, 0, 18) // "CNetworkConnection"

    },
    "CNetworkConnection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CNetworkConnection[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void CNetworkConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CNetworkConnection::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CNetworkConnection.data,
      qt_meta_data_CNetworkConnection,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CNetworkConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CNetworkConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CNetworkConnection.stringdata))
        return static_cast<void*>(const_cast< CNetworkConnection*>(this));
    if (!strcmp(_clname, "INetworkConnection"))
        return static_cast< INetworkConnection*>(const_cast< CNetworkConnection*>(this));
    if (!strcmp(_clname, "INetworkConnection/1.0"))
        return static_cast< INetworkConnection*>(const_cast< CNetworkConnection*>(this));
    return QObject::qt_metacast(_clname);
}

int CNetworkConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_CNetworkConnectionUDP_t {
    QByteArrayData data[3];
    char stringdata[31];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CNetworkConnectionUDP_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CNetworkConnectionUDP_t qt_meta_stringdata_CNetworkConnectionUDP = {
    {
QT_MOC_LITERAL(0, 0, 21), // "CNetworkConnectionUDP"
QT_MOC_LITERAL(1, 22, 7), // "ReadUDP"
QT_MOC_LITERAL(2, 30, 0) // ""

    },
    "CNetworkConnectionUDP\0ReadUDP\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CNetworkConnectionUDP[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CNetworkConnectionUDP::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CNetworkConnectionUDP *_t = static_cast<CNetworkConnectionUDP *>(_o);
        switch (_id) {
        case 0: _t->ReadUDP(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CNetworkConnectionUDP::staticMetaObject = {
    { &CNetworkConnection::staticMetaObject, qt_meta_stringdata_CNetworkConnectionUDP.data,
      qt_meta_data_CNetworkConnectionUDP,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CNetworkConnectionUDP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CNetworkConnectionUDP::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CNetworkConnectionUDP.stringdata))
        return static_cast<void*>(const_cast< CNetworkConnectionUDP*>(this));
    return CNetworkConnection::qt_metacast(_clname);
}

int CNetworkConnectionUDP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CNetworkConnection::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_CNetworkConnectionTCP_t {
    QByteArrayData data[5];
    char stringdata[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CNetworkConnectionTCP_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CNetworkConnectionTCP_t qt_meta_stringdata_CNetworkConnectionTCP = {
    {
QT_MOC_LITERAL(0, 0, 21), // "CNetworkConnectionTCP"
QT_MOC_LITERAL(1, 22, 12), // "ConnectedTCP"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 15), // "DisconnectedTCP"
QT_MOC_LITERAL(4, 52, 7) // "ReadTCP"

    },
    "CNetworkConnectionTCP\0ConnectedTCP\0\0"
    "DisconnectedTCP\0ReadTCP"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CNetworkConnectionTCP[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    0,   30,    2, 0x0a /* Public */,
       4,    0,   31,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CNetworkConnectionTCP::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CNetworkConnectionTCP *_t = static_cast<CNetworkConnectionTCP *>(_o);
        switch (_id) {
        case 0: _t->ConnectedTCP(); break;
        case 1: _t->DisconnectedTCP(); break;
        case 2: _t->ReadTCP(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CNetworkConnectionTCP::staticMetaObject = {
    { &CNetworkConnection::staticMetaObject, qt_meta_stringdata_CNetworkConnectionTCP.data,
      qt_meta_data_CNetworkConnectionTCP,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CNetworkConnectionTCP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CNetworkConnectionTCP::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CNetworkConnectionTCP.stringdata))
        return static_cast<void*>(const_cast< CNetworkConnectionTCP*>(this));
    return CNetworkConnection::qt_metacast(_clname);
}

int CNetworkConnectionTCP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CNetworkConnection::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
