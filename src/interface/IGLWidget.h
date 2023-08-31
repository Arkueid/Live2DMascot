#pragma once
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qsystemtrayicon.h>
#include <QtWidgets/qopenglwidget.h>

class IControlWidget;
class IChatWidget;
class IPieMenu;
class IChatHistoryView;
class IDialog;

class IGLWidget 
{
public:
	/**
	* @brief	获取 OpenGL 窗口(Live2D 模型绘制窗口)
	*/
	virtual QOpenGLWidget* GetSelf() = 0;
	/**
	* @brief	获取设置窗口
	*/
	virtual IControlWidget* GetControlWidget() = 0;
	/**
	* @brief	获取环形菜单
	*/
	virtual IPieMenu* GetPieMenu() = 0;
	/**
	* @brief	获取聊天输入框
	*/
	virtual IChatWidget* GetConversationWidget() = 0;
	/**
	* @brief	获取系统托盘的右键菜单
	*/
	virtual QMenu* GetTrayMenu() = 0;
	/**
	* @brief	获取系统托盘
	*/
	virtual QSystemTrayIcon* GetTray() = 0;
	/**
	* @brief	获取对话框
	*/
	virtual IDialog* GetDialog() = 0;
};