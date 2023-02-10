#include <iostream>
#include <fstream>
#include <Windows.h>
#include <lmcons.h>
#include <io.h>
#include <QtWidgets/qmessagebox.h>

#include "LAppDelegate.hpp"
#include "LApp.h"
#include "LAppDefine.hpp"
#include "LAppPal.hpp"
#include "json/json.h"
#include "NetworkUtils.h"
using namespace std;
using namespace LAppDefine;

namespace LAppConfig {
    int _WindowWidth;
    int _WindowHeight;
    int _LastPosX;
    int _LastPosY;
    int _FPS;
    int _MotionInterval;
    string _AppName;
    string _IconPath;
    string _ModelName;
    string _ModelDir;
    bool _KeepQuiet;
    bool _MouseTrack;
    bool _StayOnTop;
    bool _NoSound;
    bool _ShowBgmList;
    bool _ShowText;
    const char* _ConfigPath;
    string _DialogStyleSheet;
    int _TextFadeOutTime;
    int _DialogWidth;
    int _DialogHeight;
    int _BgmListLastPosX;
    int _BgmListLastPosY;
    int _DialogWordInterval;
    string _NoteOutPath;
    string _ApiKey;
    string _ApiSecret;
    string _UserName;
    bool _ShowBackground;
    bool _TransparentBackground;
    bool _MouseOn;
    bool _TransparentCharacter;
};

namespace {
    LApp* _instance = NULL;
}


void LApp::Warning(const char* x)
{
    QMessageBox::warning(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit(x));
}


LApp::LApp()
{
    _app = NULL;
    _win = NULL;
}

LApp::~LApp()
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
    _app->connect(qApp, &QGuiApplication::commitDataRequest, [&](QSessionManager& manager) {
        // 关机、重启、注销前 保存数据
        // 例如Win系统，有时关机缓慢，用户会点击“强制关机”，系统会直接kill剩余进程
        // 所以在得知系统即将关机的时候，便应立即保存数据，以免被kill而错失时机
        LApp::SaveConfig();
        });
    _win = new GLWidget();
    BgmListUtils::CheckUpdate();
    HolidayUtils::CheckUpdate();
    LoadConfig();
    _win->setupUI();
}

