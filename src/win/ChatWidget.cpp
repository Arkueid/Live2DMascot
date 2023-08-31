/*Chat input
*/

#include "ChatWidget.h"
#include "../live2d/LAppLive2DManager.hpp"
#include "../live2d/LAppModel.hpp"
#include "../utils/NetworkUtils.h"
#include "../LApp.h"
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
#include <QtCore/qtimer.h>
#include "../utils/ChatHistoryDB.h"

using namespace std;

ChatWidget::ChatWidget()
{
	
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
	
	setAttribute(Qt::WA_TranslucentBackground);
	
	setFixedSize(410, 50);

	_font.setFamily(QString::fromUtf8(LAppConfig::_ChatWidgetFontFamily.c_str()));
	_font.setPointSizeF(LAppConfig::_ChatWidgetFontSize);
	LAppConfig::_WaitChatResponse = false;
	_frame = new QFrame(this);
	_frame->setFixedSize(400, 50);

	inputArea = new QLineEdit();

	inputArea->setFont(_font);
	inputArea->setPlaceholderText(QString("prompt..."));
	inputArea->setStyleSheet(
		QString("QLineEdit {background-color: transparent; color: white; border: none;} ")
	);
	_frame->setObjectName("ChatWidget");
	_frame->setStyleSheet(
		QString("QScrollBar:vertical{width: 8px;background: #DDD;}"
			"QScrollBar::handle:vertical{background: #AAA;}"
			"QScrollBar::handle:vertical:hover{background: #888;}"
			"QWidget#ChatWidget{background-color: rgba(0, 0,0,200); border-radius: 10px}")
	);
	_Send = new QPushButton();
	_Record = new QPushButton();
	_History = new QPushButton();
	_History->setCheckable(true);
	_Record->setStyleSheet("QPushButton {border-image: url(assets/voiceInput.png); } QPushButton:pressed{border-image: url(assets/voiceInput_active.png)}");
	_Send->setStyleSheet("QPushButton {border-image: url(assets/send.png); } QPushButton:pressed{border-image: url(assets/send_active.png)}");
	_History->setStyleSheet(
		"QPushButton {border-image: url(assets/history.png); } QPushButton:checked{border-image: url(assets/history_active.png)}"
	);
	_Send->setFixedSize(24, 24);
	_Record->setFixedSize(30, 30);
	_History->setFixedSize(22, 22);

	historyView = new ChatHistoryView;

	grid = new QGridLayout();
	grid->addWidget(_Record, 0, 1, 1, 1);
	grid->addWidget(inputArea, 0, 2, 1, 4);
	grid->addWidget(_Send, 0, 6, 1, 1);
	grid->addWidget(_History, 0, 0, 1, 1);

	connect(_Record, SIGNAL(pressed()), SLOT(StartVoiceInput()));
	connect(_Record, SIGNAL(released()), SLOT(StopVoiceInput()));
	connect(_History, SIGNAL(clicked()), SLOT(ShowHistory()));
	connect(this, SIGNAL(popDialogInThread(bool)), SLOT(PopDialog(bool)));
	connect(historyView, SIGNAL(closed()), SLOT(CloseHistory()));

	connect(this, SIGNAL(textInputTriggered(const char*, const char*, const char*)), this, SLOT(UpdateHistory(const char*, const char*, const char*)));

	_frame->setLayout(grid);
	connect(_Send, SIGNAL(clicked()), SLOT(SendRequest()));
	mouseX = 0;
	mouseY = 0; 

}

void ChatWidget::CloseHistory() {
	_History->setChecked(false);
}

void ChatWidget::UpdateHistory(const char* chara, const char* text, const char* sound) {
	historyView->Switch2Today();
	historyView->Insert(
		chara,
		text,
		sound
	);
}

void ChatWidget::ShowHistory() {
	historyView->setVisible(!historyView->isVisible());
	historyView->move(x() + width() / 2 - historyView->width() / 2, y() - 5 - historyView->height());
}

