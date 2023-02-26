
add_library(Qt5::VectorCanBusPlugin MODULE IMPORTED)


_populate_SerialBus_plugin_properties(VectorCanBusPlugin RELEASE "canbus/qtvectorcanbus.dll" TRUE)
_populate_SerialBus_plugin_properties(VectorCanBusPlugin DEBUG "canbus/qtvectorcanbusd.dll" TRUE)

list(APPEND Qt5SerialBus_PLUGINS Qt5::VectorCanBusPlugin)
set_property(TARGET Qt5::SerialBus APPEND PROPERTY QT_ALL_PLUGINS_canbus Qt5::VectorCanBusPlugin)
set_property(TARGET Qt5::VectorCanBusPlugin PROPERTY QT_PLUGIN_TYPE "canbus")
set_property(TARGET Qt5::VectorCanBusPlugin PROPERTY QT_PLUGIN_EXTENDS "")
set_property(TARGET Qt5::VectorCanBusPlugin PROPERTY QT_PLUGIN_CLASS_NAME "VectorCanBusPlugin")
