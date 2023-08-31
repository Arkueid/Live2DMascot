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

	virtual void Activate() = 0;  // 关闭插件时调用

	virtual void Deactivate() = 0;  // 开启插件时调用

	virtual void Initialize(ILApp* app) = 0;  // 插件示例化后，初次使用前应该先初始化

	virtual void OnLaunch() = 0;  // 程序启动时调用

	virtual void OnScheduledTask() = 0;  // 周期性任务，每帧调用一次

	virtual void OnShutdown() = 0;  // 程序关闭时调用
};

Q_DECLARE_INTERFACE(IPlugin, PLUGIN_IID)
