#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qframe.h>
#include "LAppDefine.hpp"
#include "ChatHisitroyView.h"


class ConversationWidget : public QWidget
{
	Q_OBJECT
public:
	ConversationWidget();
	~ConversationWidget() { if (LAppDefine::DebugLogEnable) printf("[APP][WIN]ConversationWidget destroyed\n"); }
	void getInput();
	void AttachToCharacter();
	void Release();
	void ProcessNetworkResponse(bool voice=false);
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
	void ShowHistory();
	void UpdateHistory(const char* chara, const char* text, const char* sound);
private:
	QFont _font;
	QString _msg;
	QLineEdit* inputArea;
	QPushButton* _Send;
	QPushButton* _Record;
	QPushButton* _History;
	QGridLayout* grid;
	QFrame* _frame;
	ChatHistoryView* historyView;
	int mouseX;
	int mouseY;
	string text;
	string soundPath;
signals:
	void textInputTriggered(const char*, const char*, const char*);
};