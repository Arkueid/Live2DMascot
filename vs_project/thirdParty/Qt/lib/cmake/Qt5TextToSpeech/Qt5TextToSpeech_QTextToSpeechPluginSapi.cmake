
add_library(Qt5::QTextToSpeechPluginSapi MODULE IMPORTED)


_populate_TextToSpeech_plugin_properties(QTextToSpeechPluginSapi RELEASE "texttospeech/qtexttospeech_sapi.dll" TRUE)
_populate_TextToSpeech_plugin_properties(QTextToSpeechPluginSapi DEBUG "texttospeech/qtexttospeech_sapid.dll" TRUE)

list(APPEND Qt5TextToSpeech_PLUGINS Qt5::QTextToSpeechPluginSapi)
set_property(TARGET Qt5::TextToSpeech APPEND PROPERTY QT_ALL_PLUGINS_texttospeech Qt5::QTextToSpeechPluginSapi)
set_property(TARGET Qt5::QTextToSpeechPluginSapi PROPERTY QT_PLUGIN_TYPE "texttospeech")
set_property(TARGET Qt5::QTextToSpeechPluginSapi PROPERTY QT_PLUGIN_EXTENDS "")
set_property(TARGET Qt5::QTextToSpeechPluginSapi PROPERTY QT_PLUGIN_CLASS_NAME "QTextToSpeechPluginSapi")
