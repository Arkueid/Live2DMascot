#include "LAppDelegate.hpp"
#include "LAppLive2DManager.hpp"
#include "LAppPal.hpp"
#include "conversationwidget.h"
#include "glwidget.h"
#include "LAppDefine.hpp"
#include "LAppView.hpp"
#include "LAppModel.hpp"
#include "LApp.h"
#include <QtGui/qevent.h>
#include <io.h>
#include <QtWidgets/qopenglwidget.h>
#include <QtWidgets/qaction.h>
#include <QtWidgets/qmenu.h>
#include <QtCore/qtextcodec.h>
#include <QtGui/qtextoption.h>
#include <QtGui/qpainter.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qtextbrowser.h>
#include <QtCore/qpropertyanimation.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <QtWidgets/qdesktopwidget.h>
#include <QtWidgets/qstyle.h>
#include <unordered_map>
#include <time.h>
#include <ctime>
#include <QtGui/qstylehints.h>
#include "json/json.h"


using namespace LAppDefine;
using namespace std;


void FinishedMotion(ACubismMotion* self)
{
	if (DebugLogEnable)
	{
		LAppPal::PrintLog("Motion Finished: %x", self);
	}
}

GLWidget::GLWidget() 
{
	//窗体透明设置，子窗口
	setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground, true);

	//鼠标显示可交互
	setCursor(QCursor(Qt::PointingHandCursor));
	//记录鼠标位置属性
	mouseX = 0;
	mouseY = 0;

	currentTimerIndex = -1;
	_LastState = true;
}

GLWidget::~GLWidget()
{

}

void GLWidget::initializeGL()
{
	LAppDelegate::GetInstance()->Initialize(this);
	LAppDelegate::GetInstance()->resize(this->width(), this->height());
}

void GLWidget::resizeGL(int w, int h)
{
	LAppDelegate::GetInstance()->resize(this->width(), this->height());
}

void GLWidget::paintGL()
{
	LAppDelegate::GetInstance()->update();
}

void GLWidget::timerEvent(QTimerEvent* e)
{

	QCursor cursor;
	float x = cursor.pos().x() - this->x();
	float y = cursor.pos().y() - this->y();
	LAppDelegate::GetInstance()->GetView()->TransformCoordinate(&x, &y);
	HWND hWnd = (HWND)(this->winId());
	if (!LAppConfig::_KeepQuiet && (LAppLive2DManager::GetInstance()->GetModel(0)->HitTest("Body", x, y) || LAppLive2DManager::GetInstance()->GetModel(0)->HitTest("Head", x, y)))
	{
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLongW(hWnd, GWL_EXSTYLE) & (~WS_EX_TRANSPARENT));
	}
	else
	{
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLongW(hWnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
	}
	if (LAppConfig::_MouseTrack)
	{
		LAppLive2DManager::GetInstance()->OnDrag(x, y);
	}
	if (runFor / LAppConfig::_FPS > 3600)
	{
		LAppLive2DManager::GetInstance()->GetModel(0)->StartRandomMotion("LongSittingTip", PriorityForce, FinishedMotion);
		runFor = 0;
	}
	runFor++;
	update();
}

void GLWidget::mousePressEvent(QMouseEvent* e)
{
	//配合
	LAppDelegate::GetInstance()->OnMouseCallBack(e->button(), 1);
	LAppDelegate::GetInstance()->OnMouseCallBack(e->localPos().x(), e->localPos().y());

	mouseX = e->pos().x();
	mouseY = e->pos().y();
	_dialog->raise();
}

void GLWidget::mouseMoveEvent(QMouseEvent* e)
{
	LAppDelegate::GetInstance()->OnMouseCallBack(e->localPos().x(), e->localPos().y()); //解除鼠标追踪后仍可以拖动视线
	if (e->buttons() == Qt::RightButton)
	{
		move(x() + e->pos().x() - mouseX, y() + e->pos().y() - mouseY);
		_dialog->AttachToCharacter();
		_cvWidget->AttachToCharacter();
	}
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (e->button() == Qt::RightButton) {
		_cvWidget->getInput();
		HoldText();
	}
}

void GLWidget::HoldText()
{
	_LastState = LAppConfig::_ShowText;
	setShowText(false);
	_dialog->hide();
}

void GLWidget::ReleaseText()
{
	setShowText(_LastState);
}

void GLWidget::mouseReleaseEvent(QMouseEvent* e)
{
	LAppDelegate::GetInstance()->OnMouseCallBack(e->button(), 0);
	LAppDelegate::GetInstance()->OnMouseCallBack(e->localPos().x(), e->localPos().y());
}

void GLWidget::keepQuiet(bool on)
{
	LAppConfig::_KeepQuiet = on;
}

void GLWidget::keepMouseTrack(bool on)
{
	LAppConfig::_MouseTrack = on;
	if (!on)
	{
		LAppLive2DManager::GetInstance()->OnDrag(0.0f, 0.0f);
	}
}

