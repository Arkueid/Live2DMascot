#pragma once
#include "interface/ILApp.h"
#include "win/GLWidget.h"
#include <QtWidgets/qapplication.h>
#include <QtCore/qthread.h>

void Log(const char* handler, const char* msg);

namespace LAppConfig {
	extern string _ConfigPath;

	extern int _WindowWidth;
	extern int _WindowHeight;
	extern int _LastPosX;
	extern int _LastPosY;
	extern int _FPS;
	extern string _AppName;
	extern string _IconPath;
	extern string _ModelName;
	extern string _ModelDir;
	extern bool _KeepQuiet;
	extern bool _MouseTrack;
	extern bool _StayOnTop;
	extern bool _NoSound;
	extern bool _ShowText;
	extern bool _ShowBgmList;

	extern int _DialogFontSize;
	extern string _DialogFontFamily;
	extern string _DialogFontColor;
	extern string _DialogBackgroundColor;
	extern int _DialogYOffset;
	extern int _DialogMaxWidth;
	extern int _DialogXPadding;
	extern int _DialogYPadding;

	extern int _ChatWidgetFontSize;
	extern string _ChatWidgetFontFamily;
	extern string _ChatWidgetFontColor;
	extern string _ChatWidgetBackgroundColor;
	extern int _ChatWidgetWidth;
	extern int _ChatWidgetHeight;

	extern int _TextFadeOutTime;
	extern int _BgmListLastPosX;
	extern int _BgmListLastPosY;
	extern float _LipSyncMagnification;
	extern string _ApiKey;
	extern string _ApiSecret;
	extern float _MotionInterval;
	extern string _UserName;
	extern bool _ShowBackground;
	extern bool _TransparentBackground;
	extern bool _MouseOn;
	extern bool _TransparentCharacter;
	
	extern bool _WaitChatResponse;
	extern double _SoundVolume;
	extern string _ChatSavePath;
	extern string _CustomChatServerHostPort;
	extern string _CustomChatServerRoute;
	extern bool _CustomChatServerOn;
	extern int _CustomChatServerReadTimeOut;
	extern bool _RepairModeOn;

	extern string _BaiduSpeechClientId;
	extern string _BaiduSpeechClientSecret;
	extern string _CustomVoiceChatRoute;
	extern bool _CustomVoiceChatOn;

	extern float _CharacterX;
	extern float _CharacterY;
	
	extern string _AssetsDir;

	extern Json::Value _Plugins;
};

class LApp: public ILApp
{
private:
	QApplication* _app;
	GLWidget* _win;
	int _holding;
public:
	LApp();
	static LApp* GetInstance();
	void SaveConfig();
	static void Warning(const char* x);
	void Initialize(int argc, char* argv[]);
	void Run();
	void Release();
	void LoadConfig();
	QApplication* GetApp();
	GLWidget* GetWindow();
	IGLWidget* GetGLWidget();
	ILAppModel* GetModel();
	string _ConfigPath() { return LAppConfig::_ConfigPath; }
	void Hold();
	void ReleaseHold();
	int	 Holding();
	int _WindowWidth() { return LAppConfig::_WindowWidth; }
	int _WindowHeight() { return LAppConfig::_WindowHeight; }
	int _LastPosX() { return LAppConfig::_LastPosX; }
	int _LastPosY() { return LAppConfig::_LastPosY; }
	int _FPS() { return LAppConfig::_FPS; }
	string _AppName() { return LAppConfig::_AppName; }
	string _IconPath() { return LAppConfig::_IconPath; }
	string _ModelName() { return LAppConfig::_ModelName; }
	string _ModelDir() { return LAppConfig::_ModelDir; }
	bool _KeepQuiet() { return LAppConfig::_KeepQuiet; }
	bool _MouseTrack() { return LAppConfig::_MouseTrack; }
	bool _StayOnTop() { return LAppConfig::_StayOnTop; }
	bool _NoSound() { return LAppConfig::_NoSound; }
	bool _ShowText() { return LAppConfig::_ShowText; }
	bool _ShowBgmList() { return LAppConfig::_ShowBgmList; }

