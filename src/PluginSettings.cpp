#include "ControlWidget.h"
#include "PluginManager.h"
#include <QtCore/qdir.h>

PluginItemView::PluginItemView(Plugin* plugin) 
{
	this->plugin = plugin;
	QString Name = plugin->Name();
	QString Version = plugin->Version();
	QString Author = plugin->Author();
	QString Desc = plugin->Desc();
	QString PluginID = plugin->ID();
	info.clear();
	info.append(Name).append("<br>")
		.append("版本: ").append(Version).append("<br>")
		.append("作者: ").append(Author).append("<br>")
		.append("ID: ").append(PluginID).append("<br>")
		.append("描述: ").append(Desc);
	btn_load = new QPushButton(plugin->IsActivated() ? "禁用" : "启用");
	btn_load->setFixedSize(90, 30);
	connect(btn_load, SIGNAL(clicked()), this, SLOT(on_btn_load_clicked()));
	QLabel* pluginName = new QLabel(Name);
	pluginName->setStyleSheet("font-size: 14px");
	QLabel* pluginVersion = new QLabel(Version);
	pluginVersion->setStyleSheet("font-size: 12px");
	QVBoxLayout* left = new QVBoxLayout;
	QHBoxLayout* main = new QHBoxLayout;
	left->addWidget(pluginName);
	left->addWidget(pluginVersion);
	main->addLayout(left);
	main->addWidget(btn_load);
	setLayout(main);
	setFixedSize(290, 60);
}

void PluginItemView::UpdateText()
{
	btn_load->setText(plugin->IsActivated() ? "禁用" : "启用");
}

void PluginItemView::on_btn_load_clicked() {
	if (plugin->IsActivated()) plugin->Deactivate();
	else plugin->Activate();
}


PluginSettings::PluginSettings() {
	setupUI();
	ScanFolder("plugins");
}

PluginSettings::~PluginSettings() {

}

void PluginSettings::setupUI() {
	QVBoxLayout* left = new QVBoxLayout;
	QVBoxLayout* right = new QVBoxLayout;
	QHBoxLayout* lower = new QHBoxLayout;
	QHBoxLayout* upper = new QHBoxLayout;
	QVBoxLayout* main = new QVBoxLayout;
	pluginList = new QListWidget;
	pluginList->setFixedSize(300, 400);
	pluginInfo = new QTextBrowser;
	pluginInfo->setFixedSize(350, 400);
	pluginNum = new QLabel("插件数目: ");
	pluginNum->setFixedSize(120, 30);
	pluginNum->setAlignment(Qt::AlignCenter);
	left->addWidget(pluginList);
	right->addWidget(pluginInfo);
	lower->addLayout(left);
	lower->addLayout(right);
	upper->addWidget(pluginNum);
	upper->addStretch(1);
	main->addLayout(upper);
	main->addLayout(lower);
	setLayout(main);
	setStyleSheet(
		"QLabel{background: transparent; color: rgba(255 ,255, 255, 180); font: 12px; }"
		"QListWidget::item{height: 30px; color: rgba(255, 255, 255, 200)}"
		"QListWidget{outline: none; border: none; color: rgba(255, 255, 255, 245); background-color: rgb(50, 50, 50); font-family: PingFang SC Medium}"
		"QListWidget::item:selected:active{background-color: rgba(50, 120, 170, 180); color: rgba(255, 255, 255, 245); border: 1px solid rgb(50, 120, 240)}"
		"QListWidget::item:selected:!active{background-color: rgba(50, 120, 170, 90); color: rgba(255, 255, 255, 245); border: 1px solid rgb(50, 120, 240)}"
		"QListWidget::item:hover{background-color: rgb(50, 50, 50); border: none}"
		"QTextBrowser {color: rgba(255, 255, 255, 245); background-color: rgb(50, 50, 50); border: 1px solid rgb(80, 80, 80); color: white;  font: 17px;}"
		"QTextBrowser:focus{border: 1px solid rgb(50, 120, 200)}"
		"QPushButton{font-family: PingFang SC Medium; font: 12px; background-color: rgb(90, 150, 244); border-radius: 7px; font-weight: 300; margin:5px; color: white; }"
		"QPushButton:pressed{background-color: rgb(80, 120, 224); }"
	);
}

void PluginSettings::on_plugin_item_clicked(const QModelIndex& idx) {
	PluginItemView* view = (PluginItemView*)pluginList->itemWidget(pluginList->item(idx.row()));
	pluginInfo->clear();
	pluginInfo->insertHtml(view->info);
}

void PluginSettings::AddPlugin(Plugin* plugin) {
	QListWidgetItem* item = new QListWidgetItem;
	PluginItemView* view = new PluginItemView(plugin);
	plugin->BindItemView(view);
	pluginList->addItem(item);
	item->setSizeHint(view->size());
	pluginList->setItemWidget(item, view);
	connect(pluginList, SIGNAL(clicked(const QModelIndex&)), this, SLOT(on_plugin_item_clicked(const QModelIndex&)));
}

void PluginSettings::ScanFolder(const char* filePath)
{
	int cnt = 0;
	QDir dir("plugins");
	if (dir.exists())
	{
		const QFileInfoList& ls = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
		for (const QFileInfo& info : ls) {
			QString path = QString(info.fileName()).append("/").append(info.fileName()).append(".dll");
			if (dir.exists(path)) {
				Plugin* p = PluginManager::GetInstance()->Register(path.toStdString().c_str());
				AddPlugin(p);
				cnt++;
			}
		}
	}
	else dir.mkpath(".");
	pluginNum->setText(QString("插件数量: ").append(to_string(cnt).c_str()));
}

