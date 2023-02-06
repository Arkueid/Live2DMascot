#pragma once
#include "json/json.h"
#include <QtWidgets/qwidget>
#include <QtCore/qpropertyanimation.h>

class BgmListView : public QWidget
{
	Q_PROPERTY(int height READ height WRITE setHeight)
public:
	BgmListView();
	~BgmListView();
	int GetCurrentHoverIndex(int my);
protected:
	void paintEvent(QPaintEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseDoubleClickEvent(QMouseEvent* e);
	void enterEvent(QEvent* e);
	void leaveEvent(QEvent* e);
private:
	Json::Value _bgmlist;
	int _mouseX;
	int _mouseY;
	int _currentIndex;
	int _w;
	int _ylen;
	Json::Value _bgmitems;
	QPropertyAnimation* _currentAnimation;
};