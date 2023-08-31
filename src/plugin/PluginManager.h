#pragma once
#include "../interface/IPlugin.h"
#include "../interface/IPluginSettings.h"
#include <unordered_map>
#include <QtCore/qpluginloader.h>
#include <QtWidgets/qlistwidget.h>
#include <QtCore/qhash.h>

class PluginItemView;

class PluginManager;

namespace {
	PluginManager* _instance = NULL;
}

class Plugin 
{
private:
	friend PluginManager;
	IPlugin* _instance;
	QPluginLoader* loader;
	PluginItemView* view;
	QString UUID;
	bool activated;
	void GetInstance();
public:
	Plugin(const char* filePath);
	~Plugin();
	const QString& GetUUID();
	void Activate();
	void Deactivate();
	bool IsActivated();
	void OnLaunch();
	void OnShutdown();
	void OnScheduledTask();
	void BindItemView(PluginItemView* view);
	QString Name();
	QString Version();
	QString Author();
	QString ID();
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
	QHash<QString, Plugin*>& GetPlugins();
};