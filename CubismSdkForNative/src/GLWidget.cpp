#include "LAppDelegate.hpp"
#include "LAppLive2DManager.hpp"
#include "LAppPal.hpp"
#include "ChatWidget.h"
#include "glwidget.h"
#include "LAppDefine.hpp"
#include "LAppView.hpp"
#include "LAppModel.hpp"
#include "LApp.h"
#include "AudioUtils.h"
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
	lastX = 0;
	lastY = 0;

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
		QSize screen = LApp::GetInstance()->GetApp()->desktop()->size();
		// 视线计算的x，y，用于drag(x, y)
		float dragX = mX - x();  //简单粗暴，就这样吧T_T，更不动了
		float dragY = mY - y();
		// 鼠标点击的x，y，用于HitTest(x, y)
		float clickX = mX - this->x();
		float clickY = mY - this->y();
		LAppDelegate::GetInstance()->GetView()->TransformCoordinate(&dragX, &dragY);
		LAppDelegate::GetInstance()->GetView()->TransformCoordinate(&clickX, &clickY);

		LAppConfig::_MouseOn = mX > this->x() && mX < this->x() + width() && mY > this->y() && mY < this->y() + height();

		if (LAppConfig::_MouseOn)
		{
			if (!LAppConfig::_KeepQuiet && (!LAppConfig::_TransparentBackground ||  
				strlen(LAppLive2DManager::GetInstance()->GetModel(0)->HitTest(clickX, clickY).GetRawString()) != 0
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
			LAppLive2DManager::GetInstance()->OnDrag(dragX, dragY);
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
	lastX = e->pos().x();  // mouse's relative coordinate to the character win
	lastY = e->pos().y();
	_dialog->raise();
}

void GLWidget::mouseMoveEvent(QMouseEvent* e)
{
	LAppDelegate::GetInstance()->OnMouseCallBack(e->localPos().x(), e->localPos().y()); //解除鼠标追踪后仍可以拖动视线
	if (e->buttons() == Qt::LeftButton)
	{
		// mouse's absolute coordinate - mouse's relative coordinate = character win's absolute coordinate
		move(QCursor::pos().x() - lastX, QCursor::pos().y() - lastY);
		_dialog->AttachToCharacter();
	}
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton) {
		_cvWidget->GetInput();
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent* e)
{  
	LAppDelegate::GetInstance()->OnMouseCallBack(e->button(), 0);
	LAppDelegate::GetInstance()->OnMouseCallBack(e->localPos().x(), e->localPos().y());
	if (e->button() == Qt::RightButton) {
		_pieMenu->pop(this);
	}
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
		act_hide->setText(QString("隐藏"));
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
	SaveConfig();
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
	if (_cvWidget)
	{
		_cvWidget->Release();
		_cvWidget->deleteLater();
	}
}

void GLWidget::hideOnTriggered()
{
	if (act_hide->text() == QString("隐藏"))
	{
		this->setVisible(false);
		act_hide->setText(QString("显示"));
	}
	else 
	{
		this->setVisible(true);
		act_hide->setText(QString("隐藏"));
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
		AudioUtils::StopSound();
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

void GLWidget::Run()
{
	srand((unsigned)time(NULL));
	trayIcon->show();
	show();
	_dialog->raise();
}

//加载配置文件
void GLWidget::SetupUI()
{
	move(LAppConfig::_LastPosX, LAppConfig::_LastPosY);
	setWindowFlag(Qt::WindowStaysOnTopHint, LAppConfig::_StayOnTop);
	//加载系统托盘
	this->trayIcon = new QSystemTrayIcon(this);
	
	LoadConfig();

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconOnActivated(QSystemTrayIcon::ActivationReason)));

	//右键菜单
	rightMenu = new QMenu(this);
	act_quit = new QAction(QString("退出"));

	act_hide = new QAction(QString("隐藏"));

	act_keepMouseTrack = new QAction(QString("鼠标追踪"));
	act_keepMouseTrack->setCheckable(true);
	act_keepMouseTrack->setChecked(LAppConfig::_MouseTrack);

	act_keepQuiet = new QAction(QString("免打扰"));
	act_keepQuiet->setCheckable(true);
	act_keepQuiet->setChecked(LAppConfig::_KeepQuiet);

	act_stayOnTop = new QAction(QString("置顶显示"));
	act_stayOnTop->setCheckable(true);
	act_stayOnTop->setChecked(LAppConfig::_StayOnTop);

	act_setNoSound = new QAction(QString("静音"));
	act_setNoSound->setCheckable(true);
	act_setNoSound->setChecked(LAppConfig::_NoSound);

	act_setShowText = new QAction(QString("文本显示"));
	act_setShowText->setCheckable(true);
	act_setShowText->setChecked(LAppConfig::_ShowText);

	act_showSettings = new QAction(QString("设置"));

	act_setShowBackground = new QAction(QString("显示背景"));
	act_setShowBackground->setCheckable(true);
	act_setShowBackground->setChecked(LAppConfig::_ShowBackground);

	act_setTransparentBackground = new QAction(QString("背景穿透"));
	act_setTransparentBackground->setCheckable(true);
	act_setTransparentBackground->setChecked(LAppConfig::_TransparentBackground);

	act_setTransparentCharacter = new QAction(QString("防遮挡"));
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
	connect(act_showSettings, SIGNAL(triggered()), SLOT(showSettingsOnTriggered()));
	connect(act_setShowBackground, SIGNAL(triggered()), SLOT(setShowBackgroundOnTriggered()));
	connect(act_setTransparentBackground, SIGNAL(triggered()), SLOT(setTransparentBackgroundOnTriggered()));
	connect(act_setTransparentCharacter, SIGNAL(triggered()), SLOT(setTransparentCharacterOnTriggered()));


	rightMenu->addActions({ act_keepMouseTrack, act_setTransparentBackground, act_setTransparentCharacter, act_setShowBackground, act_keepQuiet, act_stayOnTop, act_setNoSound, act_setShowText, act_hide, act_showSettings});
	rightMenu->addSeparator();
	rightMenu->addAction(act_quit);
	rightMenu->setFixedWidth(120);
	rightMenu->setStyleSheet("QMenu { background-color: white; padding-top: 8px; padding-bottom: 8px; border: 1px solid rgb(214, 214, 214); padding: 4px; color: black;} QMenu::item::selected{background-color: rgba(50, 150, 240, 200); color: white;} QMenu::item {padding: 0 0 0 20px; margin-left: 4px; margin-right: 4px;color: rgb(90, 90, 90);} QMenu::indicator{width: 13px;} QMenu::item:checked, QMenu::item:unchecked{padding-left: 7;}");

	for (const QAction* action : rightMenu->actions()) {
		connect(action, &QAction::triggered, this, &GLWidget::SaveConfig);
	}
	 
	_dialog = new Dialog();

	_cvWidget = new ChatWidget();

	_control = new ControlWidget();

	_pieMenu = new PieMenu();

	_pieMenu->setPieButtonIconSize(32);

	_pieMenu->setAlternateColors(true);

	_pieMenu->setButtonIcon(0, QString::fromStdString(LAppConfig::_AssetsDir).append("/chat.png"));

	_pieMenu->setButtonIcon(1, QString::fromStdString(LAppConfig::_AssetsDir).append("/settings.png"));

	_pieMenu->setButtonIcon(2, QString::fromStdString(LAppConfig::_AssetsDir).append("/quit.png"));

	_pieMenu->setButtonIcon(3, QString::fromStdString(LAppConfig::_AssetsDir).append("/hide.png"));

	_pieMenu->setCloseButtonIcon(QIcon(QString::fromStdString(LAppConfig::_AssetsDir).append("/close.png")));

	_pieMenu->setPinButtonIcon(QIcon(QString::fromStdString(LAppConfig::_AssetsDir).append("/pin.png")));

	connect(_pieMenu, SIGNAL(buttonClicked(uint8_t)), SLOT(pieMenuOnClicked(uint8_t)));
}

IDialog* GLWidget::GetDialog()
{
	return _dialog;
}

QOpenGLWidget* GLWidget::GetSelf()
{
	return this;
}

IControlWidget* GLWidget::GetControlWidget()
{
	return _control;
}

IPieMenu* GLWidget::GetPieMenu()
{
	return _pieMenu;
}

IChatWidget* GLWidget::GetConversationWidget()
{
	return _cvWidget;
}

QMenu* GLWidget::GetTrayMenu()
{
	return rightMenu;
}

QSystemTrayIcon* GLWidget::GetTray()
{
	return trayIcon;
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

void GLWidget::SaveConfig()
{
#ifdef CONSOLE_FLAG
	printf("[APP]config saved: %s\n", LAppConfig::_ConfigPath.c_str());
#endif
	LApp::GetInstance()->SaveConfig();
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

void GLWidget::pieMenuOnClicked(uint8_t btni) {
	switch (btni)
	{
	case 0:
		_cvWidget->GetInput();
		break;
	case 1:
		_control->Pop();
		break;
	case 2:
		this->quitOnTriggered();
		break;
	case 3:
		this->hideOnTriggered();
		break;
	default:
		break;
	}
}