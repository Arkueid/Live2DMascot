#include "LAppDelegate.hpp"
#include "LAppLive2DManager.hpp"
#include "LAppPal.hpp"
#include "ChatWidget.h"
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
#include <QtGui/qtextoption.h>
#include <QtGui/qpainter.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qtextbrowser.h>
#include <QtCore/qpropertyanimation.h>
#include <iostream>
#include <fstream>
#include <QtWidgets/qdesktopwidget.h>
#include <QtWidgets/qstyle.h>
#include <time.h>
#include <QtGui/qstylehints.h>
#include "json/json.h"


using namespace LAppDefine;
using namespace std;

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
	_LastShowText = true;
	_LastNoSound = false;
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
	if (isVisible())
	{
		int mX = QCursor::pos().x();
		int mY = QCursor::pos().y();
		float x = mX - this->x();
		float y = mY - this->y();
		LAppDelegate::GetInstance()->GetView()->TransformCoordinate(&x, &y);

		LAppConfig::_MouseOn = mX > this->x() && mX < this->x() + width() && mY > this->y() && mY < this->y() + height();

		if (LAppConfig::_MouseOn)
		{
			if (!LAppConfig::_KeepQuiet && (!LAppConfig::_TransparentBackground ||  
				strlen(LAppLive2DManager::GetInstance()->GetModel(0)->HitTest(x, y).GetRawString()) != 0
					)
				)
			{
				if (_transparent)
				{
					HWND hWnd = (HWND)(this->winId());
					SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLongW(hWnd, GWL_EXSTYLE) & (~WS_EX_TRANSPARENT));
					_transparent = false;
				}
			}
			else
			{
				if (!_transparent)
				{
					HWND hWnd = (HWND)(this->winId());
					SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLongW(hWnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
					_transparent = true;
				}
			}
		}
		else {
			LAppConfig::_MouseOn = false;
		}

		if (LAppConfig::_MouseTrack)
		{
			LAppLive2DManager::GetInstance()->OnDrag(x, y);
		}

	}

	if (runFor / LAppConfig::_FPS > 3600)
	{
		LAppLive2DManager::GetInstance()->GetModel(0)->StartRandomMotion("LongSittingTip", PriorityForce);
		runFor = 0;
	}
	runFor++;
	update();
}

void GLWidget::mousePressEvent(QMouseEvent* e)
{
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
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent* e)
{
	LAppDelegate::GetInstance()->OnMouseCallBack(e->button(), 0);
	LAppDelegate::GetInstance()->OnMouseCallBack(e->localPos().x(), e->localPos().y());
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
		act_hide->setText(QString::fromLocal8Bit("隐藏"));
		// 窗口移动最顶端
		this->setWindowFlag(Qt::WindowStaysOnTopHint, true);
		setVisible(true);
		this->setWindowFlag(Qt::WindowStaysOnTopHint, LAppConfig::_StayOnTop);
		setVisible(true);
		_transparent = false;
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
	int mX = cursor.pos().x() + 10;
	int mY = cursor.pos().y() - rightMenu->height() - 10;
	rightMenu->move(mX, mY);
}

void GLWidget::quitOnTriggered()
{
	Release();
	deleteLater();
	LApp::GetInstance()->Release();
}

void GLWidget::Release()
{
	close();
	saveConfig();
	killTimer(currentTimerIndex);
	trayIcon->hide();
	trayIcon->deleteLater();
	if (_control)
	{
		_control->Release();
		_control->deleteLater();
	}
	for each (QAction * act in rightMenu->actions())
	{
		act->deleteLater();
	}
	rightMenu->deleteLater();
	if (_dialog)
	{
		_dialog->Release();
		_dialog->deleteLater();
	}
	if (_bgmlist)
	{
		_bgmlist->Release();
		_bgmlist->deleteLater();
	}
	if (_cvWidget)
	{
		_cvWidget->Release();
		_cvWidget->deleteLater();
	}
}

void GLWidget::hideOnTriggered()
{
	if (act_hide->text() == QString::fromLocal8Bit("隐藏"))
	{
		this->setVisible(false);
		act_hide->setText(QString::fromLocal8Bit("显示"));
	}
	else 
	{
		this->setVisible(true);
		act_hide->setText(QString::fromLocal8Bit("隐藏"));
	}
}
void GLWidget::keepMouseTrackOnTriggered()
{
	if (!act_keepMouseTrack->isChecked())
	{
		LAppConfig::_MouseTrack = false;
		act_keepMouseTrack->setChecked(false);
		LAppLive2DManager::GetInstance()->OnDrag(0.0f, 0.0f);
	}
	else
	{
		LAppConfig::_MouseTrack = true;
		act_keepMouseTrack->setChecked(true);
	}
}
void GLWidget::keepQuietOnTriggered()
{
	if (!act_keepQuiet->isChecked())
	{
		LAppConfig::_KeepQuiet = false;
		act_keepQuiet->setChecked(false);
	}
	else
	{
		LAppConfig::_KeepQuiet = true;
		act_keepQuiet->setChecked(true);
	}
}

