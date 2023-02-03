#include "conversationwidget.h"
#include "NetworkUtils.h"
#include "LApp.h"
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>
#include <QtGui/qclipboard.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qfontmetrics.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qfile.h>
#include <QtCore/qtextcodec.h>
#include <iostream>

using namespace std;

ConversationWidget::ConversationWidget()
{
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground);
	resize(400, 200);
	setEnabled(true);
	setAttribute(Qt::WA_InputMethodEnabled, true);
	blink = false;
	_fontMetrics = new QFontMetrics(_font);
	_font.setFamily("Î¢ÈíÑÅºÚ");
	_font.setPointSizeF(14);
	_outPath = LAppConfig::_NoteOutPath;
	_focused = false;
	startTimer(500);
}
void ConversationWidget::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	painter.fillRect(this->rect(), QColor(0,0,0,200));
	painter.setFont(_font);
	int xpos=20;
	int ypos=25;
	QString x;
	painter.setPen(QColor(51, 150, 210));
	QString person = QString::fromLocal8Bit("Äã: ");
	painter.drawText(xpos, ypos, person);
	xpos += _fontMetrics->width(person) + 7;
	painter.setPen(Qt::white);
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
	if (blink && _focused)
	painter.drawText(xpos, ypos, "_");
}
void ConversationWidget::focusInEvent(QFocusEvent* e) {
	_focused = true;
}
void ConversationWidget::focusOutEvent(QFocusEvent* e)
{
	_focused = false;
}
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
void ConversationWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		close();
		_text.clear();
		LApp::GetInstance()->GetWindow()->ReleaseText();
	}
}
void ConversationWidget::getInput(const char* outPath)
{
	AttachToCharacter();
	_outPath = outPath;
	show();
}
void ConversationWidget::AttachToCharacter()
{
	GLWidget* win = LApp::GetInstance()->GetWindow();
	move(win->x() + (win->width() - width()) / 2, win->y() + (win->height() - height()) * 2 / 3);
}
void ConversationWidget::getInput()
{
	AttachToCharacter();
	show();
}
void ConversationWidget::inputMethodEvent(QInputMethodEvent* e)
{
	_text.append(e->commitString());
	update();
}
void ConversationWidget::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Backspace)
	{
		_text = _text.mid(0, _text.length() - 1);
	}
	else if (e->key() == Qt::Key_Return)
	{
		close();
		if (_text.isEmpty()) {
			return;
		}
		GLWidget* win = LApp::GetInstance()->GetWindow();
		win->ReleaseText();
		const char* rsp = ChatAPI::AskMlyai(_text.toStdString().c_str());
		win->showDialog(QString::fromLocal8Bit(rsp).toStdString().c_str());
		QDateTime date = QDateTime::currentDateTime();
		QFile f(string().append(_outPath).append("/").append(date.toString("yyyy-MM-dd").toStdString()).append(".txt").c_str());
		f.open(QIODevice::Append);
		f.write(date.toString("[yyyy-MM-dd hh:mm:ss]\n").toStdString().c_str());
		f.write(LAppConfig::_UserName.c_str());
		f.write(": ");
		QTextCodec* codec = QTextCodec::codecForName("gbk");
		f.write(codec->fromUnicode(_text.toStdString().c_str()));
		f.write("\n");
		f.write("Hiyori: ");
		f.write(rsp);
		f.write("\n");
		f.close();
		_text.clear();
	}
	else {
		_text.append(e->text());
	}
	update();
}
void ConversationWidget::timerEvent(QTimerEvent* e)
{
	blink = !blink;
	update();
}