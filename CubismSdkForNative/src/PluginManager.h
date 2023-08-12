#pragma once
#include "IPlugin.h"
#include "IPluginSettings.h"
#include <unordered_map>
#include <QtCore/qpluginloader.h>
#include <QtWidgets/qlistwidget.h>
#include <QtCore/qhash.h>

class PluginManager;

namespace {
	PluginManager* _instance = NULL;
}

class Plugin 
{
private:
	IPlugin* _instance;
	QPluginLoader* loader;
	QString UUID;
	bool activated;
public:
	Plugin(const char* filePath);
	~Plugin();
	IPlugin* GetInstance();
	const QString& GetUUID();
	void Activate();
	void Deactivate();
	bool IsActivated();
	QString Name();
	QString Version();
	QString Author();
	QString PluginID();
	QString Desc();
};

class PluginManager {
private:
	QHash<QString, Plugin*> plugins;
public:
	PluginManager();
	~PluginManager();
	static PluginManager* GetInstance();
	static void Release();
	Plugin* Register(const char* filePath);
	void Activate(QString plugin_id);
	const Plugin* GetPlugin(QString plugin_id);
	void Deactivate(QString plugin_id);
};