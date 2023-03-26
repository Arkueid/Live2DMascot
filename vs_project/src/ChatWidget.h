#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qframe.h>
#include "LAppDefine.hpp"
#include <stack>



class ConversationWidget : public QWidget
{
	Q_OBJECT
public:
	ConversationWidget();
	~ConversationWidget() { if (LAppDefine::DebugLogEnable) printf("[APP][WIN]ConversationWidget destroyed\n"); }
	void getInput();
	void AttachToCharacter();
	void Release();
	void ProcessNetworkResponse();
	void ProcessBaiduVoiceInput();
protected:
	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
private slots:
	void SendRequest();
	void StartVoiceInput();
	void StopVoiceInput();
private:
	QFont _font;
	QString _msg;
	QLineEdit* inputArea;
	QPushButton* _Send;
	QPushButton* _Record;
	QGridLayout* grid;
	QFrame* _frame;
	int mouseX;
	int mouseY;
	std::stack<int> workflow;
};