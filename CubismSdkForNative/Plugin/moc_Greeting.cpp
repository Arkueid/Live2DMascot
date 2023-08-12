/****************************************************************************
** Meta object code from reading C++ file 'Greeting.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "Greeting.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Greeting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Greeting_t {
    QByteArrayData data[1];
    char stringdata0[9];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Greeting_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Greeting_t qt_meta_stringdata_Greeting = {
    {
QT_MOC_LITERAL(0, 0, 8) // "Greeting"

    },
    "Greeting"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Greeting[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void Greeting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Greeting::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Greeting.data,
    qt_meta_data_Greeting,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Greeting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Greeting::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Greeting.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IPlugin"))
        return static_cast< IPlugin*>(this);
    if (!strcmp(_clname, "com.arkueid.live2dmascot.plugin"))
        return static_cast< IPlugin*>(this);
    return QObject::qt_metacast(_clname);
}

int Greeting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}

QT_PLUGIN_METADATA_SECTION
static constexpr unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    0xbf, 
    // "IID"
    0x02,  0x78,  0x1f,  'c',  'o',  'm',  '.',  'a', 
    'r',  'k',  'u',  'e',  'i',  'd',  '.',  'l', 
    'i',  'v',  'e',  '2',  'd',  'm',  'a',  's', 
    'c',  'o',  't',  '.',  'p',  'l',  'u',  'g', 
    'i',  'n', 
    // "className"
    0x03,  0x68,  'G',  'r',  'e',  'e',  't',  'i', 
    'n',  'g', 
    // "MetaData"
    0x04,  0xa5,  0x66,  'a',  'u',  't',  'h',  'o', 
    'r',  0x67,  'A',  'r',  'k',  'u',  'e',  'i', 
    'd',  0x64,  'd',  'e',  's',  'c',  0x78,  0x60, 
    uchar('\xe7'), uchar('\xa8'), uchar('\x8b'), uchar('\xe5'), uchar('\xba'), uchar('\x8f'), uchar('\xe5'), uchar('\x90'),
    uchar('\xaf'), uchar('\xe5'), uchar('\x8a'), uchar('\xa8'), uchar('\xe6'), uchar('\x97'), uchar('\xb6'), uchar('\xe6'),
    uchar('\xa0'), uchar('\xb9'), uchar('\xe6'), uchar('\x8d'), uchar('\xae'), uchar('\xe6'), uchar('\x97'), uchar('\xb6'),
    uchar('\xe9'), uchar('\x97'), uchar('\xb4'), uchar('\xe8'), uchar('\xaf'), uchar('\xb4'), uchar('\xe5'), uchar('\x87'),
    uchar('\xba'), uchar('\xe7'), uchar('\x9b'), uchar('\xb8'), uchar('\xe5'), uchar('\xba'), uchar('\x94'), uchar('\xe7'),
    uchar('\x9a'), uchar('\x84'), uchar('\xe9'), uchar('\x97'), uchar('\xae'), uchar('\xe5'), uchar('\x80'), uchar('\x99'),
    uchar('\xe8'), uchar('\xaf'), uchar('\xad'), uchar('\xef'), uchar('\xbc'), uchar('\x8c'), uchar('\xe6'), uchar('\xa0'),
    uchar('\xb9'), uchar('\xe6'), uchar('\x8d'), uchar('\xae'), uchar('\xe8'), uchar('\xbf'), uchar('\x90'), uchar('\xe8'),
    uchar('\xa1'), uchar('\x8c'), uchar('\xe6'), uchar('\x97'), uchar('\xb6'), uchar('\xe9'), uchar('\x95'), uchar('\xbf'),
    uchar('\xe7'), uchar('\xbb'), uchar('\x99'), uchar('\xe5'), uchar('\x87'), uchar('\xba'), uchar('\xe7'), uchar('\x9b'),
    uchar('\xb8'), uchar('\xe5'), uchar('\xba'), uchar('\x94'), uchar('\xe9'), uchar('\x97'), uchar('\xae'), uchar('\xe5'),
    uchar('\x80'), uchar('\x99'), uchar('\xe8'), uchar('\xaf'), uchar('\xad'), uchar('\xe3'), uchar('\x80'), uchar('\x82'),
    0x62,  'i',  'd',  0x78,  0x1a,  'c',  'o',  'm', 
    '.',  'a',  'r',  'k',  'u',  'e',  'i',  'd', 
    '.',  'g',  'r',  'e',  'e',  't',  'i',  'n', 
    'g',  '/',  '1',  '.',  '0',  '.',  '0',  0x64, 
    'n',  'a',  'm',  'e',  0x68,  'G',  'r',  'e', 
    'e',  't',  'i',  'n',  'g',  0x67,  'v',  'e', 
    'r',  's',  'i',  'o',  'n',  0x65,  '1',  '.', 
    '0',  '.',  '0', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN(Greeting, Greeting)

QT_WARNING_POP
QT_END_MOC_NAMESPACE
