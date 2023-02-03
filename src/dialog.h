#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qtextbrowser.h>
#include <QtCore/qpropertyanimation.h>

class Dialog : public QWidget
{
protected:
	void mouseReleaseEvent(QMouseEvent* e);
public:
	Dialog();
	void pop(const char* text);
	void Release();
	void AttachToCharacter();
	void timerEvent(QTimerEvent* e);
private:
	QTextBrowser* _textBrowser;
	QPropertyAnimation* animation;
	int _wordInterval;
	int _currentTimer;
	QString _text;
	int _currentIndex;
};