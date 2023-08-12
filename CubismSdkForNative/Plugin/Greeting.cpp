#include "Greeting.h"
#include "interface/IControlWidget.h"
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qlabel.h>

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
    _app->GetModel()->StartRandomMotion(group, 3, NULL);
}

void Greeting::Initialize(ILApp* app)
{
    _app = app;
}

Greeting::Greeting()
{
    _app = NULL;
    tabIndex = -1;
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
