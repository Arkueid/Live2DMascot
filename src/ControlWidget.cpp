/*设置窗口
*/

#include "LAppModel.hpp"
#include "LAppLive2DManager.hpp"
#include "LApp.h"
#include "ControlWidget.h"
#include <fstream>
#include <vector>
#include <io.h>
#include <QtWidgets/qtabbar.h>
#include <QtCore/qregexp.h>
#include <QtGui/qpainter.h>
#include <QtGui/qvalidator.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qtreewidget.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qlistview.h>
#include <QtGui/qfontdatabase.h>

#include <qtmaterialslider.h>
#include <qtmaterialdialog.h>

using namespace LAppConfig;


namespace {
	Tip* _tip = NULL;
}

Tip::Tip()
{
	resize(100, 40); //预设值，不设置第一次显示时不居中
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
	anime = new QPropertyAnimation(this, "windowOpacity");
	anime->setStartValue(1);
	anime->setEndValue(0);
	anime->setDuration(2000);
}
Tip::~Tip()
{
	if (LAppDefine::DebugLogEnable)
	printf("Tip destroyed\n");
}

void Tip::ReleaseInstance()
{
	if (_tip)
	{
		_tip->Release();
		_tip->deleteLater();
	}
}

void Tip::Release()
{
	if (this->anime)
		this->anime->deleteLater();
	close();
}
void Tip::paintEvent(QPaintEvent* e)
{
	QFontMetrics _fontMetrics(QFont(""));
	int boxWidth = _fontMetrics.width(QString(text));
	resize(boxWidth+10, 40);
	QPainter painter(this);
	painter.fillRect(0, 0, boxWidth+10, 40, QColor(0, 0, 0, 180));
	painter.setPen(Qt::white);
	painter.drawText(10, 25, QString(text).toStdString().c_str());
}
void Tip::Pop(QWidget* p, const char* text)
{
	close();
	this->text = text;
	anime->stop();
	move(p->x() + p->width() / 2 - width()/2, p->y() + 20);
	show();
	anime->start();
}
Tip* Tip::GetInstance()
{
	if (_tip == NULL) _tip = new Tip();
	return _tip;
}


void getDirNames(string path, vector<string>& files)
{
	//文件句柄
#ifdef WIN64_FLAG
	__int64		hFile = 0;
#else
	long		hFile = 0;
#endif // WIN64_FLAG

	//文件信息
#ifdef WIN64_FLAG
	struct		__finddata64_t fileinfo;
#else
	struct		_finddata_t fileinfo;
#endif // WIN64_FLAG

	string p;
	if (
#ifdef WIN64_FLAG
	(hFile	=	_findfirst64(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1L
#else
	(hFile	=	_findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1
#endif // WIN64_FLAG
		)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				string x = fileinfo.name;
				if (strcmp(x.c_str(), ".") != 0 && strcmp(x.c_str(), "..") != 0)
				{
					files.push_back(string(fileinfo.name));
				}
			}
		} while (
#ifdef WIN64_FLAG
			_findnext64(hFile, &fileinfo) == 0
#else
			_findnext(hFile, &fileinfo) == 0
#endif // WIN64_FLAG

			);
		_findclose(hFile);
	}
}




