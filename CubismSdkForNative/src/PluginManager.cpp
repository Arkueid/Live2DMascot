#include "PluginManager.h"
#include "LApp.h"
#include <QtCore/qdir.h>
#include <QtCore/quuid.h>

Plugin::Plugin(const char* filePath)
{
    loader = new QPluginLoader;
    loader->setFileName(filePath);
    UUID = QUuid::createUuid().toString();
    _instance = NULL;
    activated = false;
}

Plugin::~Plugin()
{
    if (_instance) {
        if (activated) {
            Deactivate();
        }
        delete _instance;
    }
    if (loader)
    {
        if (loader->isLoaded())
        {
            loader->unload();
        }
        delete loader;
    }
}

IPlugin* Plugin::GetInstance()
{
    if (!loader->isLoaded()) 
    {
        if(!loader->load()) 
        {
#ifdef CONSOLE_FLAG
            printf("[QPluginLoader]: %s\n", loader->errorString().toLocal8Bit().constData());
#endif // CONSOLE_FLAG
            return NULL;
        }
    }
    if (!_instance) _instance = qobject_cast<IPlugin*>(loader->instance());
    _instance->Initialize(LApp::GetInstance());
    return _instance;
}

const QString& Plugin::GetUUID()
{
    return UUID;
}

void Plugin::Activate()
{
    _instance->Activate();
    activated = true;
}

void Plugin::Deactivate()
{
    _instance->Deactivate();
    activated = false;
}

bool Plugin::IsActivated()
{
    return activated;
}

QString Plugin::Name()
{
    QJsonObject& metaData = loader->metaData()["MetaData"].toObject();
    return metaData["name"].toString();
}

QString Plugin::Version()
{
    QJsonObject& metaData = loader->metaData()["MetaData"].toObject();
    return metaData["version"].toString();
}

QString Plugin::Author()
{
    QJsonObject& metaData = loader->metaData()["MetaData"].toObject();
    return metaData["author"].toString();
}

QString Plugin::PluginID()
{
    QJsonObject& metaData = loader->metaData()["MetaData"].toObject();
    return metaData["id"].toString();
}

QString Plugin::Desc()
{
    QJsonObject& metaData = loader->metaData()["MetaData"].toObject();
    return metaData["desc"].toString();
}

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
}

PluginManager* PluginManager::GetInstance()
{
    if (!_instance) _instance = new PluginManager;
    return _instance;
}

void PluginManager::Release()
{
    if (_instance) delete _instance;
    _instance = NULL;
}

Plugin* PluginManager::Register(const char* filePath)
{
    Plugin* plugin = new Plugin(filePath);
    plugins.insert(plugin->GetUUID(), plugin);
    return plugin;
}

void PluginManager::Activate(QString plugin_id)
{
    Plugin* p = plugins.value(plugin_id); 
    if (!p->GetInstance()) {
#ifdef CONSOLE_FLAG
        printf("[Plugin]: failed to get instance.\n");
#endif // CONSOLE_FLAG
        return;
    }
    p->Activate();
}

const Plugin* PluginManager::GetPlugin(QString plugin_id)
{
    return plugins.value(plugin_id);
}

void PluginManager::Deactivate(QString plugin_id)
{
    plugins.value(plugin_id)->Deactivate();
}
