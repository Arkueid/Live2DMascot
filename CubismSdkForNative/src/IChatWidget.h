#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlayout.h>
#include "IChatHistoryView.h"

class IChatWidget
{
public:
	virtual QWidget* GetSelf() = 0;
	virtual void GetInput() = 0;
	virtual IChatHistoryView* GetChatHistoryView() = 0;
	virtual QPushButton* GetSendButton() = 0;
	virtual QPushButton* GetRecordButton() = 0;
	virtual QPushButton* GetHistoryButton() = 0;
	virtual QGridLayout* GetMainLayout() = 0;
	virtual QLineEdit* GetLineEdit() = 0;
	virtual QFrame* GetFrame() = 0;
};