	int _DialogFontSize() { return LAppConfig::_DialogFontSize; }
	string _DialogFontFamily() { return LAppConfig::_DialogFontFamily; }
	string _DialogFontColor() { return LAppConfig::_DialogFontColor; }
	string _DialogBackgroundColor() { return LAppConfig::_DialogBackgroundColor; }
	int _DialogYOffset() { return LAppConfig::_DialogYOffset; }
	int _DialogMaxWidth() { return LAppConfig::_DialogMaxWidth; }
	int _DialogXPadding() { return LAppConfig::_DialogXPadding; }
	int _DialogYPadding() { return LAppConfig::_DialogYPadding; }

	int _ChatWidgetFontSize() { return LAppConfig::_ChatWidgetFontSize; }
	string _ChatWidgetFontFamily() { return LAppConfig::_ChatWidgetFontFamily; }
	string _ChatWidgetFontColor() { return LAppConfig::_ChatWidgetFontColor; }
	string _ChatWidgetBackgroundColor() { return LAppConfig::_ChatWidgetBackgroundColor; }
	int _ChatWidgetWidth() { return LAppConfig::_ChatWidgetWidth; }
	int _ChatWidgetHeight() { return LAppConfig::_ChatWidgetHeight; }

	int _TextFadeOutTime() { return LAppConfig::_TextFadeOutTime; }
	int _BgmListLastPosX() { return LAppConfig::_BgmListLastPosX; }
	int _BgmListLastPosY() { return LAppConfig::_BgmListLastPosY; }
	float _LipSyncMagnification() { return LAppConfig::_LipSyncMagnification; }
	string _ApiKey() { return LAppConfig::_ApiKey; }
	string _ApiSecret() { return LAppConfig::_ApiSecret; }
	float _MotionInterval() { return LAppConfig::_MotionInterval; }
	string _UserName() { return LAppConfig::_UserName; }
	bool _ShowBackground() { return LAppConfig::_ShowBackground; }
	bool _TransparentBackground() { return LAppConfig::_TransparentBackground; }
	bool _MouseOn() { return LAppConfig::_MouseOn; }
	bool _TransparentCharacter() { return LAppConfig::_TransparentCharacter; }

	bool _WaitChatResponse() { return LAppConfig::_WaitChatResponse; }
	double _SoundVolume() { return LAppConfig::_SoundVolume; }
	string _ChatSavePath() { return LAppConfig::_ChatSavePath; }
	string _CustomChatServerHostPort() { return LAppConfig::_CustomChatServerHostPort; }
	string _CustomChatServerRoute() { return LAppConfig::_CustomChatServerRoute; }
	bool _CustomChatServerOn() { return LAppConfig::_CustomChatServerOn; }
	int _CustomChatServerReadTimeOut() { return LAppConfig::_CustomChatServerReadTimeOut; }
	bool _RepairModeOn() { return LAppConfig::_RepairModeOn; }

	string _BaiduSpeechClientId() { return LAppConfig::_BaiduSpeechClientId; }
	string _BaiduSpeechClientSecret() { return LAppConfig::_BaiduSpeechClientSecret; }
	string _CustomVoiceChatRoute() { return LAppConfig::_CustomVoiceChatRoute; }
	bool _CustomVoiceChatOn() { return LAppConfig::_CustomVoiceChatOn; }

	float _CharacterX() { return LAppConfig::_CharacterX; }
	float _CharacterY() { return LAppConfig::_CharacterY; }

	string _AssetsDir() { return LAppConfig::_AssetsDir; }

	string _Plugins() { return LAppConfig::_Plugins.toStyledString(); }
};

namespace {
	LApp* _lapp_instance = NULL;
};

