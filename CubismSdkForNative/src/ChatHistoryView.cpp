#include "ChatHistoryView.h"
#include <QtWidgets/qdesktopwidget.h>
#include <QtWidgets/qapplication.h>
#include <QtGui/qpainter.h>
#include <QtGui/qpainterpath.h>
#include "LApp.h"
#include "AudioUtils.h"
#include "ChatHistoryDB.h"
#include <QtCore/qdatetime.h>
#include <string.h>

ChatHistoryItemView::ChatHistoryItemView(const char* character, const char* text, const char* soundPath) {
	
	this->character = new QLabel(QString("「 ").append(character).append(QString(" 」")).toUtf8());
	this->text = new QLabel(QString(text).toUtf8());
	this->soundPath = soundPath;

	this->text->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByMouse);
	
	vbox = new QVBoxLayout;
	hbox = new QHBoxLayout;
	hbox->addStretch(0);

	vbox->addWidget(this->character);
	vbox->addWidget(this->text);
	hbox->addLayout(vbox);

	if (strlen(soundPath) != 0) {
		this->play = new QPushButton(this);
		this->play->setFixedSize(30, 30);
		this->play->setStyleSheet(
			QString("border-image: url(assets/play.png); ")
		);
		this->play->move(580, 20);
		connect(
			this->play, SIGNAL(clicked()), SLOT(playOnClicked())
		);
	}
	else {
		this->play = nullptr;
	}
	hbox->addStretch(0);

	setLayout(hbox);
	setStyleSheet("QLabel{color: rgba(255, 255, 255, 220); font-size: 16px;}");
	this->text->setWordWrap(true);
	this->text->setStyleSheet("padding: 5px");
	this->text->setFixedWidth(500);

	this->text->adjustSize();
	adjustSize();
}

ChatHistoryItemView::~ChatHistoryItemView() {
}

void ChatHistoryItemView::playOnClicked() {
	AudioUtils::StartSound(this->soundPath.c_str(), LAppConfig::_SoundVolume, true);
}

ChatHistoryView::ChatHistoryView() {
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground);

	view = new QListWidget(this);

	_currentAnimation = new QPropertyAnimation(this, "geometry");
	_currentAnimation->setDuration(500);

	_dateSelector = new QComboBox(this);

	ChatHistoryDB db(string(LAppConfig::_ChatSavePath).append("/chat_history.db").c_str());
	_dateSelector->addItems(QStringList::fromVector(db.GetDates()));
	_dateSelector->move((width() - _dateSelector->width())/2, 15);
	db.Close();

	connect(_dateSelector, SIGNAL(currentTextChanged(const QString&)), SLOT(selectDateOnTriggered()));

	_dateSelector->setEditable(true);
	_dateSelector->setCurrentIndex(0);
	_dateSelector->setView(new QListView());
	_dateSelector->setStyleSheet(
		"QWidget{border: none; font-family: PingFang SC Medium}"
		"QComboBox QAbstractItemView {outline: none; background: rgb(80, 80, 80); border: 2px solid rgb(80, 80, 80); }"
		"QComboBox QAbstractItemView::item {height: 26px; color: rgba(255, 255, 255, 210);}" 
		"QComboBox QAbstractItemView::item:hover, QComboBox QAbstractItemView::item:focus {border: none; background-color: rgba(80, 150, 244, 200)}"
		"QComboBox::drop-down{ border-image: url(assets/search.png); width: 20px; height: 20px; }" 
		"QComboBox{height: 34px; font-size: 14px; background-color: rgb(20, 20, 20); color: white; padding: 2px; }"
	);
	_dateSelector->setFixedWidth(120);
	selectDateOnTriggered();

	setFixedSize(740, 480);
	view->setFixedSize(650, 400);
	view->move(45, 45);
	
	view->setStyleSheet(
		"QListWidget{background-color: rgba(0, 0, 0, 0); border: none}"
		"QListWidget::item:hover, QListWidget::item:selected {border: none; background-color: none; }"
		"QScrollBar::handle:vertical{background: rgb(244, 143, 192);}"
		"QScrollBar:vertical{width: 6px; background: #DDD; border: none}"
		"QScrollBar{width: 6px; border: none}"
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
	_mouseX = e->pos().x();
	_mouseY = e->pos().y();
}

void ChatHistoryView::mouseMoveEvent(QMouseEvent* e)
{
	if (e->buttons() == Qt::LeftButton)
	{
		move(QCursor::pos().x() - _mouseX, QCursor::pos().y() - _mouseY);
	}
}

void ChatHistoryView::mouseDoubleClickEvent(QMouseEvent* e) {
	if (e->button() == Qt::LeftButton) {
		hide();
		emit closed();
	}
}

void ChatHistoryView::paintEvent(QPaintEvent* e) {
	QWidget::paintEvent(e);
	QPainter painter(this);
	QPainterPath path;
	painter.setRenderHints(QPainter::Antialiasing);
	path.addRoundRect(this->rect(), 5);
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

void ChatHistoryView::Switch2Today() {
	if (_dateSelector->findText(QDate::currentDate().toString("yyyy-MM-dd")) == -1) {
		_dateSelector->insertItem(0, QDate::currentDate().toString("yyyy-MM-dd"));
	}
	if (_dateSelector->currentText().compare(QDate::currentDate().toString("yyyy-MM-dd")) != 0) {
		_dateSelector->setCurrentText(QDate::currentDate().toString("yyyy-MM-dd"));
	}
}

void ChatHistoryView::selectDateOnTriggered() {
	view->clear();
	ChatHistoryDB db(string(LAppConfig::_ChatSavePath).append("/chat_history.db").c_str());
	vector<Msg> msg_ls = db.SelectByDate(_dateSelector->currentText().toStdString().c_str());
	for (Msg& msg : msg_ls) {
		Insert(msg.name.c_str(), msg.text.c_str(), msg.soundPath.c_str());
	}
}

