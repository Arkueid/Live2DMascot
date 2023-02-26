
add_library(Qt5::QWindowsPrinterSupportPlugin MODULE IMPORTED)


_populate_PrintSupport_plugin_properties(QWindowsPrinterSupportPlugin RELEASE "printsupport/windowsprintersupport.dll" TRUE)
_populate_PrintSupport_plugin_properties(QWindowsPrinterSupportPlugin DEBUG "printsupport/windowsprintersupportd.dll" TRUE)

list(APPEND Qt5PrintSupport_PLUGINS Qt5::QWindowsPrinterSupportPlugin)
set_property(TARGET Qt5::PrintSupport APPEND PROPERTY QT_ALL_PLUGINS_printsupport Qt5::QWindowsPrinterSupportPlugin)
set_property(TARGET Qt5::QWindowsPrinterSupportPlugin PROPERTY QT_PLUGIN_TYPE "printsupport")
set_property(TARGET Qt5::QWindowsPrinterSupportPlugin PROPERTY QT_PLUGIN_EXTENDS "")
set_property(TARGET Qt5::QWindowsPrinterSupportPlugin PROPERTY QT_PLUGIN_CLASS_NAME "QWindowsPrinterSupportPlugin")
