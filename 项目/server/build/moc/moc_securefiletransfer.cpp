/****************************************************************************
** Meta object code from reading C++ file 'securefiletransfer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/securefiletransfer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'securefiletransfer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SecureFileTransfer_t {
    QByteArrayData data[8];
    char stringdata0[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SecureFileTransfer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SecureFileTransfer_t qt_meta_stringdata_SecureFileTransfer = {
    {
QT_MOC_LITERAL(0, 0, 18), // "SecureFileTransfer"
QT_MOC_LITERAL(1, 19, 16), // "transferProgress"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 9), // "sessionId"
QT_MOC_LITERAL(4, 47, 8), // "progress"
QT_MOC_LITERAL(5, 56, 17), // "transferCompleted"
QT_MOC_LITERAL(6, 74, 13), // "transferError"
QT_MOC_LITERAL(7, 88, 5) // "error"

    },
    "SecureFileTransfer\0transferProgress\0"
    "\0sessionId\0progress\0transferCompleted\0"
    "transferError\0error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SecureFileTransfer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,
       5,    1,   34,    2, 0x06 /* Public */,
       6,    2,   37,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Double,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    7,

       0        // eod
};

void SecureFileTransfer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SecureFileTransfer *_t = static_cast<SecureFileTransfer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->transferProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->transferCompleted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->transferError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SecureFileTransfer::*_t)(const QString & , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SecureFileTransfer::transferProgress)) {
                *result = 0;
            }
        }
        {
            typedef void (SecureFileTransfer::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SecureFileTransfer::transferCompleted)) {
                *result = 1;
            }
        }
        {
            typedef void (SecureFileTransfer::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SecureFileTransfer::transferError)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject SecureFileTransfer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SecureFileTransfer.data,
      qt_meta_data_SecureFileTransfer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SecureFileTransfer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SecureFileTransfer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SecureFileTransfer.stringdata0))
        return static_cast<void*>(const_cast< SecureFileTransfer*>(this));
    return QObject::qt_metacast(_clname);
}

int SecureFileTransfer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void SecureFileTransfer::transferProgress(const QString & _t1, double _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SecureFileTransfer::transferCompleted(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SecureFileTransfer::transferError(const QString & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
