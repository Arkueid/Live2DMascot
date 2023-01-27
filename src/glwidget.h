#pragma once
#include <QtWidgets/qopenglwidget.h>
#include <QtGui/qevent.h>
#include <QtWidgets/qsystemtrayicon.h>
#include <QtWidgets/qaction.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qtextbrowser.h>
#include <QtWidgets/qwidget.h>
#include <QtCore/qpropertyanimation.h>
using namespace std;


class Dialog : public QWidget
{
protected:
	void mouseReleaseEvent(QMouseEvent* e);
public:
	Dialog();
	void pop(const char* text);
	void Release();
private:
	QTextBrowser* _textBrowser;
	QPropertyAnimation* animation;
};


class GLWidget : public QOpenGLWidget
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
	Dialog* _dialog = NULL;
public:
	GLWidget();
	~GLWidget();
	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mouseDoubleClickEvent(QMouseEvent* e);
	void showRightMenu();
	void loadConfig();
	void saveConfig();
	void Release();
	void Run();
	void showDialog(const char* text);
	bool MouseTrack() { return _mouseTrack; }
	bool Quiet() { return _keepQuiet; }
	bool OnTop() { return _onTop; }
	bool NoSound() { return _noSound; }
	bool ShowText() { return _showText; }
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void timerEvent(QTimerEvent* e);
	void keepQuiet(bool on);
	void keepMouseTrack(bool on);
	void stayOnTop(bool on);
	void setNoSound(bool on);
	void setShowText(bool on);
	int mouseX;
	int mouseY;
	bool _mouseTrack;
	bool _keepQuiet;
	bool _onTop;
	bool _noSound;
	bool _showText;
	int FPS;
	long runFor = 0;
private slots:
	void trayIconOnActivated(QSystemTrayIcon::ActivationReason reason);
	void quitOnTriggered();
	void keepQuietOnTriggered();
	void keepMouseTrackOnTriggered();
	void hideOnTriggered();
	void stayOnTopOnTriggered();
	void setNoSoundOnTriggered();
	void setShowTextOnTriggered();
};


