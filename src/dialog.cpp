#include "dialog.h"
#include "LApp.h"
#include <QtCore/qtextcodec.h>

Dialog::Dialog()
{
	_textBrowser = new QTextBrowser(this);
	const char* styleSheet = LAppConfig::_DialogStyleSheet.c_str();
	setStyleSheet(styleSheet);
	int w = LAppConfig::_DialogWidth;
	int h = LAppConfig::_DialogHeight;
	setFixedSize(w + 10, h + 10);
	_textBrowser->setFixedSize(w, h);
	setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	_textBrowser->setAttribute(Qt::WA_TransparentForMouseEvents);
	animation = new QPropertyAnimation(this, NULL);
	animation->setStartValue(1);
	animation->setEndValue(0);
	animation->setDuration(LAppConfig::_TextFadeOutTime * 1000);
	connect(animation, SIGNAL(finished()), this, SLOT(close()));
	_textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_wordInterval = LAppConfig::_DialogWordInterval;
	_currentIndex = -1;
	_currentTimer = -1;
}

void Dialog::pop(const char* text)
{
	if (_currentIndex != -1)
	{
		killTimer(_currentTimer);
	}
	_text = QString::fromStdString(text);
	_currentIndex = 0;
	animation->stop();
	_textBrowser->clear();
	setWindowFlag(Qt::WindowStaysOnTopHint, LAppConfig::_StayOnTop);
	AttachToCharacter();
	_currentTimer = startTimer(_wordInterval);
	show();
}
void Dialog::Release()
{
	_textBrowser->deleteLater();
}

void Dialog::AttachToCharacter()
{
	GLWidget* win = LApp::GetInstance()->GetWindow();
	move(win->x() + (win->width() - width()) / 2, win->y() + (win->height() - height()) * 2 / 3);
}

void Dialog::mouseReleaseEvent(QMouseEvent* e)
{
	close();
}

void Dialog::timerEvent(QTimerEvent* e)
{
	if (_currentIndex > _text.size())
	{
		killTimer(_currentTimer);
		_currentIndex = -1;
		animation->start();
	}
	else {
		_textBrowser->setText(_text.mid(0, _currentIndex));
		_currentIndex++;
	}
}

