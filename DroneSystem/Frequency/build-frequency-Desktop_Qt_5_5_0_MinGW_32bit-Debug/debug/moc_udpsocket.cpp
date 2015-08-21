/****************************************************************************
** Meta object code from reading C++ file 'udpsocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../UDPSocket/udpsocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'udpsocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SysFUDPSocket_t {
    QByteArrayData data[9];
    char stringdata0[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SysFUDPSocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SysFUDPSocket_t qt_meta_stringdata_SysFUDPSocket = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SysFUDPSocket"
QT_MOC_LITERAL(1, 14, 7), // "NewData"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 7), // "Address"
QT_MOC_LITERAL(4, 31, 4), // "data"
QT_MOC_LITERAL(5, 36, 9), // "ReadyRead"
QT_MOC_LITERAL(6, 46, 4), // "Send"
QT_MOC_LITERAL(7, 51, 7), // "address"
QT_MOC_LITERAL(8, 59, 11) // "std::string"

    },
    "SysFUDPSocket\0NewData\0\0Address\0data\0"
    "ReadyRead\0Send\0address\0std::string"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SysFUDPSocket[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   32,    2, 0x0a /* Public */,
       6,    2,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 8,    7,    4,

       0        // eod
};

void SysFUDPSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SysFUDPSocket *_t = static_cast<SysFUDPSocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->NewData((*reinterpret_cast< Address(*)>(_a[1]))); break;
        case 1: _t->ReadyRead(); break;
        case 2: _t->Send((*reinterpret_cast< Address(*)>(_a[1])),(*reinterpret_cast< std::string(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SysFUDPSocket::*_t)(Address );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SysFUDPSocket::NewData)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject SysFUDPSocket::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SysFUDPSocket.data,
      qt_meta_data_SysFUDPSocket,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SysFUDPSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SysFUDPSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SysFUDPSocket.stringdata0))
        return static_cast<void*>(const_cast< SysFUDPSocket*>(this));
    return QObject::qt_metacast(_clname);
}

int SysFUDPSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void SysFUDPSocket::NewData(Address _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
