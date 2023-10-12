#include "Greeting.h"
#include "../src/interface/IControlWidget.h"
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qlabel.h>
#include "../src/interface/ILAppModel.h"
#include "../src/interface/IDialog.h"
#include <QtCore/qthread.h>

static ILApp* _iapp = NULL;

void Greeting::OnLaunch()
{
    //启动时问候语
    const char* greeting = NULL;
    const char* group = NULL;
    time_t tick = time(0);
    struct tm now;
    
    localtime_s(&now, &tick);
    
    if (now.tm_hour > 4 && now.tm_hour < 12)
    {
        group = "Morning";
    }
    else if (now.tm_hour < 18)
    {
        group = "Afternoon";
    }
    else if (now.tm_hour < 21) {
        group = "Evening";
    }
    else {
        group = "Midnight";
    }
    _app->GetModel()->StartRandomMotion(group, PriorityForce, [](IMotion* self) {
        _iapp->GetGLWidget()->GetDialog()->TimeUp();
    });
}

void Greeting::OnScheduledTask()
{
    if (shutDown) {
        _app->ReleaseHold();
        shutDown = false;
    }

    if (frameCount / _app->_FPS() > 3600)
    {
    	    _app->GetModel()->StartRandomMotion("LongSittingTip", PriorityForce, [](IMotion* self) {
                _iapp->GetGLWidget()->GetDialog()->TimeUp();
            });
    	    frameCount = 0;
    } 
    else frameCount++;
}

void OnFinishMotionCallBack(IMotion* self) {
    _iapp->ReleaseHold();
    _iapp->GetGLWidget()->GetDialog()->TimeUp();
}

void Greeting::OnShutdown()
{
    _app->GetModel()->Speak("再见~", "", [](IMotion* self) {
        _iapp->GetGLWidget()->GetDialog()->TimeUp();
        });
    _app->Hold();
    shutDown = true;
}

void Greeting::Initialize(ILApp* app)
{
    _app = app;
    _iapp = app;
}

Greeting::Greeting()
{
    _app = NULL;
    _settings = NULL;
    frameCount = 0;
}

Greeting::~Greeting()
{
}

void Greeting::Activate()
{
    _settings = new QWidget;
    QLabel* lbl = new QLabel("Greeting 设置页面 (插件演示样例)", _settings);
    lbl->setStyleSheet("color: rgba(255, 255, 255, 200)");
    lbl->move(20, 20);
    _app->GetGLWidget()->GetControlWidget()->GetSelf()->addTab(_settings, "Greeting");
}

void Greeting::Deactivate()
{
    QTabWidget* w = _app->GetGLWidget()->GetControlWidget()->GetSelf();
    w->removeTab(w->indexOf(_settings));
}