void GLWidget::stayOnTopOnTriggered()
{
	if (!act_stayOnTop->isChecked())
	{
		LAppConfig::_StayOnTop = false;
		setWindowFlag(Qt::WindowStaysOnTopHint, false);
		show();
		_transparent = false;
		act_stayOnTop->setChecked(false);
	}
	else
	{
		LAppConfig::_StayOnTop = true;
		setWindowFlag(Qt::WindowStaysOnTopHint, true);
		show();
		_transparent = false;
		act_stayOnTop->setChecked(true);
	}
}

void GLWidget::setNoSoundOnTriggered()
{
	if (!act_setNoSound->isChecked())
	{
		LAppConfig::_NoSound = false;
		act_setNoSound->setChecked(false);
	}
	else
	{
		LAppConfig::_NoSound = true;
		act_setNoSound->setChecked(true);
		PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);  //立即关闭声音
	}
}

void GLWidget::setShowTextOnTriggered()
{
	if (!act_setShowText->isChecked())
	{
		LAppConfig::_ShowText = false;
		act_setShowText->setChecked(false);
		_dialog->setVisible(false);
	}
	else
	{
		LAppConfig::_ShowText = true;
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

//加载配置文件
void GLWidget::setupUI()
{
	move(LAppConfig::_LastPosX, LAppConfig::_LastPosY);
	setWindowFlag(Qt::WindowStaysOnTopHint, LAppConfig::_StayOnTop);
	//加载系统托盘
	this->trayIcon = new QSystemTrayIcon(this);
	
	LoadConfig();

	if (LAppDefine::DebugLogEnable)
		printf("[DEBUG]GLwin end load config\n");

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconOnActivated(QSystemTrayIcon::ActivationReason)));

	//右键菜单
	rightMenu = new QMenu(this);
	act_quit = new QAction(QString::fromLocal8Bit("退出"));

	act_hide = new QAction(QString::fromLocal8Bit("隐藏"));

	act_keepMouseTrack = new QAction(QString::fromLocal8Bit("鼠标追踪"));
	act_keepMouseTrack->setCheckable(true);
	act_keepMouseTrack->setChecked(LAppConfig::_MouseTrack);

	act_keepQuiet = new QAction(QString::fromLocal8Bit("免打扰"));
	act_keepQuiet->setCheckable(true);
	act_keepQuiet->setChecked(LAppConfig::_KeepQuiet);

	act_stayOnTop = new QAction(QString::fromLocal8Bit("置顶显示"));
	act_stayOnTop->setCheckable(true);
	act_stayOnTop->setChecked(LAppConfig::_StayOnTop);

	act_setNoSound = new QAction(QString::fromLocal8Bit("静音"));
	act_setNoSound->setCheckable(true);
	act_setNoSound->setChecked(LAppConfig::_NoSound);

	act_setShowText = new QAction(QString::fromLocal8Bit("文本显示"));
	act_setShowText->setCheckable(true);
	act_setShowText->setChecked(LAppConfig::_ShowText);

	act_setShowBgmList = new QAction(QString::fromLocal8Bit("追番列表"));
	act_setShowBgmList->setCheckable(true);
	act_setShowBgmList->setChecked(LAppConfig::_ShowBgmList);

	act_showSettings = new QAction(QString::fromLocal8Bit("设置"));

	act_setShowBackground = new QAction(QString::fromLocal8Bit("显示背景"));
	act_setShowBackground->setCheckable(true);
	act_setShowBackground->setChecked(LAppConfig::_ShowBackground);

	act_setTransparentBackground = new QAction(QString::fromLocal8Bit("背景穿透"));
	act_setTransparentBackground->setCheckable(true);
	act_setTransparentBackground->setChecked(LAppConfig::_TransparentBackground);

	act_setTransparentCharacter = new QAction(QString::fromLocal8Bit("防遮挡"));
	act_setTransparentCharacter->setCheckable(true);
	act_setTransparentCharacter->setChecked(LAppConfig::_TransparentCharacter);


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
	connect(act_setShowBackground, SIGNAL(triggered()), SLOT(setShowBackgroundOnTriggered()));
	connect(act_setTransparentBackground, SIGNAL(triggered()), SLOT(setTransparentBackgroundOnTriggered()));
	connect(act_setTransparentCharacter, SIGNAL(triggered()), SLOT(setTransparentCharacterOnTriggered()));


	rightMenu->addActions({ act_setShowBgmList, act_keepMouseTrack, act_setTransparentBackground, act_setTransparentCharacter, act_setShowBackground, act_keepQuiet, act_stayOnTop, act_setNoSound, act_setShowText, act_hide, act_showSettings});
	rightMenu->addSeparator();
	rightMenu->addAction(act_quit);
	rightMenu->setFixedWidth(120);
	rightMenu->setStyleSheet("QMenu { background-color: white; padding-top: 8px; padding-bottom: 8px; border: 1px solid rgb(214, 214, 214); padding: 4px; color: black;} QMenu::item::selected{background-color: rgba(50, 150, 240, 200); color: white;} QMenu::item {padding: 0 0 0 20px; margin-left: 4px; margin-right: 4px;color: rgb(90, 90, 90);} QMenu::indicator{width: 13px;} QMenu::item:checked, QMenu::item:unchecked{padding-left: 7;}");
	if (LAppDefine::DebugLogEnable)
		printf("GLwin end load rightmenu\n");

	_dialog = new Dialog();
	if (LAppDefine::DebugLogEnable)
		printf("[DEBUG]GLwin end load dialog\n");

	_bgmlist = new BgmListView();
	if (LAppDefine::DebugLogEnable)
		printf("[DEBUG]GLwin end load bgmlist\n");

	_cvWidget = new ConversationWidget();
	if (LAppDefine::DebugLogEnable)
		printf("[DEBUG]GLwin end load cv\n");

	_control = new ControlWidget();
	if (LAppDefine::DebugLogEnable)
		printf("[DEBUG]GLwin end load control\n");

	_bgmlist->move(LAppConfig::_BgmListLastPosX, LAppConfig::_BgmListLastPosY);

	QDesktopWidget* screen = LApp::GetInstance()->GetApp()->desktop();
	if (LAppConfig::_BgmListLastPosY <= 0) _bgmlist->move(LAppConfig::_BgmListLastPosX, 0);
	else if (LAppConfig::_BgmListLastPosX <= 0) _bgmlist->move(0, LAppConfig::_BgmListLastPosY);
	else if (LAppConfig::_BgmListLastPosX >= screen->width() - 5) _bgmlist->move(screen->width() - 5, LAppConfig::_BgmListLastPosY);
}

