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
	this->text = new QLabel(QString("「 ").append(text).append(QString(" 」")).toUtf8());
	this->soundPath = soundPath;
	
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
			QString("border-image: url(").append(LAppConfig::_AssetsDir.c_str()).append("/play.png); ")
		);
		//hbox->addWidget(this->play);
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
	view->setStyleSheet(
		"QScrollBar::handle:vertical{background: #AAA;}"
		"QScrollBar::handle:vertical:hover{background: #888;}"
		"QScrollBar:vertical{width: 8px;background: #DDD;border: none}"
	);

	_currentAnimation = new QPropertyAnimation(this, "geometry");
	_currentAnimation->setDuration(500);

	_dateSelector = new QComboBox(this);

	ChatHistoryDB db(string(LAppConfig::_ChatSavePath).append("/chat_history.db").c_str());
	_dateSelector->addItems(QStringList::fromVector(db.GetDates()));
	_dateSelector->move(520, 10);
	db.Close();

	connect(_dateSelector, SIGNAL(currentTextChanged(const QString&)), SLOT(selectDateOnTriggered()));

	_dateSelector->setEditable(true);
	_dateSelector->setCurrentIndex(0);
	_dateSelector->setStyleSheet("QComboBox{height: 30px; padding: 1px 18px 1px 3px; }");
	_dateSelector->setFixedWidth(120);
	selectDateOnTriggered();

	setFixedSize(700, 450);
	view->setFixedSize(650, 400);
	view->move(30, 30);
	
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

void ChatHistoryView::Switch2Today() {
	if (_dateSelector->findText(QDate::currentDate().toString("yyyy-MM-dd")) == -1) {
		_dateSelector->addItem(QDate::currentDate().toString("yyyy-MM-dd"));
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

