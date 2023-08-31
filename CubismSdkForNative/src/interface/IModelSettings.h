#pragma once
#include <QtWidgets/qwidget.h>

class IModelSettings
{
public:
	virtual QWidget* GetSelf() = 0;
	virtual void LoadConfig() = 0;
};