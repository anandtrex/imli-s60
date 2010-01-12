/****************************************************************************
** Meta object code from reading C++ file 'aciedit.h'
**
** Created: Wed Jan 6 16:42:35 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "aciedit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'aciedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ACIEdit[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      36,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      28,    8,   18,    8, 0x0a,
      37,    8,    8,    8, 0x0a,
      47,    8,    8,    8, 0x0a,
      69,    8,   64,    8, 0x0a,
      79,    8,   64,    8, 0x0a,
      91,    8,    8,    8, 0x0a,
     104,    8,    8,    8, 0x0a,
     117,    8,    8,    8, 0x0a,
     129,    8,   64,    8, 0x0a,
     140,    8,    8,    8, 0x0a,
     148,    8,    8,    8, 0x0a,
     156,    8,    8,    8, 0x0a,
     173,    8,    8,    8, 0x0a,
     192,    8,    8,    8, 0x0a,
     216,    8,    8,    8, 0x0a,
     232,    8,    8,    8, 0x0a,
     239,    8,    8,    8, 0x0a,
     245,    8,    8,    8, 0x0a,
     252,    8,    8,    8, 0x0a,
     260,    8,    8,    8, 0x0a,
     273,    8,    8,    8, 0x0a,
     281,    8,    8,    8, 0x0a,
     293,    8,    8,    8, 0x0a,
     315,    8,    8,    8, 0x0a,
     324,    8,    8,    8, 0x0a,
     334,    8,    8,    8, 0x0a,
     348,    8,    8,    8, 0x0a,
     371,    8,    8,    8, 0x0a,
     393,    8,    8,    8, 0x0a,
     412,    8,    8,    8, 0x0a,
     423,    8,    8,    8, 0x0a,
     434,    8,    8,    8, 0x0a,
     457,    8,    8,    8, 0x0a,
     475,    8,    8,    8, 0x0a,
     492,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ACIEdit[] = {
    "ACIEdit\0\0NewDoc()\0TextEdit*\0NewTab()\0"
    "OpenDoc()\0OpenDoc(QString)\0bool\0"
    "SaveDoc()\0SaveDocAs()\0SaveAsUtf8()\0"
    "SaveAsHTML()\0SaveAsRTF()\0CloseDoc()\0"
    "About()\0Usage()\0UpdateTitleBar()\0"
    "ScriptChanged(int)\0InputMethodChanged(int)\0"
    "ToggleEnglish()\0Undo()\0Cut()\0Copy()\0"
    "Paste()\0PasteASCII()\0Clear()\0SelectAll()\0"
    "DocumentWasModified()\0ZoomIn()\0ZoomOut()\0"
    "SelectStyle()\0ReadPendingDatagrams()\0"
    "FlagBadState(QString)\0ClearFileHistory()\0"
    "FindNext()\0FindPrev()\0IndexCurrentDocument()\0"
    "ShowCompletions()\0LoadDictionary()\0"
    "ClearDictionary()\0"
};

const QMetaObject ACIEdit::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ACIEdit,
      qt_meta_data_ACIEdit, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ACIEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ACIEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ACIEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ACIEdit))
        return static_cast<void*>(const_cast< ACIEdit*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ACIEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: NewDoc(); break;
        case 1: { TextEdit* _r = NewTab();
            if (_a[0]) *reinterpret_cast< TextEdit**>(_a[0]) = _r; }  break;
        case 2: OpenDoc(); break;
        case 3: OpenDoc((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: { bool _r = SaveDoc();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = SaveDocAs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: SaveAsUtf8(); break;
        case 7: SaveAsHTML(); break;
        case 8: SaveAsRTF(); break;
        case 9: { bool _r = CloseDoc();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: About(); break;
        case 11: Usage(); break;
        case 12: UpdateTitleBar(); break;
        case 13: ScriptChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: InputMethodChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: ToggleEnglish(); break;
        case 16: Undo(); break;
        case 17: Cut(); break;
        case 18: Copy(); break;
        case 19: Paste(); break;
        case 20: PasteASCII(); break;
        case 21: Clear(); break;
        case 22: SelectAll(); break;
        case 23: DocumentWasModified(); break;
        case 24: ZoomIn(); break;
        case 25: ZoomOut(); break;
        case 26: SelectStyle(); break;
        case 27: ReadPendingDatagrams(); break;
        case 28: FlagBadState((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 29: ClearFileHistory(); break;
        case 30: FindNext(); break;
        case 31: FindPrev(); break;
        case 32: IndexCurrentDocument(); break;
        case 33: ShowCompletions(); break;
        case 34: LoadDictionary(); break;
        case 35: ClearDictionary(); break;
        default: ;
        }
        _id -= 36;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
