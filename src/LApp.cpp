#include "LAppDelegate.hpp"
#include "LApp.h"
#include "LAppDefine.hpp"
#include "LAppPal.hpp"
#include <iostream>
#include <fstream>
#include "json/json.h"
#include <unordered_map>
#include <vector>
#include <QtCore/qtextcodec.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qapplication.h>
using namespace std;
using namespace LAppDefine;

namespace LAppConfig {
    int _WindowWidth;
    int _WindowHeight;
    int _LastPosX;
    int _LastPosY;
    int _FPS;
    string _WindowTitle;
    string _IconPath;
    string _DefaultModelName;
    string _ModelDir;
    bool _KeepQuiet;
    bool _MouseTrack;
    bool _StayOnTop;
    bool _NoSound;
    bool _ShowText;
    const char* _ConfigPath;
    string _DialogStyleSheet;
    int _TextFadeOutTime;
    int _DialogWidth;
    int _DialogHeight;
};

namespace {
    LApp* _instance = NULL;
}


void LApp::warning(const char* x)
{
    QTextCodec* codec = QTextCodec::codecForName("gb2312");
    QMessageBox::warning(NULL, codec->toUnicode("警告"), codec->toUnicode(x));
}


LApp::LApp()
{

}

LApp* LApp::GetInstance()
{
	if (!_instance)
	{
		_instance = new LApp();
	}
    return _instance;
}

QApplication* LApp::GetApp()
{
    return _app;
}

GLWidget* LApp::GetWindow()
{
    return _win;
}

void LApp::Initialize(int argc, char* argv[])
{
    _app = new QApplication(argc, argv);
    char* testConfigPath = "config.json";
    LAppConfig::_ConfigPath = testConfigPath;
    if (DebugLogEnable)
    {
        LAppPal::PrintLog("[APP]config path: [%s]", testConfigPath);
    }
    ifstream file;
    file.open(testConfigPath);
    Json::Value config;
    if (!file.fail())
    {
        try {
            file >> config;
        }
        catch (exception e)
        {
            LApp::warning("Json配置文件格式有误！");
            file.close();
            LApp::GetInstance()->GetApp()->quit();
            exit(0);
        }
    }
    file.close();
    LAppConfig::_IconPath = !config["WindowSettings"]["IconPath"].isNull() ? config["WindowSettings"]["IconPath"].asCString() : "";
    LAppConfig::_WindowTitle = !config["WindowSettings"]["AppName"].isNull() ? config["WindowSettings"]["AppName"].asCString() : "Live2D Displayer";
    LAppConfig::_FPS = !config["WindowSettings"]["FPS"].isNull() ? config["WindowSettings"]["FPS"].asInt() : 60;
    LAppConfig::_WindowWidth = !config["WindowSettings"]["Width"].isNull() ? config["WindowSettings"]["Width"].asInt() : 500;
    LAppConfig::_WindowHeight = !config["WindowSettings"]["Height"].isNull() ? config["WindowSettings"]["Height"].asInt() : 700;
    LAppConfig::_LastPosX = !config["WindowSettings"]["LastPos"]["X"].isNull() ? config["WindowSettings"]["LastPos"]["X"].asInt() : 0;
    LAppConfig::_LastPosY = !config["WindowSettings"]["LastPos"]["Y"].isNull() ? config["WindowSettings"]["LastPos"]["Y"].asInt() : 0;
    LAppConfig::_DefaultModelName = !config["UserSettings"]["ModelName"].isNull() ? config["UserSettings"]["ModelName"].asCString() : "Hiyori";
    LAppConfig::_ModelDir = !config["UserSettings"]["ModelDir"].isNull() ? config["UserSettings"]["ModelDir"].asCString() : "Resources";
    LAppConfig::_MouseTrack = !config["UserSettings"]["MouseTrack"].isNull() ? config["UserSettings"]["MouseTrack"].asBool() : true;
    LAppConfig::_KeepQuiet = !config["UserSettings"]["KeepQuiet"].isNull() ? config["UserSettings"]["KeepQuiet"].asBool() : false;
    LAppConfig::_StayOnTop = !config["UserSettings"]["StayOnTop"].isNull() ? config["UserSettings"]["StayOnTop"].asBool() : false;
    LAppConfig::_NoSound = !config["UserSettings"]["NoSound"].isNull() ? config["UserSettings"]["NoSound"].asBool() : false;
    LAppConfig::_ShowText = !config["UserSettings"]["ShowText"].isNull() ? config["UserSettings"]["ShowText"].asBool() : true;
    LAppConfig::_TextFadeOutTime = !config["UserSettings"]["TextFadeOutTime"].isNull() ? config["UserSettings"]["TextFadeOutTime"].asInt() : 6;
    LAppConfig::_DialogWidth = !config["UserSettings"]["Dialog"]["Width"].isNull() ? config["UserSettings"]["Dialog"]["Width"].asInt() : 400;
    LAppConfig::_DialogHeight = !config["UserSettings"]["Dialog"]["Height"].isNull() ? config["UserSettings"]["Dialog"]["Height"].asInt() : 150;
    LAppConfig::_DialogStyleSheet = !config["UserSettings"]["Dialog"]["StyleSheet"].isNull() ? config["UserSettings"]["Dialog"]["StyleSheet"].asCString() :
        "font-size: 20px;"
        "border: 5px solid rgb(100, 100, 100);"
        "background-color: rgba(100, 100, 100, 200);"
        "padding: 10px;"
        "margin : 0;"
        "color: white;"
        "font-family: Comic Sans MS;";
    _win = new GLWidget();
    _win->loadConfig();
}

