#include "TaskListView.h"
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
#include <thread>
#include <ctime>
#include <queue>
#include <iomanip>

using namespace std;


TaskListView::TaskListView()
{
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
	setWindowFlag(Qt::WindowStaysOnTopHint, true);
	setAttribute(Qt::WA_TranslucentBackground);
	setMouseTracking(true);
	_mouseX = 0;
	_mouseY = 0;
	_currentIndex = -1;
	_w = 250;
	_ylen = 70;
	_currentAnimation = NULL;
	Json::Value temp;
	ifstream ifs("tasks.json");
	int size = _tasklist.size();
	for (int i = 0; i < size; i++) _tasklist.pop();
	if (ifs.good())
	{
		ifs >> temp;
		for (int i = 0; i < temp.size(); i++)
		{
			_tasklist.push({ temp[i] });
		}
		_ylen = 30 * _tasklist.size() + 30;
	}
	else {
		_ylen = 70;
	}
	ifs.close();
	Refresh();
	_rightMenu = new QMenu();
	_rightMenu->setStyleSheet("QMenu { background-color: white; padding-top: 8px; padding-bottom: 8px; border: 1px solid rgb(214, 214, 214); padding: 4px; color: black;} QMenu::item::selected{background-color: rgba(50, 150, 240, 200); color: white;} QMenu::item {padding: 0 0 0 20px; margin-left: 4px; margin-right: 4px;color: rgb(90, 90, 90); width: 100px} QMenu::indicator{width: 13px;} QMenu::item:checked, QMenu::item:unchecked{padding-left: 7;}");
	QAction* refresh = _rightMenu->addAction(QString::fromLocal8Bit("刷新"));
	connect(refresh, SIGNAL(triggered()), SLOT(Refresh()));
}

TaskListView::~TaskListView()
{

}

void TaskListView::UpdateList()
{
	ToDoUtils::Update();
	Log("TaskListView", "start view update...");
	Json::Value temp;
	ifstream ifs("tasks.json");
	int size = _tasklist.size();
	for (int i = 0; i < size; i++) _tasklist.pop();
	if (ifs.good())
	{
		ifs >> temp;
		for (int i = 0; i < temp.size(); i++)
		{
			_tasklist.push({ temp[i] });
		}
		_ylen = 30 * _tasklist.size() + 30;
	}
	else {
		_ylen = 70;
	}
	ifs.close();
	update();
	Log("TaskListView", "view updated");
}

void TaskListView::Refresh()
{
	std::thread(&TaskListView::UpdateList, this).detach();
}

void TaskListView::paintEvent(QPaintEvent* e)
{
	QDateTime now = QDateTime::currentDateTime();
	QColor normal = Qt::white;
	QColor hover(240, 120, 180);
	QPainter painter(this);
	resize(_w, _ylen);
	painter.fillRect(this->rect(), QColor(0, 0, 0, 180));
	painter.fillRect(QRectF(0, 0, _w, 30), QColor(0, 0, 0, 180));

	//绘制时间
	QFont font;
	font.setFamily(QString::fromLocal8Bit("微软雅黑"));
	font.setPixelSize(15);
	painter.setFont(font);
	painter.setPen(Qt::white);
	painter.drawText(QRect(0, 0, this->width(), 30).adjusted(5, 3, -5, -3), Qt::AlignVCenter | Qt::AlignLeft, QString::fromLocal8Bit("待办事项    ").append(now.toString("yy-MM-dd hh:mm dddd")));

	//抗锯齿
	painter.setRenderHint(QPainter::Antialiasing);

	//绘制番剧列表
	if (!_tasklist.empty())
	{
		//绘制垂直轴
		QLine line(70, 30, 70, _ylen - 6);
		painter.setPen(Qt::white);
		painter.drawLine(line);
		QPixmap pixmap;
		QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd");
		QColor color;
		priority_queue<item> temp = _tasklist;
		int size = temp.size();
		for (int i = 0; i < size; i++)
		{
			color = Qt::white;
			const char* name = temp.top().data["title"].asCString();
			string ddl;  //无期限，未来，今天，今天某时刻
			if (!temp.top().data["reminderDateTime"]["dateTime"].isNull())
			{
				string taskLocalTime = ToDoUtils::ISO8601ToLocaltime(temp.top().data["reminderDateTime"]["dateTime"].asCString());
				if (strcmp(taskLocalTime.substr(5, 5).c_str(), now.mid(5, 5).toStdString().c_str()) == 0)  //今天某时刻
				{
					ddl = taskLocalTime.substr(11, 5); 
					color = QColor(50, 150, 240);
				}
				else {
					ddl = taskLocalTime.substr(5, 5);  //未来
				}
			}
			else if (!temp.top().data["dueDateTime"]["dateTime"].isNull()){
				string taskLocalTime = ToDoUtils::ISO8601ToLocaltime(temp.top().data["dueDateTime"]["dateTime"].asCString());
				if (strcmp(taskLocalTime.substr(5, 5).c_str(), now.mid(5, 5).toStdString().c_str()) == 0)  //今天某时刻
				{
					ddl = QString::fromLocal8Bit("今日内").toStdString();
					color = QColor(50, 150, 240);
				}
				else {
					ddl = taskLocalTime.substr(5, 5);  //未来
				}
			}
			else {
				ddl = QString::fromLocal8Bit("无期限").toStdString();
			}
			color = i == _currentIndex ? hover : color;
			painter.setPen(color);
			painter.drawText(
				QPoint(10, i * 30 + 50), ddl.c_str()
			);
			painter.drawText(QPoint(80, i * 30 + 50), name);
			
			QPainterPath circle;
			circle.addEllipse(67, i * 30 + 40, 7, 7);
			painter.fillPath(circle, color);
			temp.pop();
		}
	}
	else {
		painter.setPen(QColor(241, 158, 194));
		painter.drawText(QPoint(40, 50), QString::fromLocal8Bit("今天没有任务~"));
	}
}