void GLWidget::trayIconOnActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick)
	{
		// 窗口移动最顶端
		this->setWindowFlag(Qt::WindowStaysOnTopHint, true);
		show();
		this->setWindowFlag(Qt::WindowStaysOnTopHint, false);
		show();
		QTextCodec* codec = QTextCodec::codecForName("gb2312");
		act_hide->setText(codec->toUnicode("隐藏"));
		
	}
	else  if (reason == QSystemTrayIcon::Trigger)
	{
	}
	else if (reason == QSystemTrayIcon::Context)
	{
		showRightMenu();
	}
}

void GLWidget::showRightMenu()
{
	QCursor cursor;
	rightMenu->show();
	int mX = cursor.pos().x() + 20;
	int mY = cursor.pos().y() - rightMenu->height() - 10;
	rightMenu->move(mX, mY);
}

void GLWidget::quitOnTriggered()
{
	Release();
	LApp::GetInstance()->Release();
}

void GLWidget::Release()
{
	saveConfig();
	trayIcon->hide();
	trayIcon->deleteLater();
	for each (QAction * act in rightMenu->actions())
	{
		act->deleteLater();
	}
	rightMenu->deleteLater();
	_dialog->deleteLater();
	_bgmlist->deleteLater();
	close();
}

void GLWidget::hideOnTriggered()
{
	QTextCodec* codec = QTextCodec::codecForName("gb2312");
	if (act_hide->text() == codec->toUnicode("隐藏"))
	{
		this->setVisible(false);
		act_hide->setText(codec->toUnicode("显示"));
	}
	else 
	{
		this->setVisible(true);
		act_hide->setText(codec->toUnicode("隐藏"));
	}
}
void GLWidget::keepMouseTrackOnTriggered()
{
	if (!act_keepMouseTrack->isChecked())
	{
		keepMouseTrack(false);
		act_keepMouseTrack->setChecked(false);
	}
	else
	{
		keepMouseTrack(true);
		act_keepMouseTrack->setChecked(true);
	}
}
void GLWidget::keepQuietOnTriggered()
{
	if (!act_keepQuiet->isChecked())
	{
		keepQuiet(false);
		act_keepQuiet->setChecked(false);
	}
	else
	{
		keepQuiet(true);
		act_keepQuiet->setChecked(true);
	}
}

void GLWidget::stayOnTopOnTriggered()
{
	if (!act_stayOnTop->isChecked())
	{
		stayOnTop(false);
		act_stayOnTop->setChecked(false);
	}
	else
	{
		stayOnTop(true);
		act_stayOnTop->setChecked(true);
	}
}

void GLWidget::setNoSoundOnTriggered()
{
	if (!act_setNoSound->isChecked())
	{
		setNoSound(false);
		act_setNoSound->setChecked(false);
	}
	else
	{
		setNoSound(true);
		act_setNoSound->setChecked(true);
	}
}

void GLWidget::setShowTextOnTriggered()
{
	if (!act_setShowText->isChecked())
	{
		setShowText(false);
		act_setShowText->setChecked(false);
	}
	else
	{
		setShowText(true);
		act_setShowText->setChecked(true);
	}
}

void GLWidget::setShowBgmListOnTriggered()
{
	if (!act_setShowBgmList->isChecked())
	{
		setShowBgmList(false);
		act_setShowBgmList->setChecked(false);
	}
	else
	{
		setShowBgmList(true);
		act_setShowBgmList->setChecked(true);
	}
}

void GLWidget::Run()
{
	srand((unsigned)time(NULL));
	trayIcon->show();
	show();
	_dialog->raise();
	setShowBgmList(LAppConfig::_ShowBgmList);
}

