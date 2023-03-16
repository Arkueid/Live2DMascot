#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qtextedit.h>
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
private slots:
	void SendRequest();
	void Cancel();
private:
	QFont _font;
	QString _msg;
	QTextEdit* inputArea;
	QPushButton* _Send;
	QPushButton* _Close;
	QGridLayout* grid;
	QFrame* _frame;
	int mouseX;
	int mouseY;
};