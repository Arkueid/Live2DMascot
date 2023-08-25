#pragma once
#include <QtCore/qpluginloader.h>


class IPluginSettings
{
public:
	virtual void ScanFolder(const char* filePath) = 0;
};