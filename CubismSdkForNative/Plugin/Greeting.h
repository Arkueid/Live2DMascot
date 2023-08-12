#pragma once
#include "interface/IPlugin.h"


class Greeting : public QObject, public IPlugin {
	Q_OBJECT
	Q_PLUGIN_METADATA(IID PLUGIN_IID FILE "plugin.json")
	Q_INTERFACES(IPlugin)
private:
	ILApp* _app;
	int tabIndex;
public:
	Greeting();
	~Greeting();

	void Activate();

	void Deactivate();

	void OnLaunch();

	void Initialize(ILApp* app);
};