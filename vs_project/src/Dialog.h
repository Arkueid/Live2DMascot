#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qtextbrowser.h>
#include <QtCore/qpropertyanimation.h>
#include "LAppDefine.hpp"

class Dialog : public QWidget
{
protected:
	void mouseReleaseEvent(QMouseEvent* e);
public:
	Dialog();
	~Dialog() { if (LAppDefine::DebugLogEnable) printf("Dialog destroyed\n"); }
	void Pop(const char* text);
	void Release();
	void AttachToCharacter();
	void WaitChatResponse();
	void paintEvent(QPaintEvent* e);
	void LoadConfig();
private:
	QPropertyAnimation* animation;
	int _xBorder;
	int _yBorder;
	QString _text;
	QFont _font;
	QFontMetrics* _fontMetrics;
};