#include "LAppLive2DManager.hpp"
#include "LAppModel.hpp"
#include "ControlWidget.h"
#include "LApp.h"
#include <iostream>
#include <fstream>
#include <QtCore/qfileinfo.h>
#include <QtCore/qdir.h>
#include <QtWidgets/qmessagebox.h>
#include <qtmaterialscrollbar.h>

using namespace std;
using namespace LAppConfig;

ModelSettings::ModelSettings(QWidget* p)
{
	_parent = p;
	model = new QComboBox();
	lbl_model = new QLabel(QString("选择模型"));
	_motionGroups = new QTreeWidget();

	_motionGroups->setHeaderLabel(QString("动作组"));
	apply = new QtMaterialRaisedButton(QString("保存"));
	apply->setOverlayColor(Qt::white);
	apply->setForegroundColor(QColor(244, 244, 244));
	apply->setBackgroundColor(QColor(50, 120, 200));
	apply->setFixedHeight(30);

	reset = new QtMaterialRaisedButton(QString("重置"));
	reset->setOverlayColor(Qt::white);
	reset->setForegroundColor(QColor(244, 244, 244));
	reset->setBackgroundColor(QColor(50, 120, 200));
	reset->setFixedHeight(30);

	motionJsonPath = new QComboBox();
	motionSoundPath = new QComboBox();
	motionText = new MyText();

	lbl_motionJsonPath = new QLabel(QString("动作路径"));
	lbl_motionJsonPath->setAlignment(Qt::AlignRight | Qt::AlignCenter);
	lbl_motionSoundPath = new QLabel(QString("语音"));
	lbl_motionSoundPath->setAlignment(Qt::AlignRight | Qt::AlignCenter);
	lbl_motionText = new QLabel(QString("文本"));
	lbl_motionText->setAlignment(Qt::AlignTop | Qt::AlignRight);
	motionGroup = new QtMaterialTextField();
	motionGroup->setLabelFontSize(8);
	motionGroup->setInkColor(QColor(50, 120, 200));
	motionGroup->setInputLineColor(QColor(80, 80, 80));

	addGroup = new QtMaterialRaisedButton();
	addGroup->setText("新建动作组");
	addGroup->setOverlayColor(Qt::white);
	addGroup->setForegroundColor(QColor(244, 244, 244));
	addGroup->setBackgroundColor(QColor(50, 120, 200));
	addGroup->setFixedHeight(25);

	deleteGroup = new QtMaterialRaisedButton();
	deleteGroup->setText("删除动作组");
	deleteGroup->setOverlayColor(Qt::white);
	deleteGroup->setForegroundColor(QColor(244, 244, 244));
	deleteGroup->setBackgroundColor(QColor(50, 120, 200));
	deleteGroup->setFixedHeight(25);

	lbl_motionGroup = new QLabel(QString("当前动作组"));

	addMotion = new QtMaterialRaisedButton();
	addMotion->setText("添加动作");
	addMotion->setOverlayColor(Qt::white);
	addMotion->setForegroundColor(QColor(244, 244, 244));
	addMotion->setBackgroundColor(QColor(50, 120, 200));
	addMotion->setFixedHeight(25);
	addMotion->setFixedWidth(80);

	deleteMotion = new QtMaterialRaisedButton();
	deleteMotion->setText("删除动作");
	deleteMotion->setOverlayColor(Qt::white);
	deleteMotion->setForegroundColor(QColor(244, 244, 244));
	deleteMotion->setBackgroundColor(QColor(50, 120, 200));
	deleteMotion->setFixedHeight(25);
	deleteMotion->setFixedWidth(80);

	changeModel = new QtMaterialRaisedButton();
	changeModel->setText("切换");
	changeModel->setOverlayColor(Qt::white);
	changeModel->setForegroundColor(QColor(244, 244, 244));
	changeModel->setBackgroundColor(QColor(50, 120, 200));
	changeModel->setFixedHeight(25);

	updateGroupName = new QtMaterialRaisedButton();
	updateGroupName->setText("修改动作组名称");
	updateGroupName->setOverlayColor(Qt::white);
	updateGroupName->setForegroundColor(QColor(244, 244, 244));
	updateGroupName->setBackgroundColor(QColor(50, 120, 200));
	updateGroupName->setFixedHeight(25);

	connect(apply, SIGNAL(clicked()), SLOT(Apply()));
	connect(reset, SIGNAL(clicked()), SLOT(Reset()));

	grid = new QGridLayout();
	grid->addWidget(lbl_model, 0, 0, 0);
	grid->addWidget(model, 0, 1, 0);
	grid->addWidget(changeModel, 0, 2, 0);
	grid->addWidget(lbl_motionGroup, 1, 0, 0);
	grid->addWidget(motionGroup, 1, 1, 0);
	grid->addWidget(addGroup, 1, 2, 0);
	grid->addWidget(deleteGroup, 1, 3, 0);
	grid->addWidget(updateGroupName, 1, 4, 0);
	grid->addWidget(_motionGroups, 2, 0, 3, 2, 0);
	grid->addWidget(motionJsonPath, 2, 3, 1, 2, 0);
	grid->addWidget(motionSoundPath, 3, 3, 1, 2, 0);
	grid->addWidget(motionText, 4, 3, 1, 2, 0);
	grid->addWidget(lbl_motionJsonPath, 2, 2, 0);
	grid->addWidget(lbl_motionSoundPath, 3, 2, 0);
	grid->addWidget(lbl_motionText, 4, 2, 0);
	grid->addWidget(addMotion, 5, 0, 0);
	grid->addWidget(deleteMotion, 5, 1, 0);
	grid->addWidget(apply, 5, 3, 0);
	grid->addWidget(reset, 5, 4, 0);
	setLayout(grid);

	connect(_motionGroups, SIGNAL(itemClicked(QTreeWidgetItem*, int)), SLOT(ShowMotionInfo(QTreeWidgetItem*, int)));
	connect(_motionGroups, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(StartMotion(QTreeWidgetItem*, int)));
	connect(addMotion, SIGNAL(clicked()), SLOT(AddMotion()));
	connect(deleteMotion, SIGNAL(clicked()), SLOT(DeleteMotion()));
	connect(motionJsonPath, SIGNAL(currentTextChanged(const QString&)), SLOT(BindMotion(const QString&)));
	connect(motionSoundPath, SIGNAL(currentTextChanged(const QString&)), SLOT(BindSound(const QString&)));
	connect(motionText, SIGNAL(loseFocus()), SLOT(BindText()));

	connect(addGroup, SIGNAL(clicked()), SLOT(AddGroup()));
	connect(deleteGroup, SIGNAL(clicked()), SLOT(DeleteGroup()));
	connect(updateGroupName, SIGNAL(clicked()), SLOT(UpdateGroupName()));

	connect(changeModel, SIGNAL(clicked()), SLOT(UpdateModel()));
	_motionGroups->setObjectName("motionGroups");
	_motionGroups->setStyleSheet(
		"QTreeWidget::branch::open:hover:has-children{border-image: url(assets/open.png);}"
		"QTreeWidget::branch::open:has-children{border-image: url(assets/open-default.png);}"
		"QTreeWidget::branch::closed:has-children {border-image: url(assets/arrow-right-default.png)}"
		"QTreeWidget::branch::closed:hover:has-children{border-image: url(assets/arrow-right.png)}"
		"QHeaderView::section{background-color: rgb(50, 50, 50); color: rgba(255, 255, 255, 210); height: 30px; font-family: PingFang SC Medium; border: none; padding-left: 10px;}"
		"QTreeWidget::item:selected {font-weight: 800; border: none }"
	);

	QtMaterialScrollBar* bar = new QtMaterialScrollBar();
	bar->setFixedWidth(10);
	bar->setSliderColor(QColor(80, 150, 244));
	_motionGroups->setVerticalScrollBar(bar);

	const char* styleSheet = ("QComboBox QAbstractItemView {outline: none; background: rgb(80, 80, 80); border: 2px solid rgb(80, 80, 80); }"
		"QComboBox QAbstractItemView::item {height: 25px; color: rgba(255, 255, 255, 210);}"
		"QComboBox QAbstractItemView::item:hover,QComboBox QAbstractItemView::item:focus {border: none; background-color: rgba(80, 150, 244, 200)}"
		"QComboBox{selection-background-color: rgb(50, 50, 50); padding-left: 5px}"
		"QComboBox::drop-down{ border-image: url(assets/open-default.png); width: 30px; height: 30px }"
		"QComboBox::drop-down:hover{ border-image: url(assets/open.png); width: 30px; height: 30px }"
		);

	model->setView(new QListView());
	motionJsonPath->setView(new QListView());
	motionSoundPath->setView(new QListView());

	model->setStyleSheet(styleSheet);
	bar = new QtMaterialScrollBar();
	bar->setFixedWidth(10);
	bar->setSliderColor(QColor(80, 150, 244));
	model->view()->setVerticalScrollBar(bar);

	motionJsonPath->setStyleSheet(styleSheet);
	bar = new QtMaterialScrollBar();
	bar->setFixedWidth(10);
	bar->setSliderColor(QColor(80, 150, 244));
	motionJsonPath->view()->setVerticalScrollBar(bar);

	motionSoundPath->setStyleSheet(styleSheet);
	bar = new QtMaterialScrollBar();
	bar->setFixedWidth(10);
	bar->setSliderColor(QColor(80, 150, 244));
	motionSoundPath->view()->setVerticalScrollBar(bar);

	motionText->setStyleSheet(
		"QTextEdit {color: rgba(255, 255, 255, 180)}"

	);
	bar = new QtMaterialScrollBar();
	bar->setFixedWidth(10);
	bar->setSliderColor(QColor(80, 150, 244));
	motionText->setVerticalScrollBar(bar);
}

