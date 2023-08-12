#pragma once
#include "ILAppModel.h"
#include "IGLWidget.h"

class ILApp {
public:
	virtual IGLWidget* GetGLWidget() = 0;
	virtual ILAppModel* GetModel() = 0;
};