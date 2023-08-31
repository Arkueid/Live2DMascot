#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qtextbrowser.h>
#include <QtCore/qpropertyanimation.h>
#include "../live2d/LAppDefine.hpp"
#include "../interface/IDialog.h"

class Dialog : public QWidget, public IDialog
{
	Q_OBJECT
protected:
	void mouseReleaseEvent(QMouseEvent* e);
public:
	Dialog();
	~Dialog() { if (LAppDefine::DebugLogEnable) printf("[APP][WIN]Dialog destroyed\n"); }
	void Pop(const char* text);
	void Release();
	void AttachToCharacter();
	void WaitChatResponse();
	void paintEvent(QPaintEvent* e);
	bool IsVisible();
	void LoadConfig();
	QWidget* GetSelf();
private:
	QPropertyAnimation* animation;
	int _xBorder;
	int _yBorder;
	QString _text;
	QFont _font;
	QFontMetrics* _fontMetrics;
};