void ModelSettings::Release()
{

}

void ModelSettings::Apply()
{
	if (QMessageBox::question(_parent, QString("保存配置"), QString("是否保存配置?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		// 中文支持
		const char* path = QString::fromUtf8(string(_ModelDir).append("/").append(_ModelName).append("/").append(_ModelName).append(".model3.json").c_str()).toLocal8Bit().constData();

		Log("Save ModelJson", path);
		ofstream ofs(path);
		ofs << _modelJson;
		ofs.close();
		LoadConfig();
		LAppLive2DManager::GetInstance()->ChangeModel(_ModelDir.c_str(), _ModelName.c_str());
		Tip::GetInstance()->Pop(_parent, "操作成功!");
	}

}

void ModelSettings::Reset()
{
	LoadConfig();
}

void ModelSettings::LoadConfig()
{
	_motionGroups->clear();
	motionJsonPath->clear();
	motionSoundPath->clear();
	motionText->clear();
	motionGroup->clear();
	int i;
	int size;
	string path;
	QFileInfoList _files;
	QDir dir;
	bool flag = false;
	Log("Load ModelDir", _ModelDir.c_str());
	dir.setPath(QString::fromStdString(_ModelDir).toLocal8Bit().constData());
	_files = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	size = _files.size();
	model->clear();
	for (i = 0; i < size; i++)
	{
		if (strcmp(_ModelName.c_str(), _files[i].fileName().toStdString().c_str()) == 0) flag = true;
		model->addItem(_files[i].fileName());
	}
	_ModelName = flag ? _ModelName : model->itemText(0).toUtf8().constData();
	model->setCurrentText(_ModelName.c_str());
	motionJsonPath->clear();
	_files.clear();

	path = QString::fromStdString(_ModelDir).append("/").append(_ModelName.c_str()).append("/").append("motions").toLocal8Bit().constData();
	Log("Load MotionJsonDir", path.c_str());

	dir.setPath(path.c_str());
	_files = dir.entryInfoList(QDir::Files);
	size = _files.size();
	for (i = 0; i < size; i++)
	{
		motionJsonPath->addItem(QString("motions/").append(_files[i].fileName()));
	}
	motionSoundPath->clear();
	motionJsonPath->addItem("");
	motionJsonPath->setCurrentText("");
	_files.clear();

	path = QString::fromStdString(_ModelDir).append("/").append(_ModelName.c_str()).append("/").append("sounds").toLocal8Bit().constData();
	Log("Load motionSoundDir", path.c_str());
	dir.setPath(path.c_str());
	_files = dir.entryInfoList(QDir::Files);
	size = _files.size();
	for (i = 0; i < size; i++)
	{
		motionSoundPath->addItem(QString("sounds/").append(_files[i].fileName()));
	}
	motionSoundPath->addItem("");
	motionSoundPath->setCurrentText("");


	path = QString::fromUtf8(string(_ModelDir).append("/").append(_ModelName).append("/").append(_ModelName).append(".model3.json").c_str()).toLocal8Bit().constData();
	Log("Load ModelJson", path.c_str());

	ifstream ifs(path);
	ifs >> _modelJson;
	ifs.close();
	Json::Value motions = _modelJson["FileReferences"]["Motions"];
	Json::Value::Members groupnames = motions.getMemberNames();
	size = groupnames.size();
	QTreeWidgetItem* item;
	QTreeWidgetItem* subItem;
	int j;
	int size2;
	for (i = 0; i < size; i++)
	{
		item = new QTreeWidgetItem();
		_motionGroups->addTopLevelItem(item);
		Json::Value motionGroups = motions[groupnames[i]];
		item->setText(0, QString::fromUtf8(groupnames[i].c_str()));
		size2 = motionGroups.size();

		for (j = 0; j < size2; j++)
		{
			subItem = new QTreeWidgetItem();
			subItem->setText(0, QString::fromUtf8(groupnames[i].c_str()).append("_").append(to_string(j).c_str()));
			subItem->setData(1, 0, j);
			item->addChild(subItem);
		}
	}

}

void ModelSettings::BindMotion(const QString& x)
{
	QTreeWidgetItem* cur = _motionGroups->currentItem();
	if (cur != NULL && cur->parent() != NULL)
	{
		string path = QString::fromUtf8(string(_ModelDir).append("/").append(_ModelName).append("/").append(motionJsonPath->currentText().toStdString()).c_str()).toStdString();
		QDir dir(path.c_str());
		if (dir.exists())
		{
			_modelJson["FileReferences"]["Motions"][cur->parent()->text(0).toStdString()][cur->parent()->indexOfChild(cur)]["File"] = motionJsonPath->currentText().toStdString();
			if (!motionJsonPath->currentText().isEmpty())
			{
				Tip::GetInstance()->Pop(_parent, "动作已更换!");
			}
			else {
				Tip::GetInstance()->Pop(_parent, "动作已清空!");
			}
		}
		else Tip::GetInstance()->Pop(_parent, "动作文件不存在!");
	}
}

void ModelSettings::BindSound(const QString& x)
{
	QTreeWidgetItem* cur = _motionGroups->currentItem();
	if (cur != NULL && cur->parent() != NULL)
	{
		const char* path = QString::fromUtf8(string(_ModelDir).append("/").append(_ModelName).append("/").append(motionSoundPath->currentText().toStdString().c_str()).c_str()).toStdString().c_str();
		QDir dir(path);
		if (dir.exists())
		{
			_modelJson["FileReferences"]["Motions"][cur->parent()->text(0).toStdString()][cur->parent()->indexOfChild(cur)]["Sound"] = motionSoundPath->currentText().toStdString();
			if (!motionSoundPath->currentText().isEmpty())
			{
				Tip::GetInstance()->Pop(_parent, "语音已更换!");
			}
			else {
				Tip::GetInstance()->Pop(_parent, "语音已清空!");
			}
		}
		else {
			Tip::GetInstance()->Pop(_parent, "音频文件不存在!");
		}
	}

}

void ModelSettings::BindText()
{
	QTreeWidgetItem* cur = _motionGroups->currentItem();
	if (cur != NULL && cur->parent() != NULL)
	{
		_modelJson["FileReferences"]["Motions"][cur->parent()->text(0).toStdString()][cur->parent()->indexOfChild(cur)]["Text"] = motionText->toPlainText().toStdString();
		if (!motionText->toPlainText().isEmpty())
		{
			Tip::GetInstance()->Pop(_parent, "文本已修改!");
		}
		else {
			Tip::GetInstance()->Pop(_parent, "文本已清空!");
		}
	}
}

void ModelSettings::AddMotion()
{
	if (_motionGroups->currentItem() == NULL)  //无选中
	{
		Tip::GetInstance()->Pop(_parent, "未选择动作组!");
	}
	else if (_motionGroups->currentItem()->parent() == NULL)  //选中整个动作组
	{
		Json::Value motion;
		motion["File"] = "";
		motion["Sound"] = "";
		motion["Text"] = "";
		_modelJson["FileReferences"]["Motions"][_motionGroups->currentItem()->text(0).toStdString()].append(motion);
		int idx = _motionGroups->currentItem()->childCount();
		QTreeWidgetItem* subItem = new QTreeWidgetItem();
		subItem->setText(0, QString(_motionGroups->currentItem()->text(0)).append("_").append(to_string(idx).c_str()));
		subItem->setData(1, 0, idx);
		_motionGroups->currentItem()->addChild(subItem);
		Tip::GetInstance()->Pop(_parent, "动作已添加!");
	}
	else  //选中子动作 
	{
		QTreeWidgetItem* p = _motionGroups->currentItem()->parent();

		Json::Value motion;
		motion["File"] = "";
		motion["Sound"] = "";
		motion["Text"] = "";
		_modelJson["FileReferences"]["Motions"][p->text(0).toStdString()].append(motion);

		int idx = p->childCount();
		QTreeWidgetItem* subItem = new QTreeWidgetItem();
		subItem->setText(0, QString(p->text(0)).append("_").append(to_string(idx).c_str()));
		subItem->setData(1, 0, idx);
		p->addChild(subItem);
		Tip::GetInstance()->Pop(_parent, "动作已添加!");
	}
}

void ModelSettings::DeleteMotion()
{
	if (_motionGroups->currentItem() == NULL)  //无选中
	{
		Tip::GetInstance()->Pop(_parent, "未选择动作!");
	}
	else if (_motionGroups->currentItem()->parent() == NULL)  //选中整个动作组
	{
		Tip::GetInstance()->Pop(_parent, "未选择动作!");
	}
	else  //选中子动作 
	{
		QTreeWidgetItem* p = _motionGroups->currentItem()->parent();
		_modelJson["FileReferences"]["Motions"][p->text(0).toStdString()].removeIndex(p->indexOfChild(_motionGroups->currentItem()), NULL);
		p->removeChild(_motionGroups->currentItem());
		int i;
		int size = p->childCount();
		for (i = 0; i < size; i++)
		{
			p->child(i)->setData(1, 0, i);
			p->child(i)->setText(0, QString(p->text(0)).append("_").append(to_string(i).c_str()));
		}
		motionJsonPath->setCurrentText("");
		motionSoundPath->setCurrentText("");
		motionText->clear();
		Tip::GetInstance()->Pop(_parent, "动作已删除!");
	}
}

void ModelSettings::ShowMotionInfo(QTreeWidgetItem* w, int idx)
{
	if (w->parent() != NULL)
	{
		motionGroup->setText(w->parent()->text(0));
		string groupname = w->parent()->text(0).toUtf8();
		int idx = w->data(1, 0).toInt();
		Json::Value motion = _modelJson["FileReferences"]["Motions"][groupname][idx];

		motionJsonPath->setCurrentText(QString::fromUtf8(motion["File"].isNull() ? "" : motion["File"].asCString()));
		motionSoundPath->setCurrentText(QString::fromUtf8(motion["Sound"].isNull() ? "" : motion["Sound"].asCString()));
		motionText->setText(QString::fromStdString(motion["Text"].isNull() ? "" : motion["Text"].asCString()));
	}
	else {
		if (w->parent() == NULL) motionGroup->setText(w->text(0));
		motionJsonPath->setCurrentText("");
		motionSoundPath->setCurrentText("");
		motionText->clear();
	}
}

void ModelSettings::AddGroup()
{
	if (motionGroup->text().isEmpty()) {
		Tip::GetInstance()->Pop(_parent, "动作组名称不能为空!");
		return;
	}
	QList<QTreeWidgetItem*> x = _motionGroups->findItems(motionGroup->text(), Qt::MatchExactly, 0);
	if (x.count() == 0)
	{
		QTreeWidgetItem* topItem = new QTreeWidgetItem();
		topItem->setText(0, motionGroup->text());
		_motionGroups->addTopLevelItem(topItem);
		_modelJson["FileReferences"]["Motions"][motionGroup->text().toUtf8().toStdString()] = Json::Value(Json::arrayValue);
		Tip::GetInstance()->Pop(_parent, "动作组添加成功!");
	}
	else {
		Tip::GetInstance()->Pop(_parent, "动作组已存在!");
	}
}

void ModelSettings::DeleteGroup()
{
	if (motionGroup->text().isEmpty()) {
		Tip::GetInstance()->Pop(_parent, "未选中动作组!");
		return;
	}
	QList<QTreeWidgetItem*> x = _motionGroups->findItems(motionGroup->text(), Qt::MatchExactly, 0);
	if (x.count() == 1)
	{
		if (QMessageBox::question(_parent, QString("模型设置"), QString("是否删除动作组<br>").append("<b>").append(motionGroup->text()).append("</b>"),
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			_modelJson["FileReferences"]["Motions"].removeMember(motionGroup->text().toUtf8().toStdString());
			x.at(0)->takeChildren();
			_motionGroups->takeTopLevelItem(_motionGroups->indexOfTopLevelItem(x.at(0)));
			Tip::GetInstance()->Pop(_parent, "动作组已删除!");
			_motionGroups->clearFocus();
			motionGroup->clear();
		}
	}
	else if (x.count() == 0)
	{
		Tip::GetInstance()->Pop(_parent, "动作组不存在!");
	}
	else {
		Tip::GetInstance()->Pop(_parent, "出现未知错误!");
	}
}

void ModelSettings::UpdateGroupName()
{
	if (motionGroup->text().isEmpty())
	{
		Tip::GetInstance()->Pop(_parent, "动作组名称不能为空!");
	}
	else if (_motionGroups->currentItem() == NULL)  //无选中
	{
		Tip::GetInstance()->Pop(_parent, "未选中动作组!");
	}
	else if (_motionGroups->findItems(motionGroup->text(), Qt::MatchExactly, 0).count() > 0)
	{
		Tip::GetInstance()->Pop(_parent, "动作组名称已存在!");
	}
	else if (_motionGroups->currentItem()->parent() == NULL)  //选中整个动作组
	{
		Json::Value group = _modelJson["FileReferences"]["Motions"][_motionGroups->currentItem()->text(0).toStdString()];
		_modelJson["FileReferences"]["Motions"].removeMember(_motionGroups->currentItem()->text(0).toStdString());
		_modelJson["FileReferences"]["Motions"][motionGroup->text().toStdString()] = group;
		_motionGroups->currentItem()->setText(0, motionGroup->text());
		Tip::GetInstance()->Pop(_parent, "动作组名称已修改!");
	}
	else  //选中子动作 
	{
		QTreeWidgetItem* p = _motionGroups->currentItem()->parent();
		Json::Value group = _modelJson["FileReferences"]["Motions"][p->text(0).toStdString()];
		_modelJson["FileReferences"]["Motions"].removeMember(p->text(0).toStdString());
		_modelJson["FileReferences"]["Motions"][motionGroup->text().toStdString()] = group;
		p->setText(0, motionGroup->text());
		Tip::GetInstance()->Pop(_parent, "动作组名称已修改!");
	}
}

void ModelSettings::StartMotion(QTreeWidgetItem* w, int idx)
{
	if (w->parent() != NULL)
	{
		string groupname = w->parent()->text(0).toUtf8();
		int idx = w->data(1, 0).toInt();
		LAppLive2DManager::GetInstance()->GetModel(0)->StartMotion(groupname.c_str(), idx, PriorityForce);

	}
	else {
		motionJsonPath->setCurrentText("");
		motionSoundPath->setCurrentText("");
		motionText->clear();
	}
}

void ModelSettings::UpdateModel()
{
	_ModelName = model->currentText().toUtf8();
	LAppLive2DManager::GetInstance()->ChangeModel(_ModelDir.c_str(), _ModelName.c_str());
	LApp::GetInstance()->SaveConfig();
	LoadConfig();
	Tip::GetInstance()->Pop(_parent, "模型已更换");
}