/****************************************************************************
** Meta object code from reading C++ file 'frequency.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../frequency/frequency.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'frequency.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SynFrequency_t {
    QByteArrayData data[9];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SynFrequency_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SynFrequency_t qt_meta_stringdata_SynFrequency = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SynFrequency"
QT_MOC_LITERAL(1, 13, 4), // "Read"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 9), // "Connected"
QT_MOC_LITERAL(4, 29, 12), // "Disconnected"
QT_MOC_LITERAL(5, 42, 7), // "ReadTcp"
QT_MOC_LITERAL(6, 50, 3), // "pos"
QT_MOC_LITERAL(7, 54, 5), // "Write"
QT_MOC_LITERAL(8, 60, 5) // "bytes"

    },
    "SynFrequency\0Read\0\0Connected\0Disconnected\0"
    "ReadTcp\0pos\0Write\0bytes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SynFrequency[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    1,   42,    2, 0x0a /* Public */,
       7,    1,   45,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::LongLong,    8,

       0        // eod
};

void SynFrequency::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SynFrequency *_t = static_cast<SynFrequency *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Read(); break;
        case 1: _t->Connected(); break;
        case 2: _t->Disconnected(); break;
        case 3: _t->ReadTcp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->Write((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject SynFrequency::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SynFrequency.data,
      qt_meta_data_SynFrequency,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SynFrequency::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SynFrequency::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SynFrequency.stringdata0))
        return static_cast<void*>(const_cast< SynFrequency*>(this));
    return QObject::qt_metacast(_clname);
}

int SynFrequency::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
