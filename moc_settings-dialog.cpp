/****************************************************************************
** Meta object code from reading C++ file 'settings-dialog.h'
**
** Created: Tue May 3 13:17:46 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "settings-dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settings-dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_shruthiEditorSettings[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      27,   22,   23,   22, 0x0a,
      45,   22,   23,   22, 0x0a,
      66,   64,   22,   22, 0x0a,
      87,   22,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_shruthiEditorSettings[] = {
    "shruthiEditorSettings\0\0int\0getInputChannel()\0"
    "getOutputChannel()\0,\0setChannels(int,int)\0"
    "getDeviceInfo()\0"
};

const QMetaObject shruthiEditorSettings::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_shruthiEditorSettings,
      qt_meta_data_shruthiEditorSettings, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &shruthiEditorSettings::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *shruthiEditorSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *shruthiEditorSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_shruthiEditorSettings))
        return static_cast<void*>(const_cast< shruthiEditorSettings*>(this));
    return QDialog::qt_metacast(_clname);
}

int shruthiEditorSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = getInputChannel();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: { int _r = getOutputChannel();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: setChannels((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: getDeviceInfo(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
