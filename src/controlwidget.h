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


class AppSettings : public QWidget
{
	Q_OBJECT
	QLineEdit* appName;
	QLineEdit* username;
	QLineEdit* iconPath;
	QLineEdit* fps;
	QLineEdit* appKey;
	QLineEdit* appSecret;
	QLineEdit* windowHeight;
	QLineEdit* windowWidth;
	QLineEdit* modelDir;
	QLabel* lbl_appName;
	QLabel* lbl_username;
	QLabel* lbl_iconPath;
	QLabel* lbl_FPS;
	QLabel* lbl_appKey;
	QLabel* lbl_appSecret;
	QLabel* lbl_windowWidth;
	QLabel* lbl_windowHeight;
	QLabel* lbl_modelDir;
	QPushButton* openFile;
	QPushButton* chooseDir;
	QPushButton* apply;
	QPushButton* cancel;
	QGridLayout* grid;
	QWidget* _parent;
public:
	AppSettings(QWidget* p);
	void LoadConfig();
private slots:
	void OpenFile();
	void Apply();
	void Cancel();
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
	QPushButton* cancel;
	QGridLayout* grid;
	Json::Value _modelJson;
	QLabel* lbl_motionJsonPath;
	QLabel* lbl_motionSoundPath;
	QLabel* lbl_motionText;
	QComboBox* motionJsonPath;
	QComboBox* motionSoundPath;
	MyText* motionText;
	QPushButton* addMotion;
	QPushButton* deleteMotion;
	QPushButton* changeModel;
public:
	ModelSettings(QWidget* p);
	void LoadConfig();
private slots:
	void Cancel();
	void Apply();
	void AddMotion();
	void DeleteMotion();
	void ShowMotionInfo(QTreeWidgetItem* w, int);
	void StartMotion(QTreeWidgetItem* w, int);
	void BindSound(const QString& x);
	void BindMotion(const QString& x);
	void BindText();
	void UpdateModel();
};

class ControlWidget : public QTabWidget
{
private:
	AppSettings* _appSettings;
	ModelSettings* _modelSettings;
public:
	friend class AppSettings;
	ControlWidget();
	void Pop();
};