#pragma once
#include <iostream>
#include <QtWidgets/QWidget>
#include <QtWidgets/qlistwidget.h>
#include <QtCore/qpropertyanimation.h>
#include <QtGui/qevent.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qcombobox.h>
#include "IChatHistoryView.h"

#include <Windows.h>
using namespace std;


class ChatHistoryItemView : public QFrame
{
	Q_OBJECT;
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
private slots:
	void playOnClicked();
};

class ChatHistoryView : public QWidget, public IChatHistoryView
{
	Q_OBJECT;
public:
	ChatHistoryView();
	~ChatHistoryView();
	void Insert(const char* character, const char* text, const char* soundPath);
	void paintEvent(QPaintEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mouseDoubleClickEvent(QMouseEvent* e);
	void Switch2Today();
private:
	QListWidget* view;  //the area showing chat history
	QPropertyAnimation* _currentAnimation;
	QComboBox* _dateSelector;
	int _mouseX;
	int _mouseY;
private slots:
	void selectDateOnTriggered();
signals:
	void closed();
};