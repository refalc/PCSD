/****************************************************************************
** Meta object code from reading C++ file 'cubes.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../untitled/cubes.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cubes.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cube_t {
    QByteArrayData data[8];
    char stringdata0[54];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cube_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cube_t qt_meta_stringdata_cube = {
    {
QT_MOC_LITERAL(0, 0, 4), // "cube"
QT_MOC_LITERAL(1, 5, 4), // "Read"
QT_MOC_LITERAL(2, 10, 0), // ""
QT_MOC_LITERAL(3, 11, 9), // "Connected"
QT_MOC_LITERAL(4, 21, 12), // "Disconnected"
QT_MOC_LITERAL(5, 34, 7), // "ReadTcp"
QT_MOC_LITERAL(6, 42, 5), // "Write"
QT_MOC_LITERAL(7, 48, 5) // "bytes"

    },
    "cube\0Read\0\0Connected\0Disconnected\0"
    "ReadTcp\0Write\0bytes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cube[] = {

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
       5,    0,   42,    2, 0x0a /* Public */,
       6,    1,   43,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,    7,

       0        // eod
};

void cube::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        cube *_t = static_cast<cube *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Read(); break;
        case 1: _t->Connected(); break;
        case 2: _t->Disconnected(); break;
        case 3: _t->ReadTcp(); break;
        case 4: _t->Write((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject cube::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_cube.data,
      qt_meta_data_cube,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *cube::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cube::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_cube.stringdata0))
        return static_cast<void*>(const_cast< cube*>(this));
    return QObject::qt_metacast(_clname);
}

int cube::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
