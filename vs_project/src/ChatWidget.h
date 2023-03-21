#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qframe.h>
#include "LAppDefine.hpp"



class ConversationWidget : public QWidget
{
	Q_OBJECT
public:
	ConversationWidget();
	~ConversationWidget() { if (LAppDefine::DebugLogEnable) printf("[APP][WIN]ConversationWidget destroyed\n"); }
	void getInput();
	void AttachToCharacter();
	void Release();
protected:
	void ProcessNetworkResponse();
	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void keyPressEvent(QKeyEvent* e);
private slots:
	void SendRequest();
private:
	QFont _font;
	QString _msg;
	QLineEdit* inputArea;
	QPushButton* _Send;
	QGridLayout* grid;
	QFrame* _frame;
	int mouseX;
	int mouseY;
};