void LApp::Run()
{
    _win->Run();
    _app->exec();
}

void LApp::Release()
{
    _win->Release();
    _win->deleteLater();
    _app->quit();
    LAppDelegate::GetInstance()->Release();
    exit(0);
}

void LApp::SaveConfig()
{
    Json::Value config;
    ifstream ifs(LAppConfig::_ConfigPath);
    if (!ifs.fail())
    {
        ifs >> config;
    }
    ifs.close();
    config["WindowSettings"]["Width"] = _win->width();
    config["WindowSettings"]["Height"] = _win->height();
    config["WindowSettings"]["LastPos"]["X"] = _win->x();
    config["WindowSettings"]["LastPos"]["Y"] = _win->y();
    config["UserSettings"]["MouseTrack"] = _win->MouseTrack();
    config["UserSettings"]["KeepQuiet"] = _win->Quiet();
    config["UserSettings"]["StayOnTop"] = _win->OnTop();
    config["UserSettings"]["NoSound"] = _win->NoSound();
    config["UserSettings"]["ShowText"] = _win->ShowText();
    config["UserSettings"]["ModelDir"] = LAppConfig::_ModelDir;
    config["UserSettings"]["ModelName"] = LAppConfig::_DefaultModelName;
    config["UserSettings"]["TextFadeOutTime"] = LAppConfig::_TextFadeOutTime;
    config["UserSettings"]["Dialog"]["Width"] = LAppConfig::_DialogWidth;
    config["UserSettings"]["Dialog"]["Height"] = LAppConfig::_DialogHeight;
    config["UserSettings"]["Dialog"]["StyleSheet"] = LAppConfig::_DialogStyleSheet;
    config["WindowSettings"]["IconPath"] = LAppConfig::_IconPath;
    config["WindowSettings"]["AppName"] = LAppConfig::_WindowTitle;
    config["WindowSettings"]["FPS"] = LAppConfig::_FPS;
    ofstream ofs(LAppConfig::_ConfigPath);
    if (ofs.fail())
    {
        LApp::warning("配置文件保存失败!");
        return;
    }
    ofs << config.toStyledString().c_str();
    ofs.close();
}