
add_library(Qt5::GLTFGeometryLoaderPlugin MODULE IMPORTED)


_populate_3DRender_plugin_properties(GLTFGeometryLoaderPlugin RELEASE "geometryloaders/gltfgeometryloader.dll" TRUE)
_populate_3DRender_plugin_properties(GLTFGeometryLoaderPlugin DEBUG "geometryloaders/gltfgeometryloaderd.dll" TRUE)

list(APPEND Qt53DRender_PLUGINS Qt5::GLTFGeometryLoaderPlugin)
set_property(TARGET Qt5::3DRender APPEND PROPERTY QT_ALL_PLUGINS_geometryloaders Qt5::GLTFGeometryLoaderPlugin)
set_property(TARGET Qt5::GLTFGeometryLoaderPlugin PROPERTY QT_PLUGIN_TYPE "geometryloaders")
set_property(TARGET Qt5::GLTFGeometryLoaderPlugin PROPERTY QT_PLUGIN_EXTENDS "")
set_property(TARGET Qt5::GLTFGeometryLoaderPlugin PROPERTY QT_PLUGIN_CLASS_NAME "GLTFGeometryLoaderPlugin")
