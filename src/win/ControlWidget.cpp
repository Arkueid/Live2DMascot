/*设置窗口
*/

#include "../live2d/LAppModel.hpp"
#include "../live2d/LAppLive2DManager.hpp"
#include "../LApp.h"
#include "ControlWidget.h"
#include <fstream>
#include <vector>
#include <io.h>
#include <QtWidgets/qtabbar.h>
#include <QtCore/qregexp.h>
#include <QtGui/qpainter.h>
#include <QtGui/qvalidator.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qtreewidget.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qlistview.h>
#include <QtGui/qfontdatabase.h>

#include <qtmaterialscrollbar.h>
#include <qtmaterialslider.h>

using namespace LAppConfig;


namespace {
	Tip* _tip = NULL;
}

Tip::Tip()
{
	resize(100, 40); //预设值，不设置第一次显示时不居中
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
	anime = new QPropertyAnimation(this, "windowOpacity");
	anime->setStartValue(1);
	anime->setEndValue(0);
	anime->setDuration(2000);
}
Tip::~Tip()
{
	if (LAppDefine::DebugLogEnable)
	printf("Tip destroyed\n");
}

void Tip::ReleaseInstance()
{
	if (_tip)
	{
		_tip->Release();
		_tip->deleteLater();
	}
}

void Tip::Release()
{
	if (this->anime)
		this->anime->deleteLater();
	close();
}
void Tip::paintEvent(QPaintEvent* e)
{
	QFontMetrics _fontMetrics(QFont(""));
	int boxWidth = _fontMetrics.width(QString(text));
	resize(boxWidth+10, 40);
	QPainter painter(this);
	painter.fillRect(0, 0, boxWidth+10, 40, QColor(0, 0, 0, 180));
	painter.setPen(Qt::white);
	painter.drawText(10, 25, QString(text).toStdString().c_str());
}
void Tip::Pop(QWidget* p, const char* text)
{
	close();
	this->text = text;
	anime->stop();
	move(p->x() + p->width() / 2 - width()/2, p->y() + 20);
	show();
	anime->start();
}
Tip* Tip::GetInstance()
{
	if (_tip == NULL) _tip = new Tip();
	return _tip;
}

