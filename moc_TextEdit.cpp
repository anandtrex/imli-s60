/****************************************************************************
** Meta object code from reading C++ file 'TextEdit.h'
**
** Created: Mon Jan 4 20:23:25 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TextEdit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TextEdit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TextEdit[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      19,    9,    9,    9, 0x0a,
      26,    9,    9,    9, 0x0a,
      32,    9,    9,    9, 0x0a,
      39,    9,    9,    9, 0x0a,
      47,    9,    9,    9, 0x0a,
      62,    9,    9,    9, 0x0a,
      70,    9,    9,    9, 0x0a,
      82,    9,    9,    9, 0x0a,
     117,  111,    9,    9, 0x0a,
     129,    9,    9,    9, 0x2a,
     138,  111,    9,    9, 0x0a,
     151,    9,    9,    9, 0x2a,
     161,    9,    9,    9, 0x0a,
     175,    9,    9,    9, 0x0a,
     210,  204,    9,    9, 0x09,
     247,    9,    9,    9, 0x0a,
     265,    9,    9,    9, 0x0a,
     277,    9,    9,    9, 0x0a,
     290,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TextEdit[] = {
    "TextEdit\0\0Update()\0Undo()\0Cut()\0Copy()\0"
    "Paste()\0PasteAsASCII()\0Clear()\0"
    "SelectAll()\0Transliterate(imli_script_t)\0"
    "range\0ZoomIn(int)\0ZoomIn()\0ZoomOut(int)\0"
    "ZoomOut()\0SyncCursors()\0"
    "InsertItem(QListWidgetItem*)\0event\0"
    "slotTextModified(TextStorage::Event)\0"
    "EnableStats(bool)\0ShowStats()\0"
    "RecordUndo()\0ClearUndo()\0"
};

const QMetaObject TextEdit::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_TextEdit,
      qt_meta_data_TextEdit, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TextEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TextEdit))
        return static_cast<void*>(const_cast< TextEdit*>(this));
    return QTextEdit::qt_metacast(_clname);
}

int TextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Update(); break;
        case 1: Undo(); break;
        case 2: Cut(); break;
        case 3: Copy(); break;
        case 4: Paste(); break;
        case 5: PasteAsASCII(); break;
        case 6: Clear(); break;
        case 7: SelectAll(); break;
        case 8: Transliterate((*reinterpret_cast< imli_script_t(*)>(_a[1]))); break;
        case 9: ZoomIn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: ZoomIn(); break;
        case 11: ZoomOut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: ZoomOut(); break;
        case 13: SyncCursors(); break;
        case 14: InsertItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 15: slotTextModified((*reinterpret_cast< const TextStorage::Event(*)>(_a[1]))); break;
        case 16: EnableStats((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: ShowStats(); break;
        case 18: RecordUndo(); break;
        case 19: ClearUndo(); break;
        default: ;
        }
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
