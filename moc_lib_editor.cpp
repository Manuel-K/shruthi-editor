/****************************************************************************
** Meta object code from reading C++ file 'lib_editor.h'
**
** Created: Tue May 3 13:17:48 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "lib_editor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lib_editor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Editor[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      24,    7,    7,    7, 0x05,
      36,    7,    7,    7, 0x05,
      47,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      77,    7,    7,    7, 0x0a,
     105,  103,   98,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Editor[] = {
    "Editor\0\0redrawNRPN(int)\0redrawAll()\0"
    "finished()\0midiOutputStatusChanged(bool)\0"
    "process(queueitem_t)\0bool\0,\0"
    "setMidiDevices(int,int)\0"
};

const QMetaObject Editor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Editor,
      qt_meta_data_Editor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Editor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Editor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Editor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Editor))
        return static_cast<void*>(const_cast< Editor*>(this));
    return QObject::qt_metacast(_clname);
}

int Editor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: redrawNRPN((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: redrawAll(); break;
        case 2: finished(); break;
        case 3: midiOutputStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: process((*reinterpret_cast< queueitem_t(*)>(_a[1]))); break;
        case 5: { bool _r = setMidiDevices((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Editor::redrawNRPN(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Editor::redrawAll()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Editor::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Editor::midiOutputStatusChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
