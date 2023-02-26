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
	resize(400, 200);
	//获取焦点？
	setEnabled(true);
	//开启后可检测输入法输入
	setAttribute(Qt::WA_InputMethodEnabled, true);
	blink = false;
	_fontMetrics = new QFontMetrics(_font);
	_font.setFamily("微软雅黑");
	_font.setPointSizeF(14);
	_focused = false;
	LAppConfig::_WaitChatResponse = false;
	startTimer(500);
}

//根据输入内容绘制文字，实现简单换行（不识别标点）
void ConversationWidget::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	painter.fillRect(this->rect(), QColor(0,0,0,200));
	painter.setFont(_font);
	int xpos=20;
	int ypos=25;
	QString x;
	painter.setPen(QColor(51, 150, 210));
	QString person = QString::fromLocal8Bit("你: ");
	painter.drawText(xpos, ypos, person);
	xpos += _fontMetrics->width(person) + 7;
	painter.setPen(Qt::white);
	if (LAppConfig::_WaitChatResponse)
	{
		painter.drawText(xpos, ypos, QString::fromLocal8Bit("获取服务器响应中, 不要太着急哟~"));
		return;
	}
	for (int i=0; i < _text.length(); i++)
	{
		if (xpos >= 370)
		{
			ypos += _fontMetrics->height() + 10;
			xpos = 20;
		}
		x = _text.at(i);
		int cwidth = _fontMetrics->width(x) + 7;
		painter.drawText(xpos, ypos, x);
		xpos += cwidth;
		if (ypos + _fontMetrics->height() + 10 > 220)
		{
			_text = _text.mid(0, i);
		}
	}
	if (blink && _focused && !LAppConfig::_WaitChatResponse)
	painter.drawText(xpos, ypos, "_");
}

//获得焦点
void ConversationWidget::focusInEvent(QFocusEvent* e) {
	_focused = true;
}

//失去焦点
void ConversationWidget::focusOutEvent(QFocusEvent* e)
{
	_focused = false;
}

//鼠标单击进入输入模式
void ConversationWidget::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::RightButton)
	{
		_text.append(QGuiApplication::clipboard()->text());
		update();
	}
	else {
		setFocus();
	}
}

//鼠标双击关闭
void ConversationWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		close();
		_text.clear();
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
	AttachToCharacter();
	show();
}

//输入法输入事件
void ConversationWidget::inputMethodEvent(QInputMethodEvent* e)
{
	if (LAppConfig::_WaitChatResponse) return; //已有请求时阻断输入
	_text.append(e->commitString());
	update();
}

//处理聊天文本发送和接收
void ConversationWidget::ProcessNetworkResponse()
{
	LAppConfig::_WaitChatResponse = true;
	GLWidget* win = LApp::GetInstance()->GetWindow();
	string x = _text.toUtf8();
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
	f.write(_text.toUtf8().toStdString().c_str());
	f.write("\n");
	f.write(QString(LAppConfig::_AppName.c_str()).append(": ").toUtf8().toStdString().c_str());
	f.write(QString::fromUtf8(text.c_str()).toUtf8().toStdString().c_str());
	f.write("\n");
	f.close();
	LAppConfig::_WaitChatResponse = false;
	_text.clear();
}

//键盘输入事件处理，不含输入法输入
void ConversationWidget::keyPressEvent(QKeyEvent* e)
{
	if (LAppConfig::_WaitChatResponse) return;
	if (e->key() == Qt::Key_Backspace)
	{
		_text = _text.mid(0, _text.length() - 1);
	}
	else if (e->key() == Qt::Key_Return)
	{
		close();
		if (_text.isEmpty()) return;
		if (!LAppConfig::_WaitChatResponse) 
		{
			PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);   //停止当前语音
			LAppLive2DManager::GetInstance()->GetModel(0)->StopLipSync();  //停止口型同步
			std::thread(&ConversationWidget::ProcessNetworkResponse, this).detach();  //启动线程发送聊天文本
		}
	}
	else {
		_text.append(e->text());
	}
	update();
}

//输入光标闪烁计时器
void ConversationWidget::timerEvent(QTimerEvent* e)
{
	blink = !blink;
	update();
}