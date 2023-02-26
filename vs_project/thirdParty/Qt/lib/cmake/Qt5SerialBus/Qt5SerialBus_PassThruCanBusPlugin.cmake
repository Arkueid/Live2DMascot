
add_library(Qt5::PassThruCanBusPlugin MODULE IMPORTED)


_populate_SerialBus_plugin_properties(PassThruCanBusPlugin RELEASE "canbus/qtpassthrucanbus.dll" TRUE)
_populate_SerialBus_plugin_properties(PassThruCanBusPlugin DEBUG "canbus/qtpassthrucanbusd.dll" TRUE)

list(APPEND Qt5SerialBus_PLUGINS Qt5::PassThruCanBusPlugin)
set_property(TARGET Qt5::SerialBus APPEND PROPERTY QT_ALL_PLUGINS_canbus Qt5::PassThruCanBusPlugin)
set_property(TARGET Qt5::PassThruCanBusPlugin PROPERTY QT_PLUGIN_TYPE "canbus")
set_property(TARGET Qt5::PassThruCanBusPlugin PROPERTY QT_PLUGIN_EXTENDS "")
set_property(TARGET Qt5::PassThruCanBusPlugin PROPERTY QT_PLUGIN_CLASS_NAME "PassThruCanBusPlugin")
