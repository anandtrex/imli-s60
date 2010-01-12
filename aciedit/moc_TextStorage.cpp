/****************************************************************************
** Meta object code from reading C++ file 'TextStorage.h'
**
** Created: Wed Dec 23 00:53:31 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TextStorage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TextStorage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TextStorage[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      46,   12,   12,   12, 0x05,
      61,   12,   12,   12, 0x05,
      75,   12,   12,   12, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_TextStorage[] = {
    "TextStorage\0\0TextModified(TextStorage::Event)\0"
    "UndoRecorded()\0UndoCleared()\0"
    "InvalidState(QString)\0"
};

const QMetaObject TextStorage::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TextStorage,
      qt_meta_data_TextStorage, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TextStorage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TextStorage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TextStorage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TextStorage))
        return static_cast<void*>(const_cast< TextStorage*>(this));
    return QObject::qt_metacast(_clname);
}

int TextStorage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: TextModified((*reinterpret_cast< const TextStorage::Event(*)>(_a[1]))); break;
        case 1: UndoRecorded(); break;
        case 2: UndoCleared(); break;
        case 3: InvalidState((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void TextStorage::TextModified(const TextStorage::Event & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TextStorage::UndoRecorded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void TextStorage::UndoCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void TextStorage::InvalidState(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
