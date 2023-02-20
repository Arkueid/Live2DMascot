#pragma once
#include <QtWidgets/qwidget.h>


class ConversationWidget : public QWidget
{
public:
	ConversationWidget();
	void getInput();
	void AttachToCharacter();
protected:
	void paintEvent(QPaintEvent* e);
	void mouseDoubleClickEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void timerEvent(QTimerEvent* e);
	void keyPressEvent(QKeyEvent* e);
	void inputMethodEvent(QInputMethodEvent* e);
	void focusInEvent(QFocusEvent* e);
	void focusOutEvent(QFocusEvent* e);
	void ProcessNetworkResponse();
private:
	QString _text;
	QFontMetrics* _fontMetrics;
	bool blink;
	QFont _font;
	bool _focused;
};