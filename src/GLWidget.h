#pragma once
#include "ChatWidget.h"
#include "ControlWidget.h"
#include "Dialog.h" 
#include <QtWidgets/qopenglwidget.h>
#include <QtGui/qevent.h>
#include <QtWidgets/qsystemtrayicon.h>
#include <QtWidgets/qaction.h>
#include <QtWidgets/qmenu.h>
#include "LAppDefine.hpp"
#include "PieMenu.h"
#include "IGLWidget.h"
using namespace std;

class GLWidget : public QOpenGLWidget, public IGLWidget
{
	Q_OBJECT
private:
	QSystemTrayIcon* trayIcon = NULL;  //系统托盘
	QMenu* rightMenu = NULL;  //右键菜单
	QAction* act_quit = NULL;  //关闭
	QAction* act_hide = NULL;  //隐藏
	QAction* act_keepQuiet = NULL;  //免打扰模式
	QAction* act_keepMouseTrack = NULL;  //鼠标追踪
	QAction* act_stayOnTop = NULL; //窗口置顶
	QAction* act_setNoSound = NULL;  //静音
	QAction* act_setShowText = NULL;  //显示文本
	QAction* act_showSettings = NULL; //显示设置窗口
	QAction* act_setShowBackground = NULL;  //显示背景
	QAction* act_setTransparentBackground = NULL; //背景穿透
	QAction* act_setTransparentCharacter = NULL;  //人物穿透
	Dialog* _dialog = NULL;  //文本框
	ChatWidget* _cvWidget = NULL;  //聊天输入框
	ControlWidget* _control = NULL;  //设置
	PieMenu* _pieMenu = NULL;  // 扇形菜单
	bool _LastShowText;
	bool _LastNoSound;
	bool _transparent = false;  //窗体透过
	bool _drawBackground = false;
	int lastX;
	int lastY;
	int currentTimerIndex;
	int runFor = 0;
public:
	GLWidget();
	~GLWidget() { if (LAppDefine::DebugLogEnable) printf("[APP][WIN]GLWdiget destroyed\n"); }
	void SaveConfig();
	void Release();
	void Run();
	void LoadConfig();
	void SetupUI();
	void Quit();
	IDialog* GetDialog();

	QOpenGLWidget* GetSelf();

	IControlWidget* GetControlWidget();

	IPieMenu* GetPieMenu();

	IChatWidget* GetConversationWidget();

	QMenu* GetTrayMenu();

	QSystemTrayIcon* GetTray();

protected:
	void showRightMenu();
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void timerEvent(QTimerEvent* e);
	void mouseDoubleClickEvent(QMouseEvent* e);
	void keepMouseTrack(bool on);
	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
private slots:
	void trayIconOnActivated(QSystemTrayIcon::ActivationReason reason);
	void quitOnTriggered();
	void keepQuietOnTriggered();
	void keepMouseTrackOnTriggered();
	void hideOnTriggered();
	void stayOnTopOnTriggered();
	void setNoSoundOnTriggered();
	void setShowTextOnTriggered();
	void showSettingsOnTriggered();
	void setShowBackgroundOnTriggered();
	void setTransparentBackgroundOnTriggered();
	void setTransparentCharacterOnTriggered();
	void pieMenuOnClicked(uint8_t btni);
};


