#pragma once
#include "..\src\interface\IPlugin.h"


class Hitokoto : public QObject, public IPlugin {
	Q_OBJECT
	Q_PLUGIN_METADATA(IID PLUGIN_IID FILE "plugin.json")
	Q_INTERFACES(IPlugin)
private:
	ILApp* _app;
	QWidget* _settings;
	int frameCount;
public:
	Hitokoto();
	~Hitokoto();

	void Activate();

	void Deactivate();

	void Initialize(ILApp* app);

	void OnLaunch();

	void OnScheduledTask();

	void OnShutdown();

	QString GetRandomSentence();
};