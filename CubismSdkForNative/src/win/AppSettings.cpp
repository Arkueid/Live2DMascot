#include "ControlWidget.h"
#include "../LApp.h"
#include <QtWidgets/qfiledialog.h>

using namespace LAppConfig;

AppSettings::AppSettings(QWidget* p)
{
	_parent = p;

	appName = new QtMaterialTextField();
	appName->setLabel("应用名称");
	//appName->setLabelFontSize(8);
	appName->setInkColor(QColor(50, 120, 200));
	appName->setInputLineColor(QColor(80, 80, 80));
	appName->setStyleSheet("font-size: 14px;");

	username = new QtMaterialTextField();
	username->setLabel("用户名称");
	//username->setLabelFontSize(8);
	username->setInkColor(QColor(50, 120, 200));
	username->setInputLineColor(QColor(80, 80, 80));
	username->setStyleSheet("font-size: 14px;");


	iconPath = new QtMaterialTextField();
	iconPath->setLabel("图标路径");
	//iconPath->setLabelFontSize(8);
	iconPath->setInkColor(QColor(50, 120, 200));
	iconPath->setInputLineColor(QColor(80, 80, 80));
	iconPath->setStyleSheet("font-size: 14px;");


	fps = new QtMaterialTextField();
	fps->setLabel("FPS");
	//fps->setLabelFontSize(8);
	fps->setInkColor(QColor(50, 120, 200));
	fps->setInputLineColor(QColor(80, 80, 80));
	fps->setStyleSheet("font-size: 14px;");

	windowWidth = new QtMaterialTextField();
	windowWidth->setLabel("桌宠宽度");
	//windowWidth->setLabelFontSize(8);
	windowWidth->setInkColor(QColor(50, 120, 200));
	windowWidth->setInputLineColor(QColor(80, 80, 80));
	windowWidth->setStyleSheet("font-size: 14px;");
	windowWidth->setMaximumWidth(60);

	windowHeight = new QtMaterialTextField();
	windowHeight->setLabel("桌宠高度");
	//windowHeight->setLabelFontSize(8);
	windowHeight->setInkColor(QColor(50, 120, 200));
	windowHeight->setInputLineColor(QColor(80, 80, 80));
	windowHeight->setStyleSheet("font-size: 14px;");

	modelDir = new QtMaterialTextField();
	modelDir->setLabel("模型路径");
	//modelDir->setLabelFontSize(8);
	modelDir->setInkColor(QColor(50, 120, 200));
	modelDir->setInputLineColor(QColor(80, 80, 80));
	modelDir->setStyleSheet("font-size: 14px;");

	lbl_volume = new QLabel(QString("音量"));
	lbl_volume->setStyleSheet("color: rgb(191, 191, 191); font-size: 14px;");
	lbl_sliderVal = new QLabel();
	lbl_sliderVal->setStyleSheet("color: rgb(191, 191, 191); font-size: 14px;");
	volumeSlider = new QtMaterialSlider();
	volumeSlider->setMaximum(100);
	volumeSlider->setMinimum(0);
	volumeSlider->setOrientation(Qt::Horizontal);
	volumeSlider->setPageStep(1);
	volumeSlider->setFixedWidth(200);
	volumeSlider->setThumbColor(QColor(50, 120, 200));
	lbl_repairMode = new QLabel(QString("动作修复"));
	lbl_repairMode->setStyleSheet("color: rgb(191, 191, 191); font-size: 14px;");
	repairModeControl = new QtMaterialToggle();
	repairModeControl->setCheckable(true);
	repairModeControl->setActiveColor(QColor(50, 120, 200));
	repairModeControl->setFixedHeight(45);

	lipSync = new QtMaterialTextField();

	QRegExpValidator* v3 = new QRegExpValidator(QRegExp("^[0-9]{1,2}\\.[0-9]{1}$"));
	lipSync->setValidator(v3);
	lipSync->setLabel("口型同步");
	//lipSync->setLabelFontSize(8);
	lipSync->setInkColor(QColor(50, 120, 200));
	lipSync->setInputLineColor(QColor(80, 80, 80));
	lipSync->setStyleSheet("font-size: 14px;");

	characterX = new QtMaterialTextField();
	characterX->setLabel("角色绘制 X 坐标");
	//characterX->setLabelFontSize(8);
	characterX->setInkColor(QColor(50, 120, 200));
	characterX->setInputLineColor(QColor(80, 80, 80));
	characterX->setStyleSheet("font-size: 14px;");

	characterY = new QtMaterialTextField();
	characterY->setLabel("角色绘制 Y 坐标");
	//characterY->setLabelFontSize(8);
	characterY->setInkColor(QColor(50, 120, 200));
	characterY->setInputLineColor(QColor(80, 80, 80));
	characterY->setStyleSheet("font-size: 14px;");

	const QValidator* v5 = new QRegExpValidator(QRegExp("^[\\-]?[0-9]{1,2}\\.[0-9]{1,2}$"));
	characterX->setValidator(v5);
	characterY->setValidator(v5);

	motionInterval = new QtMaterialTextField();
	motionInterval->setValidator(v3);
	motionInterval->setLabel("动作频率");
	//motionInterval->setLabelFontSize(8);
	motionInterval->setInkColor(QColor(50, 120, 200));
	motionInterval->setInputLineColor(QColor(80, 80, 80));
	motionInterval->setStyleSheet("font-size: 14px;");

	fps->setFixedWidth(80);
	windowWidth->setFixedWidth(80);
	windowHeight->setFixedWidth(80);
	lipSync->setFixedWidth(80);
	motionInterval->setFixedWidth(80);
	const QValidator* v = new QRegExpValidator(QRegExp("[0-9]{1,2}"));
	const QValidator* v2 = new QRegExpValidator(QRegExp("[0-9]{1,4}"));
	const QValidator* v4 = new QRegExpValidator(QRegExp("[\\-]?[0-9]{1,4}"));

	dialogMaxWidth = new QtMaterialTextField();
	dialogMaxWidth->setLabel("对话框宽度限制");
	//dialogMaxWidth->setLabelFontSize(8);
	dialogMaxWidth->setInkColor(QColor(50, 120, 200));
	dialogMaxWidth->setInputLineColor(QColor(80, 80, 80));
	dialogMaxWidth->setStyleSheet("font-size: 14px;");

	dialogFontSize = new QtMaterialTextField();
	dialogFontSize->setLabel("对话框字体大小");
	////dialogFontSize->setLabelFontSize(8);
	dialogFontSize->setInkColor(QColor(50, 120, 200));
	dialogFontSize->setInputLineColor(QColor(80, 80, 80));
	dialogFontSize->setStyleSheet("font-size: 14px;");

	dialogYOffset = new QtMaterialTextField();
	dialogYOffset->setLabel("对话框垂直偏移");
	//dialogYOffset->setLabelFontSize(8);
	dialogYOffset->setInkColor(QColor(50, 120, 200));
	dialogYOffset->setInputLineColor(QColor(80, 80, 80));
	dialogYOffset->setStyleSheet("font-size: 14px;");

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
	openFile->setStyleSheet("font-size: 12px;");
	chooseDir = new QtMaterialRaisedButton();
	chooseDir->setText("打开");
	chooseDir->setOverlayColor(Qt::white);
	chooseDir->setForegroundColor(QColor(244, 244, 244));
	chooseDir->setBackgroundColor(QColor(50, 120, 200));
	chooseDir->setFixedHeight(25);
	chooseDir->setStyleSheet("font-size: 12px;");
	apply = new QtMaterialRaisedButton();
	apply->setText("保存");
	apply->setOverlayColor(Qt::white);
	apply->setForegroundColor(QColor(244, 244, 244));
	apply->setBackgroundColor(QColor(50, 120, 200));
	apply->setFixedHeight(30);
	apply->setFixedWidth(120);
	apply->setStyleSheet("font-size: 12px;");
	reset = new QtMaterialRaisedButton();
	reset->setText("重置");
	reset->setOverlayColor(Qt::white);
	reset->setForegroundColor(QColor(244, 244, 244));
	reset->setBackgroundColor(QColor(50, 120, 200));
	reset->setFixedHeight(30);
	reset->setFixedWidth(120);
	reset->setStyleSheet("font-size: 12px;");

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
}

void AppSettings::OpenFile()
{
	QString path = QFileDialog::getOpenFileName(this, QString("选择图标路径"), iconPath->text(), "Image File(*.jpg *.png);;");
	if (!path.isEmpty())
		iconPath->setText(path);
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
	_FPS = fps->text().toInt() == 0 ? _FPS : fps->text().toInt();
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
	ControlWidget* p = (ControlWidget*) _parent;
	try {
		if (p) {
			p->GetModelSettings()->LoadConfig();
			Log("ModelDir changed", modelDir->text().toLocal8Bit().constData());
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
		p->GetModelSettings()->LoadConfig();
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