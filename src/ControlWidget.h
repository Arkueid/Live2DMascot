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

#include <qtmaterialtextfield.h>
#include <qtmaterialslider.h>
#include <qtmaterialtoggle.h>
#include <qtmaterialraisedbutton.h>

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


class AppSettings : public QWidget
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
	//QLabel* lbl_dialogMaxWidth;
	QtMaterialTextField* dialogMaxWidth;
	//QLabel* lbl_dialogFontSize;
	QtMaterialTextField* dialogFontSize;
	//QLabel* lbl_dialogYOffset;
	QtMaterialTextField* dialogYOffset;
	QtMaterialSlider* volumeSlider;
	//QLabel* lbl_motioninterval;
	//QLabel* lbl_lipsync;
	//QLabel* lbl_appName;
	//QLabel* lbl_username;
	//QLabel* lbl_iconPath;
	//QLabel* lbl_FPS;
	//QLabel* lbl_windowWidth;
	//QLabel* lbl_windowHeight;
	//QLabel* lbl_modelDir;
	QLabel* lbl_volume;
	QLabel* lbl_sliderVal;
	QLabel* lbl_repairMode;
	QtMaterialRaisedButton* openFile;
	QtMaterialRaisedButton* chooseDir;
	QtMaterialRaisedButton* apply;
	QtMaterialRaisedButton* reset;
	//QGridLayout* grid;
	QWidget* _parent;
	QtMaterialToggle* repairModeControl;
	//QLabel* lbl_characterX;
	QtMaterialTextField* characterX;
	//QLabel* lbl_characterY;
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
	void SetRepairMode();
};

class ModelSettings : public QWidget
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

class ChatSettings : public QWidget
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

class ControlWidget : public QTabWidget
{
private:
	AppSettings* _appSettings;
	ModelSettings* _modelSettings;
	ChatSettings* _chatSettings;
public:
	friend class AppSettings;
	ControlWidget();
	~ControlWidget() { if (LAppDefine::DebugLogEnable) printf("[APP][WIN]ControlWidget destroyed\n"); }
	void Release();
	void Pop();
};