ControlWidget::ControlWidget()
{
	setFixedSize(750, 500);
	setWindowFlags(Qt::Tool);
	setWindowTitle(QString::fromUtf8(_AppName.c_str()));
	setWindowFlag(Qt::WindowStaysOnTopHint, true);
	_appSettings = new AppSettings(this);
	_modelSettings = new ModelSettings(this);
	_chatSettings = new ChatSettings(this);
	addTab(_appSettings, QString("用户设置"));
	addTab(_modelSettings, QString("模型设置"));
	addTab(_chatSettings, QString("ChatAPI"));
	_pluginSettings = new PluginSettings;
	addTab(_pluginSettings, QString("插件设置"));

	QFontDatabase::addApplicationFont(QString::fromStdString(_AssetsDir).append("/PingFang-Medium.ttf"));

	setStyleSheet("QTabBar::tab{font-size: 14px; font-family: PingFang SC Medium; width: 120px; height: 30px; background-color: rgb(50, 50, 50); color: rgba(180, 180, 180, 180); padding: 0; margin: 0} "
		"QTabBar::tab:selected{background-color: rgb(30, 30, 30); color: rgba(255, 255, 255, 180); }"
		"QTabBar{background-color: rgb(37, 37, 38);}"
		"QTabWidget::pane{border: none; background-color: rgb(30, 30, 30); color: white}"
		"QTabWidget{background-color: rgb(37, 37, 38)}"
	);
	_appSettings->setStyleSheet(
		QString(
		"QWidget{color: rgba(255, 255, 255, 180); background-color: rgb(50, 50, 50); font-family: PingFang SC Medium;}"
		"QLabel{background: transparent; color: rgba(255, 255, 255, 180); font: 12px}"
		"QLineEdit{padding: 3px; color: rgb(191, 191, 191); font: 15px; border: 1px solid rgb(80, 80, 80); background-color: rgb(60, 60, 60);}"
		"QLineEdit::focus{border: 1px solid rgb(50, 120, 200)}"
		"QPushButton{font-family: PingFang SC Medium; font: 12px; border-radius: 7px; font-weight: 300; margin:5px}"
		)
	);
	_chatSettings->setStyleSheet(
		"QWidget{color: rgba(255, 255, 255, 180);background-color: rgb(50, 50, 50); font-family: PingFang SC Medium;}"
		"QLabel{background: transparent; color: rgba(255, 255, 255, 180); font: 14px}"
		"QLineEdit{padding: 3px; color: rgb(191, 191, 191); font: 15px; border: 1px solid rgb(80, 80, 80); background-color: rgb(60, 60, 60);}"
		"QLineEdit::focus{border: 1px solid rgb(50, 120, 200)}"
		"QLineEdit::!enabled{background-color: rgba(60, 60, 60, 100); color: grey; border: 1px solid rgba(80, 80, 80, 100)}"
		"QPushButton{width: 100px;font-family: PingFang SC Medium; font: 12px; border-radius: 7px; font-weight: 300; color: rgb(235, 235, 235); background-color: rgba(70, 172, 255, 190); border: none; padding-left: 7px; padding-right: 7px; padding-top: 7px; padding-bottom: 7px}"
		"QPushButton:pressed{color: rgba(255, 255, 255, 100); background-color: rgba(50, 150, 235, 150); border: none; padding-left: 10px; padding-right: 10px; padding-top: 7px; padding-bottom: 7px}"
		"QCheckBox{font: 13.5px;  font-family: PingFang SC Medium; color: rgba(255, 255, 255, 200);background: transparent;}"
	);
	_modelSettings->setStyleSheet("QWidget{color: rgba(255, 255, 255, 180); background-color: rgb(50, 50, 50); font-family: PingFang SC Medium}"
		"QLabel{background: transparent; color: rgba(255 ,255, 255, 180); font: 12px}"
		"QTreeWidget::item{height: 30px; color: rgba(255, 255, 255, 200)}"
		"QTreeWidget{outline: none; border: none; color: rgba(255, 255, 255, 245); background-color: rgb(50, 50, 50); font-family: PingFang SC Medium}"
		"QTreeWidget::item:selected:active{background-color: rgba(50, 120, 170, 180); color: rgba(255, 255, 255, 245); border: 1px solid rgb(50, 120, 240)}"
		"QTreeWidget::item:selected:!active{background-color: rgba(50, 120, 170, 90); color: rgba(255, 255, 255, 245); border: 1px solid rgb(50, 120, 240)}"
		"QTreeWidget::item:hover{background-color: rgb(50, 50, 50); border: none}"
		"QTextEdit{padding: 5px; border: 1px solid rgb(80, 80, 80); color: rgba(255, 255, 255, 245); font: 17px}"
		"QTextEdit::focus{border: 1px solid rgb(50, 120, 200)}"
		"QLineEdit{padding: 5px; color: rgb(191, 191, 191); font: 15px; border: 1px solid rgb(80, 80, 80); background-color: rgb(60, 60, 60);}"
		"QLineEdit::focus{border: 1px solid rgb(50, 120, 200)}"
		"QPushButton{font-family: PingFang SC Medium; font: 12px; border-radius: 7px; font-weight: 300; margin:5px}"
		"QComboBox{height: 30px; padding: 1px 18px 1px 3px;}"
	);
}

void ControlWidget::Release()
{
	if (_appSettings)
	{
		_appSettings->Release();
		_appSettings->deleteLater();
	}
	if (_modelSettings)
	{
		_modelSettings->Release();
		_modelSettings->deleteLater();
	}
	if (_chatSettings)
	{
		_chatSettings->Release();
		_chatSettings->deleteLater();
	}
	Tip::ReleaseInstance();
	close();
}

void ControlWidget::Pop()
{
	_appSettings->LoadConfig();
	_modelSettings->LoadConfig();
	_chatSettings->LoadConfig();
	show();
	raise();
}

