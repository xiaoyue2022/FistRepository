/****************************************************************************
** Meta object code from reading C++ file 'accesscontrol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/accesscontrol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'accesscontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AccessControl_t {
    QByteArrayData data[7];
    char stringdata0[79];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AccessControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AccessControl_t qt_meta_stringdata_AccessControl = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AccessControl"
QT_MOC_LITERAL(1, 14, 17), // "permissionChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 6), // "userId"
QT_MOC_LITERAL(4, 40, 8), // "resource"
QT_MOC_LITERAL(5, 49, 21), // "twoFactorAuthRequired"
QT_MOC_LITERAL(6, 71, 7) // "adminId"

    },
    "AccessControl\0permissionChanged\0\0"
    "userId\0resource\0twoFactorAuthRequired\0"
    "adminId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AccessControl[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,
       5,    1,   29,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString,    6,

       0        // eod
};

void AccessControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AccessControl *_t = static_cast<AccessControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->permissionChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->twoFactorAuthRequired((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AccessControl::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AccessControl::permissionChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (AccessControl::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AccessControl::twoFactorAuthRequired)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject AccessControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AccessControl.data,
      qt_meta_data_AccessControl,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AccessControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AccessControl::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AccessControl.stringdata0))
        return static_cast<void*>(const_cast< AccessControl*>(this));
    return QObject::qt_metacast(_clname);
}

int AccessControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void AccessControl::permissionChanged(const QString & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AccessControl::twoFactorAuthRequired(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