void ChatWidget::Release()
{
	_Send->deleteLater();
	grid->deleteLater();
	inputArea->deleteLater();
	close();
}

void ChatWidget::keyPressEvent(QKeyEvent* e) {
	if (e->key() == Qt::Key_Escape) {
		close();
	}
	else if (e->key() == Qt::Key_Return) {
		SendRequest();
	}
	else if (e->key() == Qt::Key_Alt) {
		_Record->setStyleSheet(
			QString("border-image: url(").append(LAppConfig::_AssetsDir.c_str()).append("/voiceInput_active.png)")
		);
		StartVoiceInput();
	}
} 

void ChatWidget::keyReleaseEvent(QKeyEvent* e) {
	if (e->key() == Qt::Key_Alt) {
		_Record->setStyleSheet(
			QString("border-image: url(").append(LAppConfig::_AssetsDir.c_str()).append("/voiceInput.png)")
		);
		StopVoiceInput();
	}
}

void ChatWidget::mousePressEvent(QMouseEvent* e)
{
	mouseX = e->pos().x();
	mouseY = e->pos().y();
}

void ChatWidget::mouseMoveEvent(QMouseEvent* e)
{
	if (e->buttons() == Qt::LeftButton)
	{
		move(QCursor::pos().x() - mouseX, QCursor::pos().y() - mouseY);
	}
}

//move chatinput to character
void ChatWidget::AttachToCharacter()
{
	GLWidget* win = LApp::GetInstance()->GetWindow();
	move(win->x() + (win->width() - width()) / 2, win->y() + (win->height() - height()) * 2 / 3);
}

QWidget* ChatWidget::GetSelf()
{
	return this;
}

void ChatWidget::GetInput()
{
	_msg.clear();
	inputArea->clear();
	AttachToCharacter();
	show();
	inputArea->activateWindow();
	inputArea->setFocus();
}
void ChatWidget::PopDialog(bool waitMode) {
	if (waitMode) LApp::GetInstance()->GetWindow()->GetDialog()->WaitChatResponse();
	else LApp::GetInstance()->GetWindow()->GetDialog()->Pop(text.c_str());
}

