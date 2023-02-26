
add_library(Qt5::DSServicePlugin MODULE IMPORTED)


_populate_Multimedia_plugin_properties(DSServicePlugin RELEASE "mediaservice/dsengine.dll" TRUE)
_populate_Multimedia_plugin_properties(DSServicePlugin DEBUG "mediaservice/dsengined.dll" TRUE)

list(APPEND Qt5Multimedia_PLUGINS Qt5::DSServicePlugin)
set_property(TARGET Qt5::Multimedia APPEND PROPERTY QT_ALL_PLUGINS_mediaservice Qt5::DSServicePlugin)
set_property(TARGET Qt5::DSServicePlugin PROPERTY QT_PLUGIN_TYPE "mediaservice")
set_property(TARGET Qt5::DSServicePlugin PROPERTY QT_PLUGIN_EXTENDS "")
set_property(TARGET Qt5::DSServicePlugin PROPERTY QT_PLUGIN_CLASS_NAME "DSServicePlugin")
