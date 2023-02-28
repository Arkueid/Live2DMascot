#pragma once
#include "json/json.h"
#include <QtWidgets/qwidget>
#include "LAppDefine.hpp"
#include <QtCore/qpropertyanimation.h>

class BgmListView : public QWidget
{
public:
	BgmListView();
	~BgmListView() { if (LAppDefine::DebugLogEnable) printf("BgmListView destroyed\n"); }
	void enterEvent(QEvent* e);
	void leaveEvent(QEvent* e);
	void Release();
protected:
	int GetCurrentHoverIndex(int my);
	void paintEvent(QPaintEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseDoubleClickEvent(QMouseEvent* e);
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