void ChatWidget::ProcessNetworkResponse(bool voice)
{
	inputArea->setPlaceholderText("waiting...");
	LAppConfig::_WaitChatResponse = true;
	GLWidget* win = LApp::GetInstance()->GetWindow();
	string x = _msg.toUtf8();
	string asr;
	LApp::GetInstance()->GetWindow()->GetDialog()->GetSelf()->moveToThread(LApp::GetInstance()->GetApp()->thread());
	emit popDialogInThread(true);
	if (LAppConfig::_CustomChatServerOn && voice)
	{
		ChatAPI::VoiceChat(string(LAppConfig::_ChatSavePath).append("/voice-input-temp.wav").c_str(), asr, text, soundPath);
	}
	else {
		if (LAppConfig::_CustomChatServerOn)
		{
			ChatAPI::Chat(x, text, soundPath);
		}
		else {
			ChatAPI::AskMlyai(x, text);
		}
	}
	emit textInputTriggered((LAppConfig::_UserName.empty() ? "用户" : LAppConfig::_UserName.c_str()), voice ? asr.c_str() : x.c_str(), "");

	QDir dir(LAppConfig::_ChatSavePath.c_str());
	if (!dir.exists())
	{
		dir.mkpath(".");
	}
	LAppLive2DManager::GetInstance()->GetModel(0)->Speak(text.c_str(), soundPath.c_str());

#ifdef SAVE_CHAT_AS_HTML
	QDateTime date = QDateTime::currentDateTime();
	QFile f(string(LAppConfig::_ChatSavePath).append("/").append(date.toString("yyyy-MM-dd").toStdString()).append(".html").c_str());
	f.open(QIODevice::Append);
	f.write(string("<div class=\"msg\" style=\"border: 1px solid black; padding: 10px\"><p class=\"time\">").append(date.toString("yyyy-MM-dd hh:mm:ss</p>\n").toStdString().c_str()).c_str());
	f.write(string("<p class=\"character\">").append(LAppConfig::_UserName).append("</p>").c_str());
	f.write(string("<p class=\"content\">").append(_msg.toUtf8().toStdString()).append("</p></div>").c_str());
	f.write(string("<div class=\"msg\" style=\"border: 1px solid black; padding: 10px\"><p class=\"time\">")
		.append(date.toString("yyyy-MM-dd hh:mm:ss</p>\n").toStdString())
		.append("<p class=\"character\">").append(LAppConfig::_AppName).append("</p>")
		.append("<p class=\"content\">").append(text).append("</p>").c_str()
	);
	if (!soundPath.empty())
		f.write(string("<audio controls><source src=\"").append(soundPath).append("\"></audio>").c_str());
	f.write("</div>");
	f.close();
#else
	ChatHistoryDB db(string(LAppConfig::_ChatSavePath).append("/chat_history.db").c_str());
	db.InsertMsg((LAppConfig::_UserName.empty() ? "用户" : LAppConfig::_UserName.c_str()), voice ? asr.c_str() : x.c_str(), "");
	db.InsertMsg((LAppConfig::_AppName.empty() ? "桌宠" : LAppConfig::_AppName.c_str()), text.c_str(), soundPath.c_str());
	db.Close();
#endif // SAVE_HTML

	emit popDialogInThread(false);
	emit textInputTriggered((LAppConfig::_AppName.empty() ? "桌宠" : LAppConfig::_AppName.c_str()), text.c_str(), soundPath.c_str());

	LAppConfig::_WaitChatResponse = false;
	inputArea->setPlaceholderText("prompt...");
}

void ChatWidget::SendRequest()
{
	if (inputArea->text().isEmpty()) close();
	if (LAppConfig::_WaitChatResponse) return;
	else
	{
		if (inputArea->text().isEmpty()) return;
		_msg = inputArea->text();
		inputArea->clear();
		if (!LAppConfig::_WaitChatResponse) 
		{
			PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);   //播放音频
			LAppLive2DManager::GetInstance()->GetModel(0)->StopLipSync();  //停止口型同步
			std::thread(&ChatWidget::ProcessNetworkResponse, this, false).detach();  //显示等待中
		}
	}
}

void ChatWidget::ProcessBaiduVoiceInput() {
	LAppConfig::_WaitChatResponse = true;
	
	_msg = VoiceInputUtils::DetectSpeech(string(LAppConfig::_ChatSavePath).append("/voice-input-temp.wav").c_str());
	if (_msg.isEmpty()) {
#ifdef CONSOLE_FLAG
		printf("[VoiceInput]Speech recognition result is empty, stop sending text\n");
#endif // CONSOLE_FLAG
		LAppConfig::_WaitChatResponse = false;
		return;
	}
	ProcessNetworkResponse();
}


void ChatWidget::StartVoiceInput() {
	if (!VoiceInputUtils::IsAvailable()) return;
	QThread* t = QThread::create(VoiceInputUtils::Record);
	connect(t, &QThread::finished, [this]{
		if (LAppConfig::_WaitChatResponse || !VoiceInputUtils::HasRecord()) return;
		if (LAppConfig::_CustomChatServerOn && LAppConfig::_CustomVoiceChatOn) {
#ifdef CONSOLE_FLAG
			printf("[APP][ChatAPI]send to custom voice chat\n");
#endif // CONSOLE_FLAG
			std::thread(&ChatWidget::ProcessNetworkResponse, this, true).detach();
		}
		else {
			std::thread(&ChatWidget::ProcessBaiduVoiceInput, this).detach();
		}
	});
	connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
	t->start();
}

void ChatWidget::StopVoiceInput() {
	VoiceInputUtils::Stop();
}