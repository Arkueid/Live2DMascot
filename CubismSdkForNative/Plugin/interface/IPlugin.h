#pragma once
#include "ILApp.h"
#include <iostream>
#include <QtWidgets/qwidget.h>
using namespace std;

#define PLUGIN_IID "com.arkueid.live2dmascot.plugin"

class IPlugin
{
public:
	~IPlugin() = default;

	virtual void Activate() = 0;

	virtual void Deactivate() = 0;

	virtual void OnLaunch() = 0;

	virtual void Initialize(ILApp* app) = 0;
};

Q_DECLARE_INTERFACE(IPlugin, PLUGIN_IID)
