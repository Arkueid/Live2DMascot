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
#include <QtCore/qtimer.h>
using namespace std;
using namespace LAppDefine;

namespace LAppConfig {
    const char* _ConfigPath;  //设置默认读取路径，不可更改
    //窗口设置
    int _WindowWidth;
    int _WindowHeight;
    int _LastPosX;
    int _LastPosY;
    int _FPS;

    //模型设置
    string _ModelDir;
    string _ModelName;
    float _MotionInterval;
    float _LipSyncMagnification;

    //用户设置
    string _AppName;
    string _UserName;
    string _IconPath;
    bool _KeepQuiet;
    bool _MouseTrack;
    bool _StayOnTop;
    bool _NoSound;
    bool _ShowBgmList;
    bool _ShowText;
    int _TextFadeOutTime;
    int _BgmListLastPosX;
    int _BgmListLastPosY;
    bool _ShowBackground;
    bool _TransparentBackground;
    bool _TransparentCharacter;
    //dialog设置
    int _DialogFontSize;
    string _DialogFontFamily;
    string _DialogFontColor;
    string _DialogBackgroundColor;
    int _DialogYOffset;
    int _DialogMaxWidth;
    int _DialogXPadding;
    int _DialogYPadding;
    //自定义聊天接口
    string _CustomChatServerHostPort;
    string _CustomChatServerRoute;
    bool _CustomChatServerOn;
    int _CustomChatServerReadTimeOut;  //等待响应的最长时间
    //聊天保存路径
    string _ChatSavePath;
    //茉莉云api
    string _ApiKey;
    string _ApiSecret;
    //瞬时状态
    bool _WaitChatResponse;
    bool _MouseOn;
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

void LApp::Initialize(QApplication* app)
{
    _app = app;
    //_app->setQuitOnLastWindowClosed(true);
    QObject::connect(qApp, &QGuiApplication::commitDataRequest, [&](QSessionManager& manager) {
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
        catch (...)
        {
            LApp::Warning("Json配置文件格式有误！");
            file.close();
            LApp::GetInstance()->GetApp()->exit(-1);
        }
    }
    file.close();

    LAppConfig::_ModelDir = !config["ModelSettings"]["ModelDir"].isNull() ? config["ModelSettings"]["ModelDir"].asCString() : "Resources";

    const char* path = QString::fromUtf8(LAppConfig::_ModelDir.c_str()).toLocal8Bit();
    Log("Resource Dir", path);
    if (_access(path, 0) != 0) {
        LApp::Warning("资源文件夹路径不正确！\n请修改config.json文件");
        exit(0);
    }
    LAppConfig::_ModelName = !config["ModelSettings"]["ModelName"].isNull() ? QString::fromUtf8(config["ModelSettings"]["ModelName"].asCString()).toStdString() : "Hiyori";
    
    path = QString::fromUtf8(string(LAppConfig::_ModelDir).append("/").append(LAppConfig::_ModelName).c_str()).toLocal8Bit();
    if (_access(path, 0) == -1)
    {
        LApp::Warning("模型文件不存在！\n请修改config.json文件");
        exit(0);
    }
    LAppConfig::_MotionInterval = !config["ModelSettings"]["MotionInterval"].isNull() ? config["ModelSettings"]["MotionInterval"].asInt() : 5;
    LAppConfig::_LipSyncMagnification = !config["ModelSettings"]["LipSyncMagnification"].isNull() ? config["ModelSettings"]["LipSyncMagnification"].asFloat() : 1.1;


    LAppConfig::_WindowWidth = !config["WindowSettings"]["Width"].isNull() ? config["WindowSettings"]["Width"].asInt() : 500;
    LAppConfig::_WindowHeight = !config["WindowSettings"]["Height"].isNull() ? config["WindowSettings"]["Height"].asInt() : 700;
    LAppConfig::_LastPosX = !config["WindowSettings"]["LastPos"]["X"].isNull() ? config["WindowSettings"]["LastPos"]["X"].asInt() : 0;
    LAppConfig::_LastPosY = !config["WindowSettings"]["LastPos"]["Y"].isNull() ? config["WindowSettings"]["LastPos"]["Y"].asInt() : 0;

    LAppConfig::_BgmListLastPosX = !config["BangumiView"]["LastPos"]["X"].isNull() ? config["BangumiView"]["LastPos"]["X"].asInt() : 500;
    LAppConfig::_BgmListLastPosY = !config["BangumiView"]["LastPos"]["Y"].isNull() ? config["BangumiView"]["LastPos"]["Y"].asInt() : 0;

    LAppConfig::_UserName = !config["UserSettings"]["UserName"].isNull() ? config["UserSettings"]["UserName"].asCString() : "User0721";
    LAppConfig::_FPS = !config["UserSettings"]["FPS"].isNull() ? config["UserSettings"]["FPS"].asInt() : 48;
    LAppConfig::_MouseTrack = !config["UserSettings"]["MouseTrack"].isNull() ? config["UserSettings"]["MouseTrack"].asBool() : true;
    LAppConfig::_IconPath = !config["UserSettings"]["IconPath"].isNull() ? config["UserSettings"]["IconPath"].asCString() : "";
    LAppConfig::_AppName = !config["UserSettings"]["AppName"].isNull() ? config["UserSettings"]["AppName"].asCString() : "Live2D Displayer";
    LAppConfig::_KeepQuiet = !config["UserSettings"]["KeepQuiet"].isNull() ? config["UserSettings"]["KeepQuiet"].asBool() : false;
    LAppConfig::_StayOnTop = !config["UserSettings"]["StayOnTop"].isNull() ? config["UserSettings"]["StayOnTop"].asBool() : false;
    LAppConfig::_TransparentCharacter = !config["UserSettings"]["TransparentCharacter"].isNull() ? config["UserSettings"]["TransparentCharacter"].asBool() : false;
    LAppConfig::_ShowBackground = !config["UserSettings"]["ShowBackground"].isNull() ? config["UserSettings"]["ShowBackground"].asBool() : false;
    LAppConfig::_TransparentBackground = !config["UserSettings"]["TransparentBackground"].isNull() ? config["UserSettings"]["TransparentBackground"].asBool() : true;
    LAppConfig::_NoSound = !config["UserSettings"]["NoSound"].isNull() ? config["UserSettings"]["NoSound"].asBool() : false;
    LAppConfig::_ShowText = !config["UserSettings"]["ShowText"].isNull() ? config["UserSettings"]["ShowText"].asBool() : true;
    LAppConfig::_ShowBgmList = !config["UserSettings"]["ShowBgmList"].isNull() ? config["UserSettings"]["ShowBgmList"].asBool() : true;
    LAppConfig::_TextFadeOutTime = !config["UserSettings"]["TextFadeOutTime"].isNull() ? config["UserSettings"]["TextFadeOutTime"].asInt() : 6;

    LAppConfig::_DialogFontSize = !config["Dialog"]["FontSize"].isNull() ? config["Dialog"]["FontSize"].asInt() : 10;
    LAppConfig::_DialogFontFamily = !config["Dialog"]["FontFamily"].isNull() ? config["Dialog"]["FontFamily"].asCString() : "Comic Sans MS";
    LAppConfig::_DialogFontColor = !config["Dialog"]["FontColor"].isNull() ? config["Dialog"]["FontColor"].asCString() : "white";
    LAppConfig::_DialogBackgroundColor = !config["Dialog"]["BackgroundColor"].isNull() ? config["Dialog"]["BackgroundColor"].asCString() : "rgba(0, 0, 0, 200)";
    LAppConfig::_DialogYOffset = !config["Dialog"]["YOffset"].isNull() ? config["Dialog"]["YOffset"].asInt() : -10;
    LAppConfig::_DialogMaxWidth = !config["Dialog"]["MaxWidth"].isNull() ? config["Dialog"]["MaxWidth"].asInt() : 370;
    LAppConfig::_DialogXPadding = !config["Dialog"]["XPadding"].isNull() ? config["Dialog"]["XPadding"].asInt() : 10;
    LAppConfig::_DialogYPadding = !config["Dialog"]["YPadding"].isNull() ? config["Dialog"]["YPadding"].asInt() : 10;
    
    LAppConfig::_ApiKey = !config["ChatAPI"]["Mlyai"]["APIKey"].isNull() ? config["ChatAPI"]["Mlyai"]["APIKey"].asCString() : "82wmm51s1bskwft3";
    LAppConfig::_ApiSecret = !config["ChatAPI"]["Mlyai"]["APISecret"].isNull() ? config["ChatAPI"]["Mlyai"]["APISecret"].asCString() : "o0vp8k7e";
    LAppConfig::_CustomChatServerOn = !config["ChatAPI"]["CustomChatServer"]["On"].isNull() ? config["ChatAPI"]["CustomChatServer"]["On"].asBool() : false;
    LAppConfig::_CustomChatServerHostPort = !config["ChatAPI"]["CustomChatServer"]["HostPort"].isNull() ? config["ChatAPI"]["CustomChatServer"]["HostPort"].asCString() : "";
    LAppConfig::_CustomChatServerRoute = !config["ChatAPI"]["CustomChatServer"]["Route"].isNull() ? config["ChatAPI"]["CustomChatServer"]["Route"].asCString() : "";
    LAppConfig::_ChatSavePath = !config["ChatAPI"]["ChatSavePath"].isNull() ? config["ChatAPI"]["ChatSavePath"].asCString() : "chat";
    LAppConfig::_CustomChatServerReadTimeOut = !config["ChatAPI"]["CustomChatServer"]["ReadTimeOut"].isNull() ? config["ChatAPI"]["CustomChatServer"]["ReadTimeOut"].asInt() : 10;
}

void LApp::Run()
{
    _win->Run();
    _app->exec();
    if (LAppDefine::DebugLogEnable)
    printf("App Quit\n");
}

void LApp::Release()
{
    LAppDelegate::GetInstance()->Release();
    _app->quit();
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

    config["ModelSettings"]["ModelDir"] = LAppConfig::_ModelDir;
    config["ModelSettings"]["ModelName"] = LAppConfig::_ModelName;
    config["ModelSettings"]["MotionInterval"] = LAppConfig::_MotionInterval;
    config["ModelSettings"]["LipSyncMagnification"] = LAppConfig::_LipSyncMagnification;

    config["UserSettings"]["FPS"] = LAppConfig::_FPS;
    config["UserSettings"]["TextFadeOutTime"] = LAppConfig::_TextFadeOutTime;
    config["UserSettings"]["IconPath"] = LAppConfig::_IconPath;
    config["UserSettings"]["AppName"] = LAppConfig::_AppName;
    config["UserSettings"]["UserName"] = LAppConfig::_UserName;
    config["UserSettings"]["ShowBackground"] = LAppConfig::_ShowBackground;
    config["UserSettings"]["TransparentBackground"] = LAppConfig::_TransparentBackground;
    config["UserSettings"]["TransparentCharacter"] = LAppConfig::_TransparentCharacter;
    config["UserSettings"]["MouseTrack"] = LAppConfig::_MouseTrack;
    config["UserSettings"]["KeepQuiet"] = LAppConfig::_KeepQuiet;
    config["UserSettings"]["StayOnTop"] = LAppConfig::_StayOnTop;
    config["UserSettings"]["NoSound"] = LAppConfig::_NoSound;
    config["UserSettings"]["ShowText"] = LAppConfig::_ShowText;
    config["UserSettings"]["ShowBgmList"] = LAppConfig::_ShowBgmList;

    config["BangumiView"]["LastPos"]["X"] = LApp::GetInstance()->GetWindow()->GetBgmListView()->x();
    config["BangumiView"]["LastPos"]["Y"] = LApp::GetInstance()->GetWindow()->GetBgmListView()->y();

    config["Dialog"]["FontSize"] = LAppConfig::_DialogFontSize;
    config["Dialog"]["FontFamily"] = LAppConfig::_DialogFontFamily;
    config["Dialog"]["FontColor"] = LAppConfig::_DialogFontColor;
    config["Dialog"]["BackgroundColor"] = LAppConfig::_DialogBackgroundColor;
    config["Dialog"]["YOffset"] = LAppConfig::_DialogYOffset;
    config["Dialog"]["MaxWdith"] = LAppConfig::_DialogMaxWidth;
    config["Dialog"]["XPadding"] = LAppConfig::_DialogXPadding;
    config["Dialog"]["YPadding"] = LAppConfig::_DialogYPadding;

    config["ChatAPI"]["CustomChatServer"]["On"] = LAppConfig::_CustomChatServerOn;
    config["ChatAPI"]["CustomChatServer"]["HostPort"] = LAppConfig::_CustomChatServerHostPort;
    config["ChatAPI"]["CustomChatServer"]["Route"] = LAppConfig::_CustomChatServerRoute;
    config["ChatAPI"]["CustomChatServer"]["ReadTimeOut"] = LAppConfig::_CustomChatServerReadTimeOut;
    config["ChatAPI"]["Mlyai"]["APIKey"] = LAppConfig::_ApiKey;
    config["ChatAPI"]["Mlyai"]["APISecret"] = LAppConfig::_ApiSecret;
    config["ChatAPI"]["ChatSavePath"] = LAppConfig::_ChatSavePath;

    ofstream ofs(LAppConfig::_ConfigPath);
    if (ofs.fail())
    {
        LApp::Warning("配置文件保存失败!");
        return;
    }
    ofs << config.toStyledString().c_str();
    ofs.close();
}