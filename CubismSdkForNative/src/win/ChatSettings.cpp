#include "ControlWidget.h"
#include "../LApp.h"
#include <QtWidgets/qfiledialog.h>


using namespace LAppConfig;


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
	voiceRoute->setEnabled(CustomChatServer->isChecked() ? CustomVoiceChat->isChecked() : false);
}