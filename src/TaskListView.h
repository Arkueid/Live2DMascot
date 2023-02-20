#pragma once
#include "json/json.h"
#include <QtWidgets/qwidget>
#include <QtWidgets/qmenu.h>
#include <QtCore/qpropertyanimation.h>
#include <queue>

struct item {
	Json::Value data;
	bool operator<(const item& other) const
	{
		//无期限 < 今日以后 < 今日 < 今日某时刻
		if (!this->data["reminderDateTime"]["dateTime"].isNull())
		{
			if (!other.data["reminderDateTime"]["dateTime"].isNull())
			{
				return strcmp(this->data["reminderDateTime"]["dateTime"].asCString(), other.data["reminderDateTime"]["dateTime"].asCString()) > 0;
			}
			else if (!other.data["dueDateTime"]["dateTime"].isNull())
			{
				return strcmp(this->data["reminderDateTime"]["dateTime"].asCString(), other.data["dueDateTime"]["dateTime"].asCString()) > 0;
			}
			else return false;
		}
		else if (!this->data["dueDateTime"]["dateTime"].isNull())
		{
			if (!other.data["reminderDateTime"]["dateTime"].isNull())
			{
				return strcmp(this->data["dueDateTime"]["dateTime"].asCString(), other.data["reminderDateTime"]["dateTime"].asCString()) > 0;
			}
			else if (!other.data["dueDateTime"]["dateTime"].isNull())
			{
				return strcmp(this->data["dueDateTime"]["dateTime"].asCString(), other.data["dueDateTime"]["dateTime"].asCString()) > 0;
			}
			else return false;
		}
		else return true;
	}
};

class TaskListView : public QWidget
{
	Q_OBJECT
public:
	TaskListView();
	~TaskListView();
	int GetCurrentHoverIndex(int my);
	void UpdateList();
protected:
	void paintEvent(QPaintEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	void mouseDoubleClickEvent(QMouseEvent* e);
	void enterEvent(QEvent* e);
	void leaveEvent(QEvent* e);
private:
	std::priority_queue<item> _tasklist;
	int _mouseX;
	int _mouseY;
	int _currentIndex;
	int _w;
	int _ylen;
	QMenu* _rightMenu;
	QPropertyAnimation* _currentAnimation;
private slots:
	void Refresh();
};