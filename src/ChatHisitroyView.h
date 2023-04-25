#pragma once
#include <iostream>
#include <QtWidgets/QWidget>
#include <QtWidgets/qlistwidget.h>
#include <QtCore/qpropertyanimation.h>
#include <QtGui/qevent.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qboxlayout.h>
#include <Windows.h>
using namespace std;


class ChatHistoryItemView : public QFrame {
public:
	ChatHistoryItemView(const char* character, const char* text, const char* soundPath);
	~ChatHistoryItemView();
private:
	QLabel* character;
	QLabel* text;
	QPushButton* play;
	QVBoxLayout* vbox;
	QHBoxLayout* hbox;
	string soundPath;
};

class ChatHistoryView : public QWidget
{
public:
	ChatHistoryView();
	~ChatHistoryView();
	void Insert(const char* character, const char* text, const char* soundPath);
	void enterEvent(QEvent* e);
	void leaveEvent(QEvent* e);
	void paintEvent(QPaintEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
private:
	QListWidget* view;  //the area showing chat history
	QPropertyAnimation* _currentAnimation;
	int _mouseX;
	int _mouseY;
};