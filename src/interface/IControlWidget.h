#pragma once
#include <QtWidgets/qtabwidget.h>

class IAppSettings;
class IModelSettings;
class IChatSettings;
class IPluginSettings;

class IControlWidget
{
public:
	virtual QTabWidget* GetSelf() = 0;
	virtual IAppSettings* GetAppSettings() = 0;
	virtual IModelSettings* GetModelSettings() = 0;
	virtual IChatSettings* GetChatSettings() = 0;
	virtual IPluginSettings* GetPluginSettings() = 0;
	virtual void Pop() = 0;
};