void getFileNames(string path, vector<string>& files)
{
	//文件句柄
#ifdef WIN64_FLAG
	__int64		hFile = 0;
	struct		__finddata64_t fileinfo;
#else
	long		hFile = 0;
	struct		_finddata_t fileinfo;
#endif // WIN64_FLAG

	//文件信息
	string p;
	if (
#ifdef WIN64_FLAG
	(hFile	=	_findfirst64(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1L
#else
	(hFile	=	_findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1
#endif // WIN64_FLAG
		)
	{
		do
		{
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				files.push_back(string(fileinfo.name));
			}
		} while (
#ifdef WIN64_FLAG
			_findnext64(hFile, &fileinfo) == 0
#else
			_findnext(hFile, &fileinfo) == 0
#endif // WIN64_FLAG
			);
		_findclose(hFile);
	}
}


AppSettings::AppSettings(QWidget* p)
{
	_parent = p;

	appName = new QtMaterialTextField();
	appName->setLabel("应用名称");
	appName->setLabelFontSize(8);
	appName->setInkColor(QColor(50, 120, 200));
	appName->setInputLineColor(QColor(80, 80, 80));

	username = new QtMaterialTextField();
	username->setLabel("用户名称");
	username->setLabelFontSize(8);
	username->setInkColor(QColor(50, 120, 200));
	username->setInputLineColor(QColor(80, 80, 80));

	iconPath = new QtMaterialTextField();
	iconPath->setLabel("图标路径");
	iconPath->setLabelFontSize(8);
	iconPath->setInkColor(QColor(50, 120, 200));
	iconPath->setInputLineColor(QColor(80, 80, 80));

	fps = new QtMaterialTextField();
	fps->setLabel("FPS");
	fps->setLabelFontSize(8);
	fps->setMaximumWidth(50);
	fps->setInkColor(QColor(50, 120, 200));
	fps->setInputLineColor(QColor(80, 80, 80));


	windowWidth = new QtMaterialTextField();
	windowWidth->setLabel("桌宠宽度");
	windowWidth->setLabelFontSize(8);
	windowWidth->setInkColor(QColor(50, 120, 200));
	windowWidth->setInputLineColor(QColor(80, 80, 80));


	windowHeight = new QtMaterialTextField();
	windowHeight->setLabel("桌宠高度");
	windowHeight->setLabelFontSize(8);
	windowHeight->setInkColor(QColor(50, 120, 200));
	windowHeight->setInputLineColor(QColor(80, 80, 80));


	modelDir = new QtMaterialTextField();
	modelDir->setLabel("模型路径");
	modelDir->setLabelFontSize(8);
	modelDir->setInkColor(QColor(50, 120, 200));
	modelDir->setInputLineColor(QColor(80, 80, 80));

	lbl_volume = new QLabel(QString("音量"));
	lbl_volume->setStyleSheet("color: rgb(191, 191, 191)");
	lbl_sliderVal = new QLabel();
	lbl_sliderVal->setStyleSheet("color: rgb(191, 191, 191)");
	volumeSlider = new QtMaterialSlider();
	volumeSlider->setMaximum(100);
	volumeSlider->setMinimum(0);
	volumeSlider->setOrientation(Qt::Horizontal);
	volumeSlider->setPageStep(1);
	volumeSlider->setFixedWidth(200);
	volumeSlider->setThumbColor(QColor(50, 120, 200));
	lbl_repairMode = new QLabel(QString("动作修复"));
	lbl_repairMode->setStyleSheet("color: rgb(191, 191, 191)");
	repairModeControl = new QtMaterialToggle();
	repairModeControl->setCheckable(true);
	repairModeControl->setActiveColor(QColor(50, 120, 200));

	lipSync = new QtMaterialTextField();
	
	QRegExpValidator* v3 = new QRegExpValidator(QRegExp("^[0-9]{1,2}\\.[0-9]{1}$"));
	lipSync->setValidator(v3);
	lipSync->setLabel("口型同步");
	lipSync->setLabelFontSize(8);
	lipSync->setInkColor(QColor(50, 120, 200));
	lipSync->setInputLineColor(QColor(80, 80, 80));


	characterX = new QtMaterialTextField();
	characterX->setLabel("角色绘制 X 坐标");
	characterX->setLabelFontSize(8);
	characterX->setInkColor(QColor(50, 120, 200));
	characterX->setInputLineColor(QColor(80, 80, 80));


	characterY = new QtMaterialTextField();
	characterY->setLabel("角色绘制 Y 坐标");
	characterY->setLabelFontSize(8);
	characterY->setInkColor(QColor(50, 120, 200));
	characterY->setInputLineColor(QColor(80, 80, 80));


	const QValidator* v5 = new QRegExpValidator(QRegExp("^[\\-]?[0-9]{1,2}\\.[0-9]{1,2}$"));
	characterX->setValidator(v5);
	characterY->setValidator(v5);

	motionInterval = new QtMaterialTextField();
	motionInterval->setValidator(v3);
	motionInterval->setLabel("动作频率");
	motionInterval->setLabelFontSize(8);
	motionInterval->setInkColor(QColor(50, 120, 200));
	motionInterval->setInputLineColor(QColor(80, 80, 80));


	windowWidth->setFixedWidth(50);
	windowHeight->setFixedWidth(50);
	lipSync->setFixedWidth(50);
	motionInterval->setFixedWidth(50);
	const QValidator* v = new QRegExpValidator(QRegExp("[0-9]{1,2}"));
	const QValidator* v2 = new QRegExpValidator(QRegExp("[0-9]{1,4}"));
	const QValidator* v4 = new QRegExpValidator(QRegExp("[\\-]?[0-9]{1,4}"));

	dialogMaxWidth = new QtMaterialTextField();
	dialogMaxWidth->setLabel("对话框宽度限制");
	dialogMaxWidth->setLabelFontSize(8);
	dialogMaxWidth->setInkColor(QColor(50, 120, 200));
	dialogMaxWidth->setInputLineColor(QColor(80, 80, 80));

	dialogFontSize = new QtMaterialTextField();
	dialogFontSize->setLabel("对话框字体大小");
	dialogFontSize->setLabelFontSize(8);
	dialogFontSize->setInkColor(QColor(50, 120, 200));
	dialogFontSize->setInputLineColor(QColor(80, 80, 80));

	dialogYOffset = new QtMaterialTextField();
	dialogYOffset->setLabel("对话框垂直偏移");
	dialogYOffset->setLabelFontSize(8);
	dialogYOffset->setInkColor(QColor(50, 120, 200));
	dialogYOffset->setInputLineColor(QColor(80, 80, 80));


	dialogYOffset->setValidator(v4);
	dialogFontSize->setValidator(v);
	dialogMaxWidth->setValidator(v2);

	fps->setValidator(v);
	windowWidth->setValidator(v2);
	windowHeight->setValidator(v2);

	openFile = new QtMaterialRaisedButton();
	openFile->setText("打开");
	openFile->setOverlayColor(Qt::white);
	openFile->setForegroundColor(QColor(244, 244, 244));
	openFile->setBackgroundColor(QColor(50, 120, 200));
	openFile->setFixedHeight(25);
	chooseDir = new QtMaterialRaisedButton();
	chooseDir->setText("打开");
	chooseDir->setOverlayColor(Qt::white);
	chooseDir->setForegroundColor(QColor(244, 244, 244));
	chooseDir->setBackgroundColor(QColor(50, 120, 200));
	chooseDir->setFixedHeight(25);
	apply = new QtMaterialRaisedButton();
	apply->setText("保存");
	apply->setOverlayColor(Qt::white);
	apply->setForegroundColor(QColor(244, 244, 244));
	apply->setBackgroundColor(QColor(50, 120, 200));
	apply->setFixedHeight(30);
	apply->setFixedWidth(120);
	reset = new QtMaterialRaisedButton();
	reset->setText("重置");
	reset->setOverlayColor(Qt::white);
	reset->setForegroundColor(QColor(244, 244, 244));
	reset->setBackgroundColor(QColor(50, 120, 200));
	reset->setFixedHeight(30);
	reset->setFixedWidth(120);

	QVBoxLayout* vbox = new QVBoxLayout();

	QHBoxLayout* line1 = new QHBoxLayout();
	line1->addWidget(appName);
	line1->addWidget(username);
	vbox->addLayout(line1);

	QHBoxLayout* line2 = new QHBoxLayout();
	line2->addWidget(iconPath);
	line2->addWidget(openFile);
	vbox->addLayout(line2);

	QHBoxLayout* line3 = new QHBoxLayout();
	line3->addWidget(modelDir);
	line3->addWidget(chooseDir);
	vbox->addLayout(line3);

	QHBoxLayout* line4 = new QHBoxLayout();
	line4->addWidget(lipSync);
	line4->addWidget(motionInterval);
	line4->addWidget(fps);
	line4->addWidget(windowWidth);
	line4->addWidget(windowHeight);
	line4->addStretch();
	vbox->addLayout(line4);

	QHBoxLayout* line5 = new QHBoxLayout();
	line5->addWidget(dialogMaxWidth);
	line5->addWidget(dialogYOffset);
	line5->addWidget(dialogFontSize);
	vbox->addLayout(line5);

	QHBoxLayout* line6 = new QHBoxLayout();
	line6->addWidget(characterX);
	line6->addWidget(characterY);
	line6->addStretch();
	vbox->addLayout(line6);

	QHBoxLayout* line7 = new QHBoxLayout();
	line7->addWidget(lbl_volume);
	line7->addWidget(volumeSlider);
	line7->addWidget(lbl_sliderVal);
	line7->addStretch(1);
	line7->addWidget(lbl_repairMode);
	line7->addWidget(repairModeControl);
	line7->addStretch(6);
	vbox->addLayout(line7);

	QHBoxLayout* line8 = new QHBoxLayout();
	line8->addStretch(8);
	line8->addWidget(apply);
	line8->addWidget(reset);
	vbox->addLayout(line8);
	setLayout(vbox);

	connect(openFile, SIGNAL(clicked()), SLOT(OpenFile()));
	connect(chooseDir, SIGNAL(clicked()), SLOT(OpenSourceDir()));
	connect(volumeSlider, SIGNAL(valueChanged(int)), SLOT(SetVolume()));
	connect(apply, SIGNAL(clicked()), SLOT(Apply()));
	connect(reset, SIGNAL(clicked()), SLOT(Reset()));
	connect(repairModeControl, SIGNAL(toggled(bool)), SLOT(SetRepairMode()));
}

void AppSettings::OpenFile()
{
	QString path = QFileDialog::getOpenFileName(this, QString("选择图标路径"), iconPath->text(), "Image File(*.jpg *.png);;");
	if (!path.isEmpty())
	iconPath->setText(path);
}

void AppSettings::SetRepairMode() {
	// do nothing
}

void AppSettings::SetVolume() {
	lbl_sliderVal->setText(to_string(volumeSlider->value()).append("%").c_str());
}

void AppSettings::OpenSourceDir()
{
	QString path = QFileDialog::getExistingDirectory(this, QString("选择资源文件夹"), modelDir->text());
	if (!path.isEmpty())
		modelDir->setText(path);
}

void AppSettings::Apply()
{
	_AppName = appName->text().toUtf8().toStdString();
	_UserName = username->text().toUtf8().toStdString();
	_IconPath = iconPath->text().toUtf8().toStdString();
	_FPS = fps->text().toInt() == 0? _FPS : fps->text().toInt();
	_WindowWidth = windowWidth->text().toInt() == 0 ? _WindowWidth : windowWidth->text().toInt();
	_WindowHeight = windowHeight->text().toInt() == 0 ? _WindowHeight : windowHeight->text().toInt();
	_ModelDir = modelDir->text().isEmpty() ? _ModelDir : modelDir->text().toUtf8().toStdString();
	_MotionInterval = motionInterval->text().isEmpty() ? _MotionInterval : motionInterval->text().toFloat();
	_LipSyncMagnification = lipSync->text().isEmpty() ? _LipSyncMagnification : lipSync->text().toFloat();
	_DialogFontSize = dialogFontSize->text().toInt() == 0 ? _DialogFontSize : dialogFontSize->text().toInt();
	_DialogMaxWidth = dialogMaxWidth->text().toInt() == 0 ? _DialogMaxWidth : dialogMaxWidth->text().toInt();
	_DialogYOffset = dialogYOffset->text().isEmpty() ? _DialogYOffset : dialogYOffset->text().toInt();
	_SoundVolume = volumeSlider->value() / 100.0;
	_CharacterX = characterX->text().isEmpty() ? _CharacterX : characterX->text().toFloat();
	_CharacterY = characterY->text().isEmpty() ? _CharacterY : characterY->text().toFloat();

	_RepairModeOn = repairModeControl->isChecked();
	LApp::GetInstance()->GetWindow()->GetDialog()->LoadConfig();
	LApp::GetInstance()->GetWindow()->LoadConfig();
	_parent->setWindowTitle(QString::fromUtf8(_AppName.c_str()));
	LoadConfig();
	ControlWidget* p = static_cast<ControlWidget*>(_parent);
	try {
		if (p) {			
			p->_modelSettings->LoadConfig();
			Log("ModelDir changed", modelDir->text().toStdString().c_str());
		}
		else Log("AppSettings", "cast failed");
		LApp::GetInstance()->SaveConfig();
		Tip::GetInstance()->Pop(_parent, "保存成功!");
	}
	catch (exception e)
	{
		Tip::GetInstance()->Pop(_parent, "资源路径有误!");
		LApp::GetInstance()->LoadConfig();
		LoadConfig();
		p->_modelSettings->LoadConfig();
	}
}
void AppSettings::Reset()
{
	LoadConfig();
}
void AppSettings::LoadConfig()
{
	appName->setText(QString::fromUtf8(_AppName.c_str()));
	username->setText(QString::fromUtf8(_UserName.c_str()));
	iconPath->setText(QString::fromUtf8(_IconPath.c_str()));
	fps->setText(to_string(_FPS).c_str());
	windowWidth->setText(to_string(_WindowWidth).c_str());
	windowHeight->setText(to_string(_WindowHeight).c_str());
	modelDir->setText(QString::fromUtf8(_ModelDir.c_str()));
	dialogMaxWidth->setText(to_string(_DialogMaxWidth).c_str());
	dialogYOffset->setText(to_string(_DialogYOffset).c_str());
	dialogFontSize->setText(to_string(_DialogFontSize).c_str());
	lbl_sliderVal->setText(to_string((int)(LAppConfig::_SoundVolume * 100)).append("%").c_str());
	volumeSlider->setValue((int)(LAppConfig::_SoundVolume * 100));
	repairModeControl->setChecked(_RepairModeOn);
	stringstream ss;
	ss << _MotionInterval;
	motionInterval->setText(ss.str().c_str());
	ss.clear();
	ss.str("");
	ss << _LipSyncMagnification;
	lipSync->setText(ss.str().c_str());
	ss.clear();
	ss.str("");
	ss << _CharacterX;
	characterX->setText(ss.str().c_str());
	ss.clear();
	ss.str("");
	ss << _CharacterY;
	characterY->setText(ss.str().c_str());
}

void AppSettings::Release()
{
	
}

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
	lbl_motionJsonPath->setAlignment(Qt::AlignRight|Qt::AlignCenter);
	lbl_motionSoundPath = new QLabel(QString("语音"));
	lbl_motionSoundPath->setAlignment(Qt::AlignRight|Qt::AlignCenter);
	lbl_motionText = new QLabel(QString("文本"));
	lbl_motionText->setAlignment(Qt::AlignTop|Qt::AlignRight);
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
			"QScrollBar::handle:vertical{background: #AAA;}"
			"QScrollBar::handle:vertical:hover{background: #888;}"
			"QScrollBar:vertical{width: 8px;background: #DDD;border: none}"
			"QHeaderView::section{background-color: rgb(50, 50, 50); color: rgba(255, 255, 255, 210); height: 30px; font-family: PingFang SC Medium; border: none; padding-left: 10px;}"
			"QTreeWidget::item:selected {font-weight: 800; border: none }"
		);
	const char* styleSheet = ("QComboBox QAbstractItemView {outline: none; background: rgb(80, 80, 80); border: 2px solid rgb(80, 80, 80); }"
		"QComboBox QAbstractItemView::item {height: 25px; color: rgba(255, 255, 255, 210);}"
		"QComboBox QAbstractItemView::item:hover,QComboBox QAbstractItemView::item:focus {border: none; background-color: rgba(80, 150, 244, 200)}"
		"QComboBox{selection-background-color: rgb(50, 50, 50); padding-left: 5px}"
		"QComboBox::drop-down{ border-image: url(assets/open-default.png); width: 30px; height: 30px }"
		"QComboBox::drop-down:hover{ border-image: url(assets/open.png); width: 30px; height: 30px }"
		"QScrollBar::vertical{width: 10px; background: #d0d2d4}"
		"QScrollBar::handle:vertical{background: rgb(160, 160, 160)}"
		);

	model->setStyleSheet(styleSheet);
	motionJsonPath->setStyleSheet(styleSheet);
	motionSoundPath->setStyleSheet(styleSheet);

	model->setView(new QListView());
	motionJsonPath->setView(new QListView());
	motionSoundPath->setView(new QListView());

	motionText->setStyleSheet(
		"QTextEdit {color: rgba(255, 255, 255, 180)}"
		"QScrollBar::handle:vertical{background: #AAA;}"
		"QScrollBar::handle:vertical:hover{background: #888;}"
		"QScrollBar:vertical{width: 8px;background: #DDD;border: none}"
	);
}

