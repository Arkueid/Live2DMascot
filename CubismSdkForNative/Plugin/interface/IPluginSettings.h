#pragma once
#include <QtCore/qpluginloader.h>


class IPluginSettings
{
public:
	virtual int ScanFolder(const char* filePath) = 0;
};