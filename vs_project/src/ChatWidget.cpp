/*聊天输入板
*/

#include "ChatWidget.h"
#include "LAppLive2DManager.hpp"
#include "LAppModel.hpp"
#include "NetworkUtils.h"
#include "LApp.h"
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>
#include <QtGui/qclipboard.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qfontmetrics.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qfile.h>
#include <io.h>
#include <QtCore/qdir.h>
#include <iostream>
#include <thread>

using namespace std;

ConversationWidget::ConversationWidget()
{
	//置顶无边框
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
	//透明背景
	setAttribute(Qt::WA_TranslucentBackground);
	//大小
	setFixedSize(LAppConfig::_ChatWidgetWidth, LAppConfig::_ChatWidgetHeight);
	_font.setFamily(QString::fromUtf8(LAppConfig::_ChatWidgetFontFamily.c_str()));
	_font.setPointSizeF(LAppConfig::_ChatWidgetFontSize);
	LAppConfig::_WaitChatResponse = false;

	_frame = new QFrame(this);
	_frame->setFixedSize(LAppConfig::_ChatWidgetWidth, LAppConfig::_ChatWidgetHeight);

	inputArea = new QTextEdit();
	inputArea->setFont(_font);
	inputArea->setPlaceholderText(QString::fromLocal8Bit("prompt..."));
	inputArea->setStyleSheet(
		QString("QTextEdit {background-color: rgba(0, 0, 0, 0); color: ").append(LAppConfig::_ChatWidgetFontColor.c_str()).append("; border: none; }")
	);
	_frame->setObjectName("ChatWidget");
	_frame->setStyleSheet(
		QString("QScrollBar:vertical{width: 8px;background: #DDD;border: none}"
			"QScrollBar::handle:vertical{background: #AAA;}"
			"QScrollBar::handle:vertical:hover{background: #888;}"
			"QWidget#ChatWidget{background-color: ").append(LAppConfig::_ChatWidgetBackgroundColor.c_str()).append("; }")
	);
	_Send = new QPushButton(QString::fromLocal8Bit("发送"));
	_Close = new QPushButton(QString::fromLocal8Bit("关闭"));
	grid = new QGridLayout();
	grid->addWidget(inputArea, 0, 0, 2, 4);
	grid->addWidget(_Send, 4, 3, 1, 1);
	grid->addWidget(_Close, 4, 2, 1, 1);
	_frame->setLayout(grid);
	
	connect(_Send, SIGNAL(clicked()), SLOT(SendRequest()));
	connect(_Close, SIGNAL(clicked()), SLOT(Cancel()));

	mouseX = 0;
	mouseY = 0;
}

void ConversationWidget::Release()
{
	_Send->deleteLater();
	_Close->deleteLater();
	grid->deleteLater();
	inputArea->deleteLater();
	close();
}

void ConversationWidget::mousePressEvent(QMouseEvent* e)
{
	mouseX = e->pos().x();
	mouseY = e->pos().y();
}

void ConversationWidget::mouseMoveEvent(QMouseEvent* e)
{
	if (e->buttons() == Qt::RightButton)
	{
		move(x() + e->pos().x() - mouseX, y() + e->pos().y() - mouseY);
	}
}

//移动到角色位置
void ConversationWidget::AttachToCharacter()
{
	GLWidget* win = LApp::GetInstance()->GetWindow();
	move(win->x() + (win->width() - width()) / 2, win->y() + (win->height() - height()) * 2 / 3);
}

//弹出
void ConversationWidget::getInput()
{
	_msg.clear();
	inputArea->clear();
	AttachToCharacter();
	show();
}


//处理聊天文本发送和接收
void ConversationWidget::ProcessNetworkResponse()
{
	inputArea->setPlaceholderText("waiting...");
	_Send->setEnabled(false);
	LAppConfig::_WaitChatResponse = true;
	GLWidget* win = LApp::GetInstance()->GetWindow();
	string x = _msg.toUtf8();
	
	LApp::GetInstance()->GetWindow()->GetDialog()->WaitChatResponse();
	string text, soundPath;
	if (LAppConfig::_CustomChatServerOn)
	{
		ChatAPI::Chat(x, text, soundPath);
	}
	else {
		ChatAPI::AskMlyai(x, text);
	}
	QDir dir(LAppConfig::_ChatSavePath.c_str());
	if (!dir.exists())
	{
		dir.mkpath(".");
	}
	LAppLive2DManager::GetInstance()->GetModel(0)->Speak(text.c_str(), soundPath.c_str());
	QDateTime date = QDateTime::currentDateTime();
	QFile f(string(LAppConfig::_ChatSavePath).append("/").append(date.toString("yyyy-MM-dd").toStdString()).append(".txt").c_str());
	f.open(QIODevice::Append);
	f.write(date.toString("[yyyy-MM-dd hh:mm:ss]\n").toStdString().c_str());
	f.write(LAppConfig::_UserName.c_str());
	f.write(": ");
	f.write(_msg.toUtf8().toStdString().c_str());
	f.write("\n");
	f.write(QString(LAppConfig::_AppName.c_str()).append(": ").toUtf8().toStdString().c_str());
	f.write(QString::fromUtf8(text.c_str()).toUtf8().toStdString().c_str());
	f.write("\n");
	f.close();
	LAppConfig::_WaitChatResponse = false;
	_Send->setEnabled(true);
	inputArea->setPlaceholderText("prompt...");
}

void ConversationWidget::Cancel() {
	inputArea->clear();
	close();
}

//键盘输入事件处理，不含输入法输入
void ConversationWidget::SendRequest()
{
	if (LAppConfig::_WaitChatResponse) return;
	else
	{
		if (inputArea->toPlainText().isEmpty()) return;
		_msg = inputArea->toPlainText();
		inputArea->clear();
		if (!LAppConfig::_WaitChatResponse) 
		{
			PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);   //停止当前语音
			LAppLive2DManager::GetInstance()->GetModel(0)->StopLipSync();  //停止口型同步
			std::thread(&ConversationWidget::ProcessNetworkResponse, this).detach();  //启动线程发送聊天文本
		}
	}
}