void GLWidget::LoadConfig()
{
	resize(LAppConfig::_WindowWidth, LAppConfig::_WindowHeight);
	//加载图标
	if (_access(QString::fromUtf8(LAppConfig::_IconPath.c_str()).toLocal8Bit().constData(), 0) == -1)
	{
		trayIcon->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
	}
	else
	{
		trayIcon->setIcon(QIcon(QString::fromUtf8(LAppConfig::_IconPath.c_str())));
	}

	//设置应用名称
	trayIcon->setToolTip(QString::fromUtf8(LAppConfig::_AppName.c_str()));

	//重设置fps
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

void GLWidget::setShowBgmList(bool on)
{
	LAppConfig::_ShowBgmList = on;
	_bgmlist->setVisible(on);
}

void GLWidget::showSettingsOnTriggered()
{
	_control->Pop();
}

void GLWidget::setShowBackgroundOnTriggered()
{
	if (!act_setShowBackground->isChecked())
	{
		LAppConfig::_ShowBackground = false;
		act_setShowBackground->setChecked(false);
	}
	else {
		LAppConfig::_ShowBackground = true;
		act_setShowBackground->setChecked(true);
	}
}

void GLWidget::setTransparentBackgroundOnTriggered()
{
	if (!act_setTransparentBackground->isChecked())
	{
		LAppConfig::_TransparentBackground = false;
		act_setTransparentBackground->setChecked(false);
	}
	else {
		LAppConfig::_TransparentBackground = true;
		act_setTransparentBackground->setChecked(true);
	}
}

void GLWidget::setTransparentCharacterOnTriggered()
{
	if (!act_setTransparentCharacter->isChecked())
	{
		LAppConfig::_TransparentCharacter = false;
		act_setTransparentCharacter->setChecked(false);
	}
	else {
		LAppConfig::_TransparentCharacter = true;
		act_setTransparentCharacter->setChecked(true);
	}
}