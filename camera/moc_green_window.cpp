/****************************************************************************
** Meta object code from reading C++ file 'green_window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "green_window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'green_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GreenWindow_t {
    QByteArrayData data[15];
    char stringdata0[176];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GreenWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GreenWindow_t qt_meta_stringdata_GreenWindow = {
    {
QT_MOC_LITERAL(0, 0, 11), // "GreenWindow"
QT_MOC_LITERAL(1, 12, 9), // "grabFrame"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 21), // "onExposureSliderMoved"
QT_MOC_LITERAL(4, 45, 3), // "pos"
QT_MOC_LITERAL(5, 49, 21), // "onExposureSpinChanged"
QT_MOC_LITERAL(6, 71, 2), // "us"
QT_MOC_LITERAL(7, 74, 19), // "onAnalogGainChanged"
QT_MOC_LITERAL(8, 94, 3), // "val"
QT_MOC_LITERAL(9, 98, 20), // "onDigitalGainChanged"
QT_MOC_LITERAL(10, 119, 19), // "onResolutionChanged"
QT_MOC_LITERAL(11, 139, 5), // "index"
QT_MOC_LITERAL(12, 145, 15), // "onConnectCamera"
QT_MOC_LITERAL(13, 161, 7), // "onStart"
QT_MOC_LITERAL(14, 169, 6) // "onStop"

    },
    "GreenWindow\0grabFrame\0\0onExposureSliderMoved\0"
    "pos\0onExposureSpinChanged\0us\0"
    "onAnalogGainChanged\0val\0onDigitalGainChanged\0"
    "onResolutionChanged\0index\0onConnectCamera\0"
    "onStart\0onStop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GreenWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    1,   60,    2, 0x08 /* Private */,
       5,    1,   63,    2, 0x08 /* Private */,
       7,    1,   66,    2, 0x08 /* Private */,
       9,    1,   69,    2, 0x08 /* Private */,
      10,    1,   72,    2, 0x08 /* Private */,
      12,    0,   75,    2, 0x08 /* Private */,
      13,    0,   76,    2, 0x08 /* Private */,
      14,    0,   77,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GreenWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GreenWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->grabFrame(); break;
        case 1: _t->onExposureSliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onExposureSpinChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onAnalogGainChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onDigitalGainChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onResolutionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onConnectCamera(); break;
        case 7: _t->onStart(); break;
        case 8: _t->onStop(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GreenWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_GreenWindow.data,
    qt_meta_data_GreenWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GreenWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GreenWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GreenWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int GreenWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