void ModelSettings::Release()
{
	
}

void ModelSettings::Apply()
{
	if (QMessageBox::question(_parent, QString("保存配置"), QString("是否保存配置?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		//中文支持
		const char* path = QString::fromUtf8(string(_ModelDir).append("/").append(_ModelName).append("/").append(_ModelName).append(".model3.json").c_str()).toStdString().c_str();

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
	vector<std::string> _files;
	bool flag = false;
	path = QString::fromUtf8(_ModelDir.c_str()).toStdString().c_str();
	Log("Load ModelDir", path.c_str());
	getDirNames(path, _files);
	size = _files.size();
	model->clear();
	for (i = 0; i < size; i++)
	{
		if (strcmp(_ModelName.c_str(), QString(_files[i].c_str()).toStdString().c_str()) == 0) flag = true;
		model->addItem(QString(_files[i].c_str()));
	}
	_ModelName = flag ? _ModelName : model->itemText(0).toUtf8().constData();
	model->setCurrentText(_ModelName.c_str());
	motionJsonPath->clear();
	_files.clear();

	path = QString::fromUtf8(string(_ModelDir).append("/").append(_ModelName).append("/").append("motions").c_str()).toStdString();
	Log("Load MotionJsonDir", path.c_str());

	getFileNames(path, _files);
	size = _files.size();
	for (i = 0; i < size; i++)
	{
		motionJsonPath->addItem(QString(string("motions/").append(_files[i]).c_str()));
	}
	motionSoundPath->clear();
	motionJsonPath->addItem("");
	motionJsonPath->setCurrentText("");
	_files.clear();

	path = QString::fromUtf8(string(_ModelDir).append("/").append(_ModelName).append("/").append("sounds").c_str()).toStdString();
	Log("Load motionSoundDir", path.c_str());
	getFileNames(path, _files);
	size = _files.size();
	for (i = 0; i < size; i++)
	{
		motionSoundPath->addItem(QString(string("sounds/").append(_files[i]).c_str()));
	}
	motionSoundPath->addItem("");
	motionSoundPath->setCurrentText("");


	path = QString::fromUtf8(string(_ModelDir).append("/").append(_ModelName).append("/").append(_ModelName).append(".model3.json").c_str()).toStdString();
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
		const char* path = QString::fromUtf8(string(_ModelDir).append("/").append(_ModelName).append("/").append(motionJsonPath->currentText().toStdString()).c_str()).toStdString().c_str();
		if (_access(path, 0) != -1)
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
		if (_access(path, 0) != -1)
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
	else{
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

ChatSettings::ChatSettings(QWidget* p)
{
	_parent = p;
	savePath = new QtMaterialTextField();
	savePath->setInkColor(QColor(50, 120, 200));
	savePath->setInputLineColor(QColor(80, 80, 80));

	apiKey = new QtMaterialTextField();
	apiKey->setInkColor(QColor(50, 120, 200));
	apiKey->setInputLineColor(QColor(80, 80, 80));

	apiSecret = new QtMaterialTextField();
	apiSecret->setInkColor(QColor(50, 120, 200));
	apiSecret->setInputLineColor(QColor(80, 80, 80));

	hostPort = new QtMaterialTextField();
	hostPort->setInkColor(QColor(50, 120, 200));
	hostPort->setInputLineColor(QColor(80, 80, 80));

	QRegExpValidator* v = new QRegExpValidator(QRegExp("[0-9]{1,4}"));
	readTimeOut = new QtMaterialTextField();
	readTimeOut->setValidator(v);
	readTimeOut->setFixedWidth(100);
	readTimeOut->setInkColor(QColor(50, 120, 200));
	readTimeOut->setInputLineColor(QColor(80, 80, 80));

	route = new QtMaterialTextField();
	route->setInkColor(QColor(50, 120, 200));
	route->setInputLineColor(QColor(80, 80, 80));

	voiceRoute = new QtMaterialTextField();
	voiceRoute->setInkColor(QColor(50, 120, 200));
	voiceRoute->setInputLineColor(QColor(80, 80, 80));

	lbl_apiKey = new QLabel(QString("ApiKey"));
	lbl_apiSecret = new QLabel(QString("ApiKey"));
	lbl_hostPort = new QLabel(QString("服务器地址"));
	lbl_route = new QLabel(QString("文本处理路径"));
	lbl_readTimeOut = new QLabel(QString("最长响应时间"));
	lbl_savePath = new QLabel(QString("聊天保存位置"));
	lbl_voiceRoute = new QLabel(QString("语音处理路径"));
	apply = new QtMaterialRaisedButton(QString("保存"));
	apply->setOverlayColor(Qt::white);
	apply->setForegroundColor(QColor(244, 244, 244));
	apply->setBackgroundColor(QColor(50, 120, 200));
	apply->setFixedHeight(30);
	apply->setFixedWidth(120);

	reset = new QtMaterialRaisedButton(QString("重置"));
	reset->setOverlayColor(Qt::white);
	reset->setForegroundColor(QColor(244, 244, 244));
	reset->setBackgroundColor(QColor(50, 120, 200));
	reset->setFixedHeight(30);
	reset->setFixedWidth(120);

	chooseDir = new QtMaterialRaisedButton(QString("打开"));
	chooseDir->setOverlayColor(Qt::white);
	chooseDir->setForegroundColor(QColor(244, 244, 244));
	chooseDir->setBackgroundColor(QColor(50, 120, 200));
	chooseDir->setFixedHeight(30);
	chooseDir->setFixedWidth(120);

	MlyAI = new QCheckBox(QString("茉莉云API"));
	CustomChatServer = new QCheckBox(QString("自定义聊天服务器"));
	CustomVoiceChat = new QCheckBox(QString("自定义语音处理路径"));

	CustomVoiceChat->setStyleSheet(":disabled{color: grey}");

	grid = new QGridLayout();

	grid->addWidget(lbl_savePath, 0, 0, 1, 1);
	grid->addWidget(savePath, 0, 1, 1, 4);
	grid->addWidget(chooseDir, 0, 5, 1, 1);
	grid->addWidget(MlyAI, 1, 0, 1, 1);
	grid->addWidget(lbl_apiKey, 2, 0, 1, 1);
	grid->addWidget(apiKey, 2, 1, 1, 4);
	grid->addWidget(lbl_apiSecret, 3, 0, 1, 1);
	grid->addWidget(apiSecret, 3, 1, 1, 4);
	grid->addWidget(CustomChatServer, 4, 0, 1, 1);
	grid->addWidget(lbl_hostPort, 5, 0, 1, 1);
	grid->addWidget(hostPort, 5, 1, 1, 4);
	grid->addWidget(lbl_route, 6, 0, 1, 1);
	grid->addWidget(route, 6, 1, 1, 4);
	grid->addWidget(CustomVoiceChat, 7, 0, 1, 1);
	grid->addWidget(lbl_voiceRoute, 8, 0, 1, 1);
	grid->addWidget(voiceRoute, 8, 1, 1, 1);
	grid->addWidget(lbl_readTimeOut, 9, 0, 1, 1);
	grid->addWidget(readTimeOut, 9, 1, 1, 4);
	grid->addWidget(apply, 10, 4, 1, 1);
	grid->addWidget(reset, 10, 5, 1, 1);

	hostPort->setPlaceholderText("example: http://127.0.0.1:8080");
	voiceRoute->setPlaceholderText("example: /voiceRoute");
	route->setPlaceholderText("example: /chat");
	readTimeOut->setPlaceholderText("eg: 10");

	this->setLayout(grid);

	connect(apply, SIGNAL(clicked()), SLOT(Apply()));
	connect(reset, SIGNAL(clicked()), SLOT(Reset()));
	connect(MlyAI, SIGNAL(clicked()), SLOT(MlyAIChecked()));
	connect(CustomChatServer, SIGNAL(clicked()), SLOT(CustomChecked()));
	connect(chooseDir, SIGNAL(clicked()), SLOT(ChooseDir()));
	connect(CustomVoiceChat, SIGNAL(clicked()), SLOT(CustomVoiceChatChecked()));
}

void ChatSettings::Release()
{

}

void ChatSettings::Apply()
{
	_CustomChatServerHostPort = hostPort->text().toUtf8().toStdString();
	_CustomChatServerRoute = route->text().toUtf8().toStdString();
	_CustomChatServerReadTimeOut = readTimeOut->text().isEmpty() ? _CustomChatServerReadTimeOut : readTimeOut->text().toInt();
	_ChatSavePath = savePath->text().toUtf8().toStdString();
	_ApiKey = apiKey->text().toUtf8().toStdString();
	_ApiSecret = apiSecret->text().toUtf8().toStdString();
	_CustomChatServerOn = CustomChatServer->isChecked();
	_CustomVoiceChatOn = CustomVoiceChat->isChecked();
	_CustomVoiceChatRoute = voiceRoute->text().toUtf8().toStdString();
	LApp::GetInstance()->SaveConfig();
	Tip::GetInstance()->Pop(_parent, "保存成功!");
}

void ChatSettings::Reset()
{
	LoadConfig();
	Tip::GetInstance()->Pop(_parent, "已重置!");
}

void ChatSettings::ChooseDir()
{
	QString dir = QFileDialog::getExistingDirectory(_parent, QString("聊天保存路径"), ".");
	if (!dir.isEmpty()) savePath->setText(dir);
}

void ChatSettings::LoadConfig()
{
	if (_CustomChatServerOn)
	{
		 CustomChatServer->setChecked(true);
		 MlyAI->setChecked(false);
	}
	else
	{
		MlyAI->setChecked(true);
		CustomChatServer->setChecked(false);
	}
	savePath->setText(QString::fromUtf8(_ChatSavePath.c_str()));
	apiKey->setText(QString::fromUtf8(_ApiKey.c_str()));
	apiSecret->setText(QString::fromUtf8(_ApiSecret.c_str()));
	hostPort->setText(QString::fromUtf8(_CustomChatServerHostPort.c_str()));
	route->setText(QString::fromUtf8(_CustomChatServerRoute.c_str()));
	readTimeOut->setText(QString::fromUtf8(to_string(_CustomChatServerReadTimeOut).c_str()));
	voiceRoute->setText(QString::fromUtf8(_CustomVoiceChatRoute.c_str()));
	CustomVoiceChat->setChecked(_CustomVoiceChatOn);
	UpdateState();
}

void ChatSettings::CustomVoiceChatChecked() {
	UpdateState();
}

void ChatSettings::MlyAIChecked()
{
	CustomChatServer->setChecked(!MlyAI->isChecked());

	UpdateState();
}

void ChatSettings::CustomChecked()
{
	MlyAI->setChecked(!CustomChatServer->isChecked());

	UpdateState();
}

void ChatSettings::UpdateState()
{
	apiKey->setEnabled(MlyAI->isChecked());
	apiSecret->setEnabled(MlyAI->isChecked());
	hostPort->setEnabled(CustomChatServer->isChecked());
	route->setEnabled(CustomChatServer->isChecked());
	readTimeOut->setEnabled(CustomChatServer->isChecked());
	CustomVoiceChat->setEnabled(CustomChatServer->isChecked());
	voiceRoute->setEnabled(CustomChatServer->isChecked() ? CustomVoiceChat->isChecked() :false);
}

ControlWidget::ControlWidget()
{
	setFixedSize(750, 500);
	setWindowFlags(Qt::Tool);
	setWindowTitle(QString::fromUtf8(_AppName.c_str()));
	setWindowFlag(Qt::WindowStaysOnTopHint, true);
	_appSettings = new AppSettings(this);
	_modelSettings = new ModelSettings(this);
	_chatSettings = new ChatSettings(this);
	addTab(_appSettings, QString("用户设置"));
	addTab(_modelSettings, QString("模型设置"));
	addTab(_chatSettings, QString("ChatAPI"));

	QFontDatabase::addApplicationFont(QString::fromStdString(_AssetsDir).append("/PingFang-Medium.ttf"));

	setStyleSheet("QTabBar::tab{font-family: PingFang SC Medium; width: 120px; height: 30px; background-color: rgb(50, 50, 50); color: rgba(180, 180, 180, 180); padding: 0; margin: 0} "
		"QTabBar::tab:selected{background-color: rgb(30, 30, 30); color: rgba(255, 255, 255, 180); }"
		"QTabBar{background-color: rgb(37, 37, 38);}"
		"QTabWidget::pane{border: none; background-color: rgb(30, 30, 30); color: white}"
		"QTabWidget{background-color: rgb(37, 37, 38)}"
	);
	_appSettings->setStyleSheet(
		QString(
		"QWidget{color: rgba(255, 255, 255, 180); background-color: rgb(50, 50, 50); font-family: PingFang SC Medium;}"
		"QLabel{background: transparent; color: rgba(255, 255, 255, 180); font: 12px}"
		"QLineEdit{padding: 3px; color: rgb(191, 191, 191); font: 15px; border: 1px solid rgb(80, 80, 80); background-color: rgb(60, 60, 60);}"
		"QLineEdit::focus{border: 1px solid rgb(50, 120, 200)}"
		"QPushButton{font-family: PingFang SC Medium; font: 12px; border-radius: 7px; font-weight: 300; margin:5px}"
		)
	);
	_chatSettings->setStyleSheet(
		"QWidget{color: rgba(255, 255, 255, 180);background-color: rgb(50, 50, 50); font-family: PingFang SC Medium;}"
		"QLabel{background: transparent; color: rgba(255, 255, 255, 180); font: 14px}"
		"QLineEdit{padding: 3px; color: rgb(191, 191, 191); font: 15px; border: 1px solid rgb(80, 80, 80); background-color: rgb(60, 60, 60);}"
		"QLineEdit::focus{border: 1px solid rgb(50, 120, 200)}"
		"QLineEdit::!enabled{background-color: rgba(60, 60, 60, 100); color: grey; border: 1px solid rgba(80, 80, 80, 100)}"
		"QPushButton{width: 100px;font-family: PingFang SC Medium; font: 12px; border-radius: 7px; font-weight: 300; color: rgb(235, 235, 235); background-color: rgba(70, 172, 255, 190); border: none; padding-left: 7px; padding-right: 7px; padding-top: 7px; padding-bottom: 7px}"
		"QPushButton:pressed{color: rgba(255, 255, 255, 100); background-color: rgba(50, 150, 235, 150); border: none; padding-left: 10px; padding-right: 10px; padding-top: 7px; padding-bottom: 7px}"
		"QCheckBox{font: 13.5px;  font-family: PingFang SC Medium; color: rgba(255, 255, 255, 200);background: transparent;}"
	);
	_modelSettings->setStyleSheet("QWidget{color: rgba(255, 255, 255, 180); background-color: rgb(50, 50, 50); font-family: PingFang SC Medium}"
		"QLabel{background: transparent; color: rgba(255 ,255, 255, 180); font: 12px}"
		"QTreeWidget::item{height: 30px; color: rgba(255, 255, 255, 200)}"
		"QTreeWidget{outline: none; border: none; color: rgba(255, 255, 255, 245); background-color: rgb(50, 50, 50); font-family: PingFang SC Medium}"
		"QTreeWidget::item:selected:active{background-color: rgba(50, 120, 170, 180); color: rgba(255, 255, 255, 245); border: 1px solid rgb(50, 120, 240)}"
		"QTreeWidget::item:selected:!active{background-color: rgba(50, 120, 170, 90); color: rgba(255, 255, 255, 245); border: 1px solid rgb(50, 120, 240)}"
		"QTreeWidget::item:hover{background-color: rgb(50, 50, 50); border: none}"
		"QTextEdit{padding: 5px; border: 1px solid rgb(80, 80, 80); color: rgba(255, 255, 255, 245); font: 17px}"
		"QTextEdit::focus{border: 1px solid rgb(50, 120, 200)}"
		"QLineEdit{padding: 5px; color: rgb(191, 191, 191); font: 15px; border: 1px solid rgb(80, 80, 80); background-color: rgb(60, 60, 60);}"
		"QLineEdit::focus{border: 1px solid rgb(50, 120, 200)}"
		"QPushButton{font-family: PingFang SC Medium; font: 12px; border-radius: 7px; font-weight: 300; margin:5px}"
		"QComboBox{height: 30px; padding: 1px 18px 1px 3px;}"
	);
}

void ControlWidget::Release()
{
	if (_appSettings)
	{
		_appSettings->Release();
		_appSettings->deleteLater();
	}
	if (_modelSettings)
	{
		_modelSettings->Release();
		_modelSettings->deleteLater();
	}
	if (_chatSettings)
	{
		_chatSettings->Release();
		_chatSettings->deleteLater();
	}
	Tip::ReleaseInstance();
	close();
}

void ControlWidget::Pop()
{
	_appSettings->LoadConfig();
	_modelSettings->LoadConfig();
	_chatSettings->LoadConfig();
	show();
	raise();
}

