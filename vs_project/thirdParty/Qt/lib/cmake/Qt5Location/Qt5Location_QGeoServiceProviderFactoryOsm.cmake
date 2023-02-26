
add_library(Qt5::QGeoServiceProviderFactoryOsm MODULE IMPORTED)


_populate_Location_plugin_properties(QGeoServiceProviderFactoryOsm RELEASE "geoservices/qtgeoservices_osm.dll" TRUE)
_populate_Location_plugin_properties(QGeoServiceProviderFactoryOsm DEBUG "geoservices/qtgeoservices_osmd.dll" TRUE)

list(APPEND Qt5Location_PLUGINS Qt5::QGeoServiceProviderFactoryOsm)
set_property(TARGET Qt5::Location APPEND PROPERTY QT_ALL_PLUGINS_geoservices Qt5::QGeoServiceProviderFactoryOsm)
set_property(TARGET Qt5::QGeoServiceProviderFactoryOsm PROPERTY QT_PLUGIN_TYPE "geoservices")
set_property(TARGET Qt5::QGeoServiceProviderFactoryOsm PROPERTY QT_PLUGIN_EXTENDS "")
set_property(TARGET Qt5::QGeoServiceProviderFactoryOsm PROPERTY QT_PLUGIN_CLASS_NAME "QGeoServiceProviderFactoryOsm")
