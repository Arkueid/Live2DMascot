#pragma once
#include "PluginLoader.h"
#include "IPlugin.h"
#include <QtCore/qpluginloader>

class PluginLoader : public QPluginLoader
{
private:
	IPlugin* _instance;
public:
	void GetInstance();
};