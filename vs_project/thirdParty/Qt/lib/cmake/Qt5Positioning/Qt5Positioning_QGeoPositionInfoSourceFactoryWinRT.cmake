
add_library(Qt5::QGeoPositionInfoSourceFactoryWinRT MODULE IMPORTED)


_populate_Positioning_plugin_properties(QGeoPositionInfoSourceFactoryWinRT RELEASE "position/qtposition_winrt.dll" TRUE)
_populate_Positioning_plugin_properties(QGeoPositionInfoSourceFactoryWinRT DEBUG "position/qtposition_winrtd.dll" TRUE)

list(APPEND Qt5Positioning_PLUGINS Qt5::QGeoPositionInfoSourceFactoryWinRT)
set_property(TARGET Qt5::Positioning APPEND PROPERTY QT_ALL_PLUGINS_position Qt5::QGeoPositionInfoSourceFactoryWinRT)
set_property(TARGET Qt5::QGeoPositionInfoSourceFactoryWinRT PROPERTY QT_PLUGIN_TYPE "position")
set_property(TARGET Qt5::QGeoPositionInfoSourceFactoryWinRT PROPERTY QT_PLUGIN_EXTENDS "")
set_property(TARGET Qt5::QGeoPositionInfoSourceFactoryWinRT PROPERTY QT_PLUGIN_CLASS_NAME "QGeoPositionInfoSourceFactoryWinRT")
