#include "ChatHisitroyView.h"
#include <QtWidgets/qdesktopwidget.h>
#include <QtWidgets/qapplication.h>
#include <QtGui/qpainter.h>
#include <QtGui/qpainterpath.h>
#include "LApp.h"
#include "AudioUtils.h"
#include <string.h>

#pragma region ChatHistoryViewItem
ChatHistoryItemView::ChatHistoryItemView(const char* character, const char* text, const char* soundPath) {
	
	this->character = new QLabel(QString::fromLocal8Bit("¡¾").append(character).append(QString::fromLocal8Bit("¡¿")).toUtf8());
	this->text = new QLabel(QString::fromLocal8Bit("¡¸").append(text).append(QString::fromLocal8Bit("¡¹")).toUtf8());
	this->soundPath = soundPath;
	
	vbox = new QVBoxLayout;
	hbox = new QHBoxLayout;

	vbox->addWidget(this->character);
	vbox->addWidget(this->text);
	hbox->addLayout(vbox);
	hbox->addStretch(0);

	if (strlen(soundPath) != 0) {
		this->play = new QPushButton();
		this->play->setFixedSize(30, 30);
		this->play->setStyleSheet(
			QString("border-image: url(").append(LAppConfig::_ModelDir.c_str()).append("/play.png); ")
		);
		hbox->addWidget(this->play);
		connect(
			this->play, &QPushButton::clicked, [soundPath]() {
				AudioUtils::StartSound(soundPath, LAppConfig::_SoundVolume, true);
			}
		);
	}
	else {
		this->play = nullptr;
	}
	setLayout(hbox);
	setStyleSheet("QLabel{color: rgba(255, 255, 255, 220); font-size: 16px;}");
	this->text->setStyleSheet("padding: 5px");
	this->text->setFixedWidth(400);
	this->text->setWordWrap(true);

	this->text->adjustSize();
	adjustSize();
}

ChatHistoryItemView::~ChatHistoryItemView() {

}
#pragma endregion


#pragma region ChatHistoryView
ChatHistoryView::ChatHistoryView() {
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground);

	view = new QListWidget(this);
	view->setStyleSheet(
		"QScrollBar::handle:vertical{background: #AAA;}"
		"QScrollBar::handle:vertical:hover{background: #888;}"
		"QScrollBar:vertical{width: 8px;background: #DDD;border: none}"
	);

	_currentAnimation = new QPropertyAnimation(this, "geometry");
	_currentAnimation->setDuration(500);
	setFixedSize(700, 450);
	view->setFixedSize(650, 400);
	view->move(25, 25);
	
	view->setStyleSheet(
		"QListWidget{background-color: rgba(0, 0, 0, 0); border: none}"
	);
	_mouseX = 0;
	_mouseY = 0;
	
}


ChatHistoryView::~ChatHistoryView() {
#ifdef CONSOLE_FLAG
	printf("[APP][WIN]ChatHistory destroyed\n");
#endif
}

void ChatHistoryView::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::RightButton)
	{
		_mouseX = e->pos().x();
		_mouseY = e->pos().y();
	}
}

void ChatHistoryView::mouseMoveEvent(QMouseEvent* e)
{
	if (e->buttons() == Qt::RightButton)
	{
		move(x() + e->pos().x() - _mouseX, y() + e->pos().y() - _mouseY);
	}
}

void ChatHistoryView::enterEvent(QEvent* e)
{
	_currentAnimation->stop();
	QDesktopWidget* screen = QApplication::desktop();
	if (this->y() <= 0)
	{
		_currentAnimation->setStartValue(QRect(x(), y(), width(), height()));
		_currentAnimation->setEndValue(QRect(x(), 0, width(), height()));
		_currentAnimation->start();
	}
	else if (this->x() + this->width() >= screen->size().width())
	{
		_currentAnimation->setStartValue(QRect(screen->width() - 5, y(), width(), height()));
		_currentAnimation->setEndValue(QRect(screen->width() - width(), y(), width(), height()));
		_currentAnimation->start();
	}
	else if (this->x() <= 0)
	{
		_currentAnimation->setStartValue(QRect(-width() + 5, y(), width(), height()));
		_currentAnimation->setEndValue(QRect(0, y(), width(), height()));
		_currentAnimation->start();
	}
}

void ChatHistoryView::leaveEvent(QEvent* e)
{
	_currentAnimation->stop();
	QDesktopWidget* screen = QApplication::desktop();
	if (this->y() <= 0)
	{
		_currentAnimation->setStartValue(QRect(x(), 0, width(), height()));
		_currentAnimation->setEndValue(QRect(x(), -height() + 5, width(), height()));
		_currentAnimation->start();
	}
	else if (this->x() + this->width() >= screen->size().width())
	{
		_currentAnimation->setStartValue(QRect(x(), y(), width(), height()));
		_currentAnimation->setEndValue(QRect(screen->width() - 5, y(), width(), height()));
		_currentAnimation->start();
	}
	else if (this->x() <= 0)
	{
		_currentAnimation->setStartValue(QRect(x(), y(), width(), height()));
		_currentAnimation->setEndValue(QRect(-width() + 5, y(), width(), height()));
		_currentAnimation->start();
	}
}

void ChatHistoryView::paintEvent(QPaintEvent* e) {
	QWidget::paintEvent(e);
	QPainter painter(this);
	QPainterPath path;
	path.addRoundRect(this->rect(), 10);
	painter.fillPath(path, QColor(0, 0, 0, 200));
}

void ChatHistoryView::Insert(const char* character, const char* text, const char *soundPath) {
	QListWidgetItem* item = new QListWidgetItem;
	ChatHistoryItemView* itemView = new ChatHistoryItemView(
		character, text, soundPath
	);
	view->addItem(item);
	item->setSizeHint(itemView->size());
	view->setItemWidget(item, itemView);
	view->setCurrentRow(view->count() - 1);
}
#pragma endregion

