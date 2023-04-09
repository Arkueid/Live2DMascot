/*╫ги╚нд╠╬©Р
*/

#include "dialog.h"
#include "LApp.h"
#include <QtCore/qtextcodec.h>
#include <QtGui/qpainter.h>
#include <QtGui/qevent.h>
#include <QtGui/qfont.h>
#include <QtGui/qpainterpath.h>
#include <QtCore/qtimer.h>

Dialog::Dialog()
{
	_font.setFamily(LAppConfig::_DialogFontFamily.c_str());
	_fontMetrics = NULL;
	LoadConfig();
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground); 
	animation = new QPropertyAnimation(this, NULL);
	animation->setStartValue(1);
	animation->setEndValue(0);
	connect(animation, SIGNAL(finished()), this, SLOT(hide()));
	_xBorder = LAppConfig::_DialogXPadding;
	_yBorder = LAppConfig::_DialogYPadding;
	show();
	hide();
}

void Dialog::Pop(const char* text)
{
	hide();
	_text = QString::fromStdString(text).toUtf8();
	animation->stop();
	setWindowFlag(Qt::WindowStaysOnTopHint, LAppConfig::_StayOnTop);
	animation->setDuration(LAppConfig::_TextFadeOutTime * 1000);
	update();
	setVisible(true);
	animation->start();
}

void Dialog::LoadConfig()
{
	_font.setPointSizeF(LAppConfig::_DialogFontSize);
	if (_fontMetrics != NULL)
	{
		delete _fontMetrics;
		_fontMetrics = NULL;
	}
	_fontMetrics = new QFontMetrics(_font);
}

void Dialog::WaitChatResponse()
{
	hide();
	_text = QString::fromUtf8("......");
	animation->stop();
	setWindowFlag(Qt::WindowStaysOnTopHint, LAppConfig::_StayOnTop);
	update();
	animation->setDuration(LAppConfig::_CustomChatServerReadTimeOut * 1000);
	setVisible(true);
	animation->start();
}

void Dialog::Release()
{
	if (animation)
		animation->deleteLater();
	close();
}

void Dialog::AttachToCharacter()
{
	GLWidget* win = LApp::GetInstance()->GetWindow();
	move(win->x() + (win->width() - width()) / 2, win->y() - height() + LAppConfig::_DialogYOffset);
}

void Dialog::mouseReleaseEvent(QMouseEvent* e)
{
	close();
}

void Dialog::paintEvent(QPaintEvent* e)
{
	int boxWidth = _fontMetrics->width(_text) > LAppConfig::_DialogMaxWidth ? LAppConfig::_DialogMaxWidth : _fontMetrics->width(_text);
	boxWidth += (2 * _xBorder);
	int boxHeight = (_fontMetrics->width(_text) / LAppConfig::_DialogMaxWidth + 1) * (_fontMetrics->descent() + _fontMetrics->leading() + _fontMetrics->ascent()) + 2 * _yBorder;
	resize(boxWidth, boxHeight);
	AttachToCharacter();
	QPainter painter(this);
	QPainterPath path;
	path.addRect(0, 0, boxWidth, boxHeight);
	painter.fillPath(path, QColor(LAppConfig::_DialogBackgroundColor.c_str()));
	painter.setFont(_font);
	painter.setPen(LAppConfig::_DialogFontColor.c_str());
	painter.drawText(_xBorder, _yBorder, boxWidth - 2 * _xBorder, boxHeight - 2*_yBorder, Qt::TextWordWrap, _text);
}