void LApp::LoadConfig() {
    char* testConfigPath = "config.json";
    LAppConfig::_ConfigPath = testConfigPath;
    if (DebugLogEnable)
    {
        Log("Config", testConfigPath);
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
            LApp::Warning("Json配置文件格式有误！");
            file.close();
            LApp::GetInstance()->GetApp()->quit();
            exit(0);
        }
    }
    file.close();

    LAppConfig::_ModelDir = !config["UserSettings"]["ModelDir"].isNull() ? QString::fromLocal8Bit(config["UserSettings"]["ModelDir"].asCString()).toLocal8Bit().constData() : "Resources";
    if (DebugLogEnable) Log("Resource Dir", LAppConfig::_ModelDir.c_str());
    if (_access(LAppConfig::_ModelDir.c_str(), 0) == -1) {
        LApp::Warning("资源文件夹路径不正确！\n请修改config.json文件");
        exit(0);
    }
    LAppConfig::_ModelName = !config["UserSettings"]["ModelName"].isNull() ? QString::fromLocal8Bit(config["UserSettings"]["ModelName"].asCString()).toLocal8Bit().constData() : "Hiyori";
    
    if (_access(string(LAppConfig::_ModelDir).append("/").append(LAppConfig::_ModelName).c_str(), 0) == -1)
    {
        LApp::Warning("模型文件不存在！\n请修改config.json文件");
        exit(0);
    }
    LAppConfig::_IconPath = !config["WindowSettings"]["IconPath"].isNull() ? QString::fromLocal8Bit(config["WindowSettings"]["IconPath"].asCString()).toLocal8Bit().constData() : "";
    LAppConfig::_AppName = !config["WindowSettings"]["AppName"].isNull() ? QString::fromLocal8Bit(config["WindowSettings"]["AppName"].asCString()).toLocal8Bit().constData() : "Live2D Displayer";
    LAppConfig::_FPS = !config["WindowSettings"]["FPS"].isNull() ? config["WindowSettings"]["FPS"].asInt() : 48;
    LAppConfig::_WindowWidth = !config["WindowSettings"]["Width"].isNull() ? config["WindowSettings"]["Width"].asInt() : 500;
    LAppConfig::_WindowHeight = !config["WindowSettings"]["Height"].isNull() ? config["WindowSettings"]["Height"].asInt() : 700;
    LAppConfig::_LastPosX = !config["WindowSettings"]["LastPos"]["X"].isNull() ? config["WindowSettings"]["LastPos"]["X"].asInt() : 0;
    LAppConfig::_LastPosY = !config["WindowSettings"]["LastPos"]["Y"].isNull() ? config["WindowSettings"]["LastPos"]["Y"].asInt() : 0;
    LAppConfig::_BgmListLastPosX = !config["UserSettings"]["BgmList"]["LastPos"]["X"].isNull() ? config["UserSettings"]["BgmList"]["LastPos"]["X"].asInt() : 500;
    LAppConfig::_BgmListLastPosY = !config["UserSettings"]["BgmList"]["LastPos"]["Y"].isNull() ? config["UserSettings"]["BgmList"]["LastPos"]["Y"].asInt() : 0;
    LAppConfig::_MouseTrack = !config["UserSettings"]["MouseTrack"].isNull() ? config["UserSettings"]["MouseTrack"].asBool() : true;
    LAppConfig::_KeepQuiet = !config["UserSettings"]["KeepQuiet"].isNull() ? config["UserSettings"]["KeepQuiet"].asBool() : false;
    LAppConfig::_StayOnTop = !config["UserSettings"]["StayOnTop"].isNull() ? config["UserSettings"]["StayOnTop"].asBool() : false;
    LAppConfig::_TransparentCharacter = !config["UserSettings"]["TransparentCharacter"].isNull() ? config["UserSettings"]["TransparentCharacter"].asBool() : false;

    LAppConfig::_ShowBackground = !config["UserSettings"]["ShowBackground"].isNull() ? config["UserSettings"]["ShowBackground"].asBool() : false;
    LAppConfig::_TransparentBackground = !config["UserSettings"]["TransparentBackground"].isNull() ? config["UserSettings"]["TransparentBackground"].asBool() : true;

    LAppConfig::_NoSound = !config["UserSettings"]["NoSound"].isNull() ? config["UserSettings"]["NoSound"].asBool() : false;
    LAppConfig::_ShowText = !config["UserSettings"]["ShowText"].isNull() ? config["UserSettings"]["ShowText"].asBool() : true;
    LAppConfig::_ShowBgmList = !config["UserSettings"]["ShowBgmList"].isNull() ? config["UserSettings"]["ShowBgmList"].asBool() : true;
    LAppConfig::_TextFadeOutTime = !config["UserSettings"]["TextFadeOutTime"].isNull() ? config["UserSettings"]["TextFadeOutTime"].asInt() : 6;
    LAppConfig::_DialogWidth = !config["UserSettings"]["Dialog"]["Width"].isNull() ? config["UserSettings"]["Dialog"]["Width"].asInt() : 400;
    LAppConfig::_DialogHeight = !config["UserSettings"]["Dialog"]["Height"].isNull() ? config["UserSettings"]["Dialog"]["Height"].asInt() : 150;
    LAppConfig::_ApiKey = !config["UserSettings"]["Mlyai"]["APIKey"].isNull() ? QString::fromLocal8Bit(config["UserSettings"]["Mlyai"]["APIKey"].asCString()).toLocal8Bit().constData() : "82wmm51s1bskwft3";
    LAppConfig::_ApiSecret = !config["UserSettings"]["Mlyai"]["APISecret"].isNull() ? QString::fromLocal8Bit(config["UserSettings"]["Mlyai"]["APISecret"].asCString()).toLocal8Bit().constData() : "o0vp8k7e";
    TCHAR username[UNLEN + 1];
    DWORD size = UNLEN + 1;
    GetUserName((TCHAR*)username, &size);
    LAppConfig::_UserName = !config["UserSettings"]["UserName"].isNull() ? QString::fromLocal8Bit(config["UserSettings"]["UserName"].asCString()).toLocal8Bit().constData() : username;
    LAppConfig::_NoteOutPath = !config["UserSettings"]["NoteOutPath"].isNull() ? QString::fromLocal8Bit(config["UserSettings"]["NoteOutPath"].asCString()).toLocal8Bit().constData() : ".";
    LAppConfig::_DialogWordInterval = !config["UserSettings"]["Dialog"]["WordInterval"].isNull() ? config["UserSettings"]["Dialog"]["WordInterval"].asInt() : 10;
    LAppConfig::_MotionInterval = !config["UserSettings"]["MotionInterval"].isNull() ? config["UserSettings"]["MotionInterval"].asInt() : 5;
    LAppConfig::_DialogStyleSheet = !config["UserSettings"]["Dialog"]["StyleSheet"].isNull() ? config["UserSettings"]["Dialog"]["StyleSheet"].asCString() :
        "font-size: 20px;"
        "border: 1px solid rgb(0, 0, 0);"
        "background-color: rgba(0, 0, 0, 200);"
        "padding: 10px;"
        "margin : 0;"
        "color: white;"
        "font-family: Comic Sans MS;";
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
    config["UserSettings"]["MouseTrack"] = LAppConfig::_MouseTrack;
    config["UserSettings"]["KeepQuiet"] = LAppConfig::_KeepQuiet;
    config["UserSettings"]["StayOnTop"] = LAppConfig::_StayOnTop;
    config["UserSettings"]["NoSound"] = LAppConfig::_NoSound;
    config["UserSettings"]["ShowText"] = LAppConfig::_ShowText;
    config["UserSettings"]["ShowBgmList"] = LAppConfig::_ShowBgmList;
    config["UserSettings"]["ModelDir"] = LAppConfig::_ModelDir;
    config["UserSettings"]["ModelName"] = LAppConfig::_ModelName;
    config["UserSettings"]["TextFadeOutTime"] = LAppConfig::_TextFadeOutTime;
    config["UserSettings"]["Dialog"]["Width"] = LAppConfig::_DialogWidth;
    config["UserSettings"]["Dialog"]["Height"] = LAppConfig::_DialogHeight;
    config["UserSettings"]["Dialog"]["StyleSheet"] = LAppConfig::_DialogStyleSheet;
    config["UserSettings"]["Dialog"]["WordInterval"] = LAppConfig::_DialogWordInterval;
    config["WindowSettings"]["IconPath"] = LAppConfig::_IconPath;
    config["WindowSettings"]["AppName"] = LAppConfig::_AppName;
    config["WindowSettings"]["FPS"] = LAppConfig::_FPS;
    config["UserSettings"]["NoteOutPath"] = LAppConfig::_NoteOutPath;
    config["UserSettings"]["BgmList"]["LastPos"]["X"] = LApp::GetInstance()->GetWindow()->GetBgmListView()->x();
    config["UserSettings"]["BgmList"]["LastPos"]["Y"] = LApp::GetInstance()->GetWindow()->GetBgmListView()->y();
    config["UserSettings"]["Mlyai"]["APIKey"] = LAppConfig::_ApiKey;
    config["UserSettings"]["Mlyai"]["APISecret"] = LAppConfig::_ApiSecret;
    config["UserSettings"]["MotionInterval"] = LAppConfig::_MotionInterval;
    config["UserSettings"]["UserName"] = LAppConfig::_UserName;
    config["UserSettings"]["ShowBackground"] = LAppConfig::_ShowBackground;
    config["UserSettings"]["TransparentBackground"] = LAppConfig::_TransparentBackground;
    config["UserSettings"]["TransparentCharacter"] = LAppConfig::_TransparentCharacter;

    ofstream ofs(LAppConfig::_ConfigPath);
    if (ofs.fail())
    {
        LApp::Warning("配置文件保存失败!");
        return;
    }
    ofs << config.toStyledString().c_str();
    ofs.close();
}