#pragma once
#include <QtGui/qevent.h>
#include <QtWidgets/qwidget.h>

class IDialog
{
public:
	virtual QWidget* GetSelf() = 0;
	virtual void Pop(const char* text) = 0;
	virtual void Release() = 0;
	virtual void AttachToCharacter() = 0;
	virtual void WaitChatResponse() = 0;
	virtual void paintEvent(QPaintEvent* e) = 0;
	virtual void LoadConfig() = 0;
	virtual bool IsVisible() = 0;
	virtual void TimeUp() = 0;
};