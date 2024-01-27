#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qframe.h>
#include "../live2d/LAppDefine.hpp"
#include "ChatHistoryView.h"
#include "../interface/IChatWidget.h"


class ChatWidget : public QWidget, public IChatWidget
{
	Q_OBJECT
public:
	ChatWidget();
	~ChatWidget() { if (LAppDefine::DebugLogEnable) printf("[APP][WIN]ConversationWidget destroyed\n"); }
	QWidget* GetSelf();
	void GetInput();
	void AttachToCharacter();
	void Release();
	void ProcessNetworkResponse(bool voice=false);
	void ProcessBaiduVoiceInput();
	IChatHistoryView* GetChatHistoryView() { return historyView; }
	QPushButton* GetSendButton() { return _Send; }
	QPushButton* GetRecordButton() { return _Record; }
	QPushButton* GetHistoryButton() { return _History; }
	QGridLayout* GetMainLayout() { return grid; }
	QLineEdit* GetLineEdit() { return inputArea; }
	QFrame* GetFrame() { return _frame; }
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
	void PopDialog(bool waitMode);
	void CloseHistory();
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
	void popDialogInThread(bool waitMode);
};