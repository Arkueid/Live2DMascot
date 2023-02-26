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
#include <vector>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qtreewidget.h>
#include "json/json.h"

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
	void Pop(QWidget* p, const char* text);
	static Tip* GetInstance();
	static void Release();
protected:
	void paintEvent(QPaintEvent* e);
	const char* text = NULL;
	QPropertyAnimation* anime;
};


class AppSettings : public QWidget
{
	Q_OBJECT
	QLineEdit* appName;
	QLineEdit* username;
	QLineEdit* iconPath;
	QLineEdit* fps;
	QLineEdit* windowHeight;
	QLineEdit* windowWidth;
	QLineEdit* modelDir;
	QLineEdit* motionInterval;
	QLineEdit* lipSync;
	QLabel* lbl_dialogMaxWidth;
	QLineEdit* dialogMaxWidth;
	QLabel* lbl_dialogFontSize;
	QLineEdit* dialogFontSize;
	QLabel* lbl_dialogYOffset;
	QLineEdit* dialogYOffset;
	
	QLabel* lbl_motioninterval;
	QLabel* lbl_lipsync;
	QLabel* lbl_appName;
	QLabel* lbl_username;
	QLabel* lbl_iconPath;
	QLabel* lbl_FPS;
	QLabel* lbl_windowWidth;
	QLabel* lbl_windowHeight;
	QLabel* lbl_modelDir;
	QPushButton* openFile;
	QPushButton* chooseDir;
	QPushButton* apply;
	QPushButton* reset;
	QGridLayout* grid;
	QWidget* _parent;
public:
	AppSettings(QWidget* p);
	void LoadConfig();
private slots:
	void OpenFile();
	void Apply();
	void Reset();
	void OpenSourceDir();
};

class ModelSettings : public QWidget
{
	Q_OBJECT
	QWidget* _parent;
	QComboBox* model;
	QLabel* lbl_model;
	QTreeWidget* _motionGroups;
	QPushButton* apply;
	QPushButton* reset;
	QGridLayout* grid;
	Json::Value _modelJson;
	QLabel* lbl_motionJsonPath;
	QLabel* lbl_motionSoundPath;
	QLabel* lbl_motionText;
	QLabel* lbl_motionGroup;
	QLineEdit* motionGroup;
	QComboBox* motionJsonPath;
	QComboBox* motionSoundPath;
	MyText* motionText;
	QPushButton* addMotion;
	QPushButton* deleteMotion;
	QPushButton* changeModel;
	QPushButton* addGroup;
	QPushButton* deleteGroup;
	QPushButton* updateGroupName;
public:
	ModelSettings(QWidget* p);
	void LoadConfig();
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
	QLineEdit* savePath;
	QLineEdit* apiKey;
	QLineEdit* apiSecret;
	QLineEdit* hostPort;
	QLineEdit* readTimeOut;
	QLineEdit* route;
	QLabel* lbl_savePath;
	QLabel* lbl_apiKey;
	QLabel* lbl_apiSecret;
	QLabel* lbl_hostPort;
	QLabel* lbl_readTimeOut;
	QLabel* lbl_route;
	QCheckBox* MlyAI;
	QCheckBox* CustomChatServer;
	QPushButton* chooseDir;
	QPushButton* apply;
	QPushButton* reset;
	QGridLayout* grid;
	QWidget* _parent;
public:
	ChatSettings(QWidget* p);
	void LoadConfig();
	void UpdateState();
private slots:
	void Reset();
	void Apply();
	void ChooseDir();
	void MlyAIChecked();
	void CustomChecked();
};

class ControlWidget : public QTabWidget
{
private:
	AppSettings* _appSettings;
	ModelSettings* _modelSettings;
	ChatSettings* _chatSettings;
public:
	friend class AppSettings;
	~ControlWidget();
	ControlWidget();
	void Pop();
};