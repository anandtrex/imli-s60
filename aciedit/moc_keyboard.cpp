/****************************************************************************
** Meta object code from reading C++ file 'keyboard.h'
**
** Created: Wed Jan 6 18:10:47 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "keyboard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'keyboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VirtualKeyboard[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      26,   16,   16,   16, 0x0a,
      43,   16,   16,   16, 0x0a,
      60,   16,   16,   16, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_VirtualKeyboard[] = {
    "VirtualKeyboard\0\0Update()\0setVisible(bool)\0"
    "setShifted(bool)\0Clicked()\0"
};

const QMetaObject VirtualKeyboard::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_VirtualKeyboard,
      qt_meta_data_VirtualKeyboard, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VirtualKeyboard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VirtualKeyboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VirtualKeyboard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VirtualKeyboard))
        return static_cast<void*>(const_cast< VirtualKeyboard*>(this));
    return QFrame::qt_metacast(_clname);
}

int VirtualKeyboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Update(); break;
        case 1: setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: setShifted((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: Clicked(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