void TaskListView::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		_mouseX = e->pos().x();
		_mouseY = e->pos().y();
	}
}

void TaskListView::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::RightButton)
	{
		_rightMenu->move(e->pos().x() + x(), e->pos().y() + y());
		_rightMenu->show();
	}
}

void TaskListView::mouseMoveEvent(QMouseEvent* e)
{
	if (e->buttons() == Qt::LeftButton)
	{
		move(x() + e->pos().x() - _mouseX, y() + e->pos().y() - _mouseY);
	}
	_currentIndex = GetCurrentHoverIndex(e->pos().y());
	this->update();
}

void TaskListView::leaveEvent(QEvent* e)
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
		show();
	}
	else if (this->x() + _w >= screen->size().width())
	{
		_currentAnimation = new QPropertyAnimation(this, "geometry");
		_currentAnimation->setDuration(200);
		_currentAnimation->setStartValue(QRect(x(), y(), width(), height()));
		_currentAnimation->setEndValue(QRect(screen->width() - 70, y(), width(), height()));
		_currentAnimation->start(QPropertyAnimation::DeleteWhenStopped);
		show();
	}
	else if (this->x() <= 0)
	{
		_currentAnimation = new QPropertyAnimation(this, "geometry");
		_currentAnimation->setDuration(200);
		_currentAnimation->setStartValue(QRect(x(), y(), width(), height()));
		_currentAnimation->setEndValue(QRect(-width() + 5, y(), width(), height()));
		_currentAnimation->start(QPropertyAnimation::DeleteWhenStopped);
		show();
	}

}

int TaskListView::GetCurrentHoverIndex(int my)
{
	if (my - 30 < 0) return -1;
	int idx = (my - 30) / 30;
	if (idx < _tasklist.size() && idx > -1) return idx;
	else return -1;
}

void TaskListView::mouseDoubleClickEvent(QMouseEvent* e)
{
	//if (e->button() == Qt::LeftButton && _currentIndex >= 0)
		//ShellExecute(NULL, "open", "explorer.exe", _tasklist[_currentIndex]["url"].asCString(), NULL, SW_SHOW);
}

void TaskListView::enterEvent(QEvent* e)
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
	else if (this->x() + _w >= screen->size().width())
	{
		_currentAnimation = new QPropertyAnimation(this, "geometry");
		_currentAnimation->setDuration(200);
		_currentAnimation->setStartValue(QRect(screen->width() - 70, y(), width(), height()));
		_currentAnimation->setEndValue(QRect(screen->width() - width(), y(), width(), height()));
		_currentAnimation->start(QPropertyAnimation::DeleteWhenStopped);
	}
	else if (this->x() <= 0)
	{
		_currentAnimation = new QPropertyAnimation(this, "geometry");
		_currentAnimation->setDuration(200);
		_currentAnimation->setStartValue(QRect(-width() + 5, y(), width(), height()));
		_currentAnimation->setEndValue(QRect(0, y(), width(), height()));
		_currentAnimation->start(QPropertyAnimation::DeleteWhenStopped);
	}
	show();
}