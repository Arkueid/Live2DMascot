
add_library(Qt5::GLTFSceneExportPlugin MODULE IMPORTED)


_populate_3DRender_plugin_properties(GLTFSceneExportPlugin RELEASE "sceneparsers/gltfsceneexport.dll" TRUE)
_populate_3DRender_plugin_properties(GLTFSceneExportPlugin DEBUG "sceneparsers/gltfsceneexportd.dll" TRUE)

list(APPEND Qt53DRender_PLUGINS Qt5::GLTFSceneExportPlugin)
set_property(TARGET Qt5::3DRender APPEND PROPERTY QT_ALL_PLUGINS_sceneparsers Qt5::GLTFSceneExportPlugin)
set_property(TARGET Qt5::GLTFSceneExportPlugin PROPERTY QT_PLUGIN_TYPE "sceneparsers")
set_property(TARGET Qt5::GLTFSceneExportPlugin PROPERTY QT_PLUGIN_EXTENDS "")
set_property(TARGET Qt5::GLTFSceneExportPlugin PROPERTY QT_PLUGIN_CLASS_NAME "GLTFSceneExportPlugin")
