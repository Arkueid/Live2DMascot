#pragma once
#include <iostream>
#include <QtWidgets/qwidget>
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtCore/qpropertyanimation.h>
#include <QtWidgets/qslider.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qtreewidget.h>
#include <vector>
#include "LAppDefine.hpp"
#include "json/json.h"
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qtextbrowser.h>
#include <qtmaterialtextfield.h>
#include <qtmaterialslider.h>
#include <qtmaterialtoggle.h>
#include <qtmaterialraisedbutton.h>
#include "IControlWidget.h"
#include "IAppSettings.h"
#include "IChatSettings.h"
#include "IModelSettings.h"
#include "IPluginSettings.h"
#include "PluginManager.h"

class MyText : public QTextEdit {
	Q_OBJECT
signals:
	void loseFocus();
protected:
	void focusOutEvent(QFocusEvent* e)
	{
		QTextEdit::focusOutEvent(e);
		emit loseFocus();
	}
};


class Tip : public QWidget
{
public:
	Tip();
	~Tip();
	void Pop(QWidget* p, const char* text);
	static Tip* GetInstance();
	static void ReleaseInstance();
	void Release();
protected:
	void paintEvent(QPaintEvent* e);
	const char* text = NULL;
	QPropertyAnimation* anime;
};


class AppSettings : public QWidget, public IAppSettings
{
	Q_OBJECT
	QtMaterialTextField* appName;
	QtMaterialTextField* username;
	QtMaterialTextField* iconPath;
	QtMaterialTextField* fps;
	QtMaterialTextField* windowHeight;
	QtMaterialTextField* windowWidth;
	QtMaterialTextField* modelDir;
	QtMaterialTextField* motionInterval;
	QtMaterialTextField* lipSync;
	QtMaterialTextField* dialogMaxWidth;
	QtMaterialTextField* dialogFontSize;
	QtMaterialTextField* dialogYOffset;
	QtMaterialSlider* volumeSlider;
	QLabel* lbl_volume;
	QLabel* lbl_sliderVal;
	QLabel* lbl_repairMode;
	QtMaterialRaisedButton* openFile;
	QtMaterialRaisedButton* chooseDir;
	QtMaterialRaisedButton* apply;
	QtMaterialRaisedButton* reset;
	QWidget* _parent;
	QtMaterialToggle* repairModeControl;
	QtMaterialTextField* characterX;
	QtMaterialTextField* characterY;
public:
	AppSettings(QWidget* p);
	~AppSettings() { if (LAppDefine::DebugLogEnable) printf("[APP][WIN]AppSettings destroyed\n"); }
	void LoadConfig();
	void Release();
private slots:
	void OpenFile();
	void Apply();
	void Reset();
	void OpenSourceDir();
	void SetVolume();
};

class ModelSettings : public QWidget, public IModelSettings
{
	Q_OBJECT
	QWidget* _parent;
	QComboBox* model;
	QLabel* lbl_model;
	QTreeWidget* _motionGroups;
	QtMaterialRaisedButton* apply;
	QtMaterialRaisedButton* reset;
	QGridLayout* grid;
	Json::Value _modelJson;
	QLabel* lbl_motionJsonPath;
	QLabel* lbl_motionSoundPath;
	QLabel* lbl_motionText;
	QLabel* lbl_motionGroup;
	QtMaterialTextField* motionGroup;
	QComboBox* motionJsonPath;
	QComboBox* motionSoundPath;
	MyText* motionText;
	QtMaterialRaisedButton* addMotion;
	QtMaterialRaisedButton* deleteMotion;
	QtMaterialRaisedButton* changeModel;
	QtMaterialRaisedButton* addGroup;
	QtMaterialRaisedButton* deleteGroup;
	QtMaterialRaisedButton* updateGroupName;
public:
	ModelSettings(QWidget* p);
	~ModelSettings() { if (LAppDefine::DebugLogEnable) printf("[APP][WIN]ModelSettings destroyed\n"); }
	void LoadConfig();
	QWidget* GetSelf() { return this; }
	void Release();
private slots:
	void Reset();
	void Apply();
	void AddMotion();
	void DeleteMotion();
	void ShowMotionInfo(QTreeWidgetItem* w, int);
	void StartMotion(QTreeWidgetItem* w, int);
	void BindSound(const QString& x);
	void BindMotion(const QString& x);
	void BindText();
	void UpdateModel();
	void AddGroup();
	void DeleteGroup();
	void UpdateGroupName();
};

class ChatSettings : public QWidget, public IChatSettings
{
	Q_OBJECT
private:
	QtMaterialTextField* savePath;
	QtMaterialTextField* apiKey;
	QtMaterialTextField* apiSecret;
	QtMaterialTextField* hostPort;
	QtMaterialTextField* readTimeOut;
	QtMaterialTextField* route;
	QtMaterialTextField* voiceRoute;
	QLabel* lbl_savePath;
	QLabel* lbl_apiKey;
	QLabel* lbl_apiSecret;
	QLabel* lbl_hostPort;
	QLabel* lbl_readTimeOut;
	QLabel* lbl_route;
	QLabel* lbl_voiceRoute;
	QCheckBox* MlyAI;
	QCheckBox* CustomChatServer;
	QCheckBox* CustomVoiceChat;
	QtMaterialRaisedButton* chooseDir;
	QtMaterialRaisedButton* apply;
	QtMaterialRaisedButton* reset;
	QGridLayout* grid;
	QWidget* _parent;
public:
	ChatSettings(QWidget* p);
	~ChatSettings() { if (LAppDefine::DebugLogEnable) printf("[APP][WIN]ChatSettings destroyed\n"); }

	void LoadConfig();
	void Release();
	void UpdateState();
private slots:
	void Reset();
	void Apply();
	void ChooseDir();
	void MlyAIChecked();
	void CustomChecked();
	void CustomVoiceChatChecked();
};

class PluginItemView : public QWidget
{
	Q_OBJECT
private:
	Plugin* plugin;
public:
	QPushButton* btn_load;
	QString info;
	PluginItemView(Plugin* plugin);
	void UpdateText();
private slots:
	void on_btn_load_clicked();
};

class PluginSettings : public QWidget, public IPluginSettings
{
	Q_OBJECT
private:
	QListWidget* pluginList;
	QTextBrowser* pluginInfo;
	QLabel* pluginNum;
public:
	PluginSettings();
	~PluginSettings();
	void AddPlugin(Plugin* plugin);
	void ScanFolder(const char* filePath);
protected:
	void setupUI();
private slots:
	void on_plugin_item_clicked(const QModelIndex&);
};

class ControlWidget : public QTabWidget, public IControlWidget
{
private:
	AppSettings* _appSettings;
	ModelSettings* _modelSettings;
	ChatSettings* _chatSettings;
	PluginSettings* _pluginSettings;
public:
	ControlWidget();
	~ControlWidget() { if (LAppDefine::DebugLogEnable) printf("[APP][WIN]ControlWidget destroyed\n"); }
	QTabWidget* GetSelf() { return this; }
	IAppSettings* GetAppSettings() { return _appSettings; }
	IModelSettings* GetModelSettings() { return _modelSettings; }
	IChatSettings* GetChatSettings() { return _chatSettings; }
	IPluginSettings* GetPluginSettings() { return _pluginSettings; }
	void Release();
	void Pop();
};