#pragma region 加载配置文件
void GLWidget::setupUI()
{
	move(LAppConfig::_LastPosX, LAppConfig::_LastPosY);
	setWindowFlag(Qt::WindowStaysOnTopHint, LAppConfig::_StayOnTop);
	//加载系统托盘
	QTextCodec* codec = QTextCodec::codecForName("gb2312");
	this->trayIcon = new QSystemTrayIcon(this);
	
	LoadConfig();

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconOnActivated(QSystemTrayIcon::ActivationReason)));

	//右键菜单
	rightMenu = new QMenu(this);
	act_quit = new QAction(codec->toUnicode("退出"));

	act_hide = new QAction(codec->toUnicode("隐藏"));

	act_keepMouseTrack = new QAction(codec->toUnicode("鼠标追踪"));
	act_keepMouseTrack->setCheckable(true);
	act_keepMouseTrack->setChecked(LAppConfig::_MouseTrack);

	act_keepQuiet = new QAction(codec->toUnicode("免打扰"));
	act_keepQuiet->setCheckable(true);
	act_keepQuiet->setChecked(LAppConfig::_KeepQuiet);

	act_stayOnTop = new QAction(codec->toUnicode("置顶显示"));
	act_stayOnTop->setCheckable(true);
	act_stayOnTop->setChecked(LAppConfig::_StayOnTop);

	act_setNoSound = new QAction(codec->toUnicode("静音"));
	act_setNoSound->setCheckable(true);
	act_setNoSound->setChecked(LAppConfig::_NoSound);

	act_setShowText = new QAction(codec->toUnicode("文本显示"));
	act_setShowText->setCheckable(true);
	act_setShowText->setChecked(LAppConfig::_ShowText);

	act_setShowBgmList = new QAction(codec->toUnicode("追番列表"));
	act_setShowBgmList->setCheckable(true);
	act_setShowBgmList->setChecked(LAppConfig::_ShowBgmList);

	act_showSettings = new QAction(codec->toUnicode("设置"));

	//右键菜单信号
	connect(act_quit, SIGNAL(triggered()), SLOT(quitOnTriggered()));
	connect(act_hide, SIGNAL(triggered()), SLOT(hideOnTriggered()));
	connect(act_keepMouseTrack, SIGNAL(triggered()), SLOT(keepMouseTrackOnTriggered()));
	connect(act_keepQuiet, SIGNAL(triggered()), SLOT(keepQuietOnTriggered()));
	connect(act_stayOnTop, SIGNAL(triggered()), SLOT(stayOnTopOnTriggered()));
	connect(act_setNoSound, SIGNAL(triggered()), SLOT(setNoSoundOnTriggered()));
	connect(act_setShowText, SIGNAL(triggered()), SLOT(setShowTextOnTriggered()));
	connect(act_setShowBgmList, SIGNAL(triggered()), SLOT(setShowBgmListOnTriggered()));
	connect(act_showSettings, SIGNAL(triggered()), SLOT(showSettingsOnTriggered()));

	rightMenu->addActions({ act_setShowBgmList, act_keepMouseTrack, act_keepQuiet, act_stayOnTop, act_setNoSound, act_setShowText, act_hide, act_showSettings});
	rightMenu->addSeparator();
	rightMenu->addAction(act_quit);
	rightMenu->setFixedWidth(120);
	rightMenu->setStyleSheet("QMenu { background-color: white; padding-top: 8px; padding-bottom: 8px; border: 1px solid rgb(214, 214, 214); padding: 4px; color: black;} QMenu::item::selected{background-color: rgb(50, 150, 240); color: white;} QMenu::item {padding: 0 0 0 20px; margin-left: 4px; margin-right: 4px;color: rgb(90, 90, 90);} QMenu::indicator{width: 13px;} QMenu::item:checked, QMenu::item:unchecked{padding-left: 7;}");
	_dialog = new Dialog();
	_bgmlist = new BgmListView();
	_cvWidget = new ConversationWidget();
	_control = new ControlWidget();
	_bgmlist->move(LAppConfig::_BgmListLastPosX, LAppConfig::_BgmListLastPosY);
	QDesktopWidget* screen = LApp::GetInstance()->GetApp()->desktop();
	if (LAppConfig::_BgmListLastPosY <= 0) _bgmlist->move(LAppConfig::_BgmListLastPosX, 0);
	else if (LAppConfig::_BgmListLastPosX <= 0) _bgmlist->move(0, LAppConfig::_BgmListLastPosY);
	else if (LAppConfig::_BgmListLastPosX >= screen->width() - 30) _bgmlist->move(screen->width() - 30, LAppConfig::_BgmListLastPosY);
}
#pragma endregion

void GLWidget::LoadConfig()
{
	resize(LAppConfig::_WindowWidth, LAppConfig::_WindowHeight);
	//加载图标
	if (_access(LAppConfig::_IconPath.c_str(), 0)==-1)
	{
		trayIcon->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
	}
	else
	{
		trayIcon->setIcon(QIcon(LAppConfig::_IconPath.c_str()));
	}

	//设置应用名称
	QTextCodec* codec0 = QTextCodec::codecForName("utf-8");
	trayIcon->setToolTip(codec0->toUnicode(LAppConfig::_AppName.c_str()));

	//设置fps
	if (currentTimerIndex != -1)
	{
		killTimer(currentTimerIndex);
	}
	currentTimerIndex = startTimer(1000 / LAppConfig::_FPS);
}

#pragma region 保存配置文件
void GLWidget::saveConfig()
{
	LApp::GetInstance()->SaveConfig();
}
#pragma endregion


void GLWidget::showDialog(const char* text)
{
	_dialog->pop(text);
}

void GLWidget::stayOnTop(bool on)
{
	LAppConfig::_StayOnTop = on;
	setWindowFlag(Qt::WindowStaysOnTopHint, on);
	show();
}

void GLWidget::setNoSound(bool on)
{
	LAppConfig::_NoSound = on;
}

void GLWidget::setShowText(bool on)
{
	LAppConfig::_ShowText = on;
}

void GLWidget::setShowBgmList(bool on)
{
	LAppConfig::_ShowBgmList = on;
	_bgmlist->setVisible(on);
}
void GLWidget::showSettingsOnTriggered()
{
	_control->Pop();
}