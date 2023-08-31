/****************************************************************************
** Meta object code from reading C++ file 'GLWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../win/GLWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GLWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GLWidget_t {
    QByteArrayData data[21];
    char stringdata0[394];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GLWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GLWidget_t qt_meta_stringdata_GLWidget = {
    {
QT_MOC_LITERAL(0, 0, 8), // "GLWidget"
QT_MOC_LITERAL(1, 9, 10), // "shouldQuit"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 19), // "trayIconOnActivated"
QT_MOC_LITERAL(4, 41, 33), // "QSystemTrayIcon::ActivationRe..."
QT_MOC_LITERAL(5, 75, 6), // "reason"
QT_MOC_LITERAL(6, 82, 15), // "quitOnTriggered"
QT_MOC_LITERAL(7, 98, 20), // "keepQuietOnTriggered"
QT_MOC_LITERAL(8, 119, 25), // "keepMouseTrackOnTriggered"
QT_MOC_LITERAL(9, 145, 15), // "hideOnTriggered"
QT_MOC_LITERAL(10, 161, 20), // "stayOnTopOnTriggered"
QT_MOC_LITERAL(11, 182, 21), // "setNoSoundOnTriggered"
QT_MOC_LITERAL(12, 204, 22), // "setShowTextOnTriggered"
QT_MOC_LITERAL(13, 227, 23), // "showSettingsOnTriggered"
QT_MOC_LITERAL(14, 251, 28), // "setShowBackgroundOnTriggered"
QT_MOC_LITERAL(15, 280, 35), // "setTransparentBackgroundOnTri..."
QT_MOC_LITERAL(16, 316, 34), // "setTransparentCharacterOnTrig..."
QT_MOC_LITERAL(17, 351, 16), // "pieMenuOnClicked"
QT_MOC_LITERAL(18, 368, 7), // "uint8_t"
QT_MOC_LITERAL(19, 376, 4), // "btni"
QT_MOC_LITERAL(20, 381, 12) // "completeQuit"

    },
    "GLWidget\0shouldQuit\0\0trayIconOnActivated\0"
    "QSystemTrayIcon::ActivationReason\0"
    "reason\0quitOnTriggered\0keepQuietOnTriggered\0"
    "keepMouseTrackOnTriggered\0hideOnTriggered\0"
    "stayOnTopOnTriggered\0setNoSoundOnTriggered\0"
    "setShowTextOnTriggered\0showSettingsOnTriggered\0"
    "setShowBackgroundOnTriggered\0"
    "setTransparentBackgroundOnTriggered\0"
    "setTransparentCharacterOnTriggered\0"
    "pieMenuOnClicked\0uint8_t\0btni\0"
    "completeQuit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GLWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   90,    2, 0x08 /* Private */,
       6,    0,   93,    2, 0x08 /* Private */,
       7,    0,   94,    2, 0x08 /* Private */,
       8,    0,   95,    2, 0x08 /* Private */,
       9,    0,   96,    2, 0x08 /* Private */,
      10,    0,   97,    2, 0x08 /* Private */,
      11,    0,   98,    2, 0x08 /* Private */,
      12,    0,   99,    2, 0x08 /* Private */,
      13,    0,  100,    2, 0x08 /* Private */,
      14,    0,  101,    2, 0x08 /* Private */,
      15,    0,  102,    2, 0x08 /* Private */,
      16,    0,  103,    2, 0x08 /* Private */,
      17,    1,  104,    2, 0x08 /* Private */,
      20,    0,  107,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void,

       0        // eod
};

void GLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GLWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->shouldQuit(); break;
        case 1: _t->trayIconOnActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 2: _t->quitOnTriggered(); break;
        case 3: _t->keepQuietOnTriggered(); break;
        case 4: _t->keepMouseTrackOnTriggered(); break;
        case 5: _t->hideOnTriggered(); break;
        case 6: _t->stayOnTopOnTriggered(); break;
        case 7: _t->setNoSoundOnTriggered(); break;
        case 8: _t->setShowTextOnTriggered(); break;
        case 9: _t->showSettingsOnTriggered(); break;
        case 10: _t->setShowBackgroundOnTriggered(); break;
        case 11: _t->setTransparentBackgroundOnTriggered(); break;
        case 12: _t->setTransparentCharacterOnTriggered(); break;
        case 13: _t->pieMenuOnClicked((*reinterpret_cast< uint8_t(*)>(_a[1]))); break;
        case 14: _t->completeQuit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GLWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GLWidget::shouldQuit)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GLWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QOpenGLWidget::staticMetaObject>(),
    qt_meta_stringdata_GLWidget.data,
    qt_meta_data_GLWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GLWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IGLWidget"))
        return static_cast< IGLWidget*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int GLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void GLWidget::shouldQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
