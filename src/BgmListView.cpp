#include "BgmListView.h"
#include "json/json.h"
#include <Windows.h>
#include "NetworkUtils.h"
#include <iostream>
#include <fstream>
#include <QtWidgets/qapplication.h>
#include <QtCore/qpropertyanimation.h>
#include <QtGui/qpainter.h>
#include <QtWidgets/qdesktopwidget.h>
#include <QtWidgets/qwidget.h>
#include <QtCore/qdatetime.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainterpath.h>

using namespace std;

BgmListView::BgmListView()
{
	resize(250, 200);
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setMouseTracking(true);
	_mouseX = 0;
	_mouseY = 0;
	_currentIndex = -1;
	_w = 250;
	_currentAnimation = NULL;
	ifstream ifs(BgmListUtils::_BgmListJsonPath);
	if (ifs.good())
	{
		ifs >> _bgmlist;
		QDateTime now = QDateTime::currentDateTime();
		int dayOfWeekIndex = now.date().dayOfWeek() - 1;
		_bgmitems = _bgmlist[dayOfWeekIndex]["items"];
		_ylen = 30 * _bgmitems.size() + 30;
	}
	else {
		_ylen = 200;
	}
	ifs.close();
}

BgmListView::~BgmListView()
{

}

void BgmListView::paintEvent(QPaintEvent* e)
{
	QDateTime now = QDateTime::currentDateTime();

	QPainter painter(this);
	resize(_w, _ylen);
	painter.fillRect(this->rect(), QColor(0, 0, 0, 200));
	painter.fillRect(QRectF(0, 0, _w, 30), QColor(0, 0, 0, 180));

	//ªÊ÷∆ ±º‰
	QFont font;
	font.setFamily(QString::fromLocal8Bit("Œ¢»Ì—≈∫⁄"));
	font.setPixelSize(15);
	painter.setFont(font);
	painter.setPen(Qt::white);
	painter.drawText(QRect(0, 0, this->width(), 30).adjusted(5, 3, -5, -3), Qt::AlignVCenter | Qt::AlignLeft, now.toString("yyyy-MM-dd hh:mm dddd"));

	//ªÊ÷∆¥π÷±÷·
	QLine line(20, 30, 20, _ylen - 6);
	painter.setPen(QColor(52, 152, 219));
	painter.drawLine(line);
	QPixmap pixmap;

	//øπæ‚≥›
	painter.setRenderHint(QPainter::Antialiasing);

	//ªÊ÷∆∑¨æÁ¡–±Ì
	if (!_bgmlist.isNull())
	{
		for (int i=0; i < _bgmitems.size(); i ++)
		{
			QColor color = i == _currentIndex ? QColor(46, 204, 113) : Qt::white;
			painter.setPen(color);
			const char* name = strcmp(_bgmitems[i]["name_cn"].asCString(), "") == 0 ? _bgmitems[i]["name"].asCString() : _bgmitems[i]["name_cn"].asCString();
			painter.drawText(QPoint(40, i * 30 + 50), name);
			QPainterPath circle;
			circle.addEllipse(17, i * 30 + 40, 7, 7);
			color = i == _currentIndex ? QColor(46, 204, 113) : QColor(52, 152, 219);
			painter.fillPath(circle, color);
		}
	}
}

void BgmListView::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		_mouseX = e->pos().x();
		_mouseY = e->pos().y();
	}
}

void BgmListView::mouseMoveEvent(QMouseEvent* e)
{
	if (e->buttons() == Qt::LeftButton)
	{
		move(x() + e->pos().x() - _mouseX, y() + e->pos().y() - _mouseY);
	}
	_currentIndex = GetCurrentHoverIndex(e->pos().y());
	this->update();
}

void BgmListView::leaveEvent(QEvent* e)
{

	_currentIndex = -1;
	this->update();
	QDesktopWidget* screen = QApplication::desktop();
	if (this->y() <= 0)
	{
		_currentAnimation = new QPropertyAnimation(this, "geometry");
		_currentAnimation->setDuration(200);
		_currentAnimation->setStartValue(QRect(x(), 0, width(), height()));
		_currentAnimation->setEndValue(QRect(x(), -height() + 5, width(), height()));
		_currentAnimation->start(QPropertyAnimation::DeleteWhenStopped);
		setWindowFlag(Qt::WindowStaysOnTopHint, true);
		show();
	}
	else if (this->x() + 250 >= screen->size().width())
	{
		_currentAnimation = new QPropertyAnimation(this, "geometry");
		_currentAnimation->setDuration(200);
		_currentAnimation->setStartValue(QRect(x(), y(), width(), height()));
		_currentAnimation->setEndValue(QRect(screen->width() - 30, y(), width(), height()));
		_currentAnimation->start(QPropertyAnimation::DeleteWhenStopped);
		setWindowFlag(Qt::WindowStaysOnTopHint, true);
		show();
	}
	else if (this->x() <= 0)
	{
		_currentAnimation = new QPropertyAnimation(this, "geometry");
		_currentAnimation->setDuration(200);
		_currentAnimation->setStartValue(QRect(x(), y(), width(), height()));
		_currentAnimation->setEndValue(QRect(-width() + 30, y(), width(), height()));
		_currentAnimation->start(QPropertyAnimation::DeleteWhenStopped);
		setWindowFlag(Qt::WindowStaysOnTopHint, true);
		show();
	}
	
}

int BgmListView::GetCurrentHoverIndex(int my)
{
	if (my - 30 < 0) return -1;
	int idx = (my - 30) / 30;
	if (idx < _bgmitems.size() && idx > -1) return idx;
	else return -1;
}

void BgmListView::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton && _currentIndex >= 0)
	ShellExecute(NULL, "open", "explorer.exe", _bgmitems[_currentIndex]["url"].asCString(), NULL, SW_SHOW);
}

void BgmListView::enterEvent(QEvent* e)
{

	QDesktopWidget* screen = QApplication::desktop();
	if (this->y() <= 0)
	{
		_currentAnimation = new QPropertyAnimation(this, "geometry");
		_currentAnimation->setDuration(200);
		_currentAnimation->setStartValue(QRect(x(), y(), width(), height()));
		_currentAnimation->setEndValue(QRect(x(), 0, width(), height()));
		_currentAnimation->start(QPropertyAnimation::DeleteWhenStopped);
	}
	else if (this->x() + 250 >= screen->size().width())
	{
		_currentAnimation = new QPropertyAnimation(this, "geometry");
		_currentAnimation->setDuration(200);
		_currentAnimation->setStartValue(QRect(screen->width() - 30, y(), width(), height()));
		_currentAnimation->setEndValue(QRect(screen->width() - width(), y(), width(), height()));
		_currentAnimation->start(QPropertyAnimation::DeleteWhenStopped);
	}
	else if (this->x() <= 0)
	{
		_currentAnimation = new QPropertyAnimation(this, "geometry");
		_currentAnimation->setDuration(200);
		_currentAnimation->setStartValue(QRect(-width() + 30, y(), width(), height()));
		_currentAnimation->setEndValue(QRect(0, y(), width(), height()));
		_currentAnimation->start(QPropertyAnimation::DeleteWhenStopped);
	}
	setWindowFlag(Qt::WindowStaysOnTopHint, false);
	show();
}