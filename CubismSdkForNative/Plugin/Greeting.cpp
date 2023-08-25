#include "Greeting.h"
#include "interface/IControlWidget.h"
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qlabel.h>

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
    _app->GetModel()->StartRandomMotion(group, 3);
    printf("OnLaunch finished\n");
}

void Greeting::OnScheduledTask()
{
    if (frameCount / _app->_FPS() > 10)
    {
    	_app->GetModel()->StartRandomMotion("LongSittingTip", 3);
    	frameCount = 0;
    } 
    else frameCount++;
}

void OnFinishMotionCallBack(IMotion* self) {
    _iapp->ReleaseHold();
}

#include "interface/IDialog.h"
void Greeting::OnShutdown()
{
    _app->Hold();
    _app->GetModel()->Speak("再见~", "", OnFinishMotionCallBack);
}

void Greeting::Initialize(ILApp* app)
{
    _app = app;
    _iapp = app;
}

Greeting::Greeting()
{
    _app = NULL;
    tabIndex = -1;
    frameCount = 0;
}

Greeting::~Greeting()
{
}

void Greeting::Activate()
{
    QWidget* w = new QWidget;
    QLabel* lbl = new QLabel("Greeting 设置页面 (插件演示样例)", w);
    lbl->setStyleSheet("color: rgba(255, 255, 255, 200)");
    lbl->move(20, 20);
    tabIndex = _app->GetGLWidget()->GetControlWidget()->GetSelf()->addTab(w, "Greeting");
}

void Greeting::Deactivate()
{
    QTabWidget* w = _app->GetGLWidget()->GetControlWidget()->GetSelf();
    w->removeTab(tabIndex);
}
