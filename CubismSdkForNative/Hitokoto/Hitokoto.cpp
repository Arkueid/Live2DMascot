#include "Hitokoto.h"
#include "../src/interface/IControlWidget.h"
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qlabel.h>
#include "../src/interface/ILAppModel.h"
#include <QtCore/qdir.h>
#include <fstream>
#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonobject.h> 
#include <QtCore/qjsonarray.h>
#include "../src/interface/IDialog.h"

static ILApp* _iapp = NULL;

Hitokoto::Hitokoto()
{
    _app = NULL;
    _settings = NULL;
    frameCount = 0;
}

Hitokoto::~Hitokoto()
{
}

void Hitokoto::Activate()
{
    _settings = new QWidget;
    QLabel* lbl = new QLabel("Hitokoto 设置页面 (插件演示样例)", _settings);
    lbl->setStyleSheet("color: rgba(255, 255, 255, 200)");
    lbl->move(20, 20);
    _app->GetGLWidget()->GetControlWidget()->GetSelf()->addTab(_settings, "Hitokoto");
}

void Hitokoto::Deactivate()
{
    QTabWidget* w = _app->GetGLWidget()->GetControlWidget()->GetSelf();
    w->removeTab(w->indexOf(_settings));
}

void Hitokoto::Initialize(ILApp* app)
{
	_app = app;
    _iapp = app;
}

void Hitokoto::OnLaunch()
{

}

void Hitokoto::OnScheduledTask()
{
    if (_app->GetModel()->IsMotionFinished()) {
        if (frameCount / _app->_FPS() > 5)
        {
            _app->GetModel()->Speak(GetRandomSentence().toStdString().c_str(), "", [](IMotion* self) {
                _iapp->GetGLWidget()->GetDialog()->TimeUp();
            });
            frameCount = 0;
        }
        else {
            frameCount++;
        }
    }
}

void Hitokoto::OnShutdown()
{ 
}

QString Hitokoto::GetRandomSentence()
{
    QDir dir("plugins/Hitokoto/assets/sentences");
    QFileInfoList ls = dir.entryInfoList(QDir::Files);
    QFile f;
    QString s = "plugins/Hitokoto/assets/sentences/" + ls[rand() % ls.size()].fileName();
    f.setFileName(s);
    f.open(QIODevice::ReadOnly);
    if (f.isOpen()) {
        printf("[Plugin][Hitokoto]:%s\n", s.toStdString().c_str());
    }
    else return "sentence get error";
    QByteArray bytes = f.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(bytes);
    QJsonArray json = doc.array();
    int idx = rand() % doc.array().size();
    f.close();
    return json.at(idx).toObject()["hitokoto"].toString();
}
