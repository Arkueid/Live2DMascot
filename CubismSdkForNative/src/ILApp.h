#pragma once
#include "ILAppModel.h"
#include "IGLWidget.h"
#include "json/json.h"
#include <iostream>
using namespace std;

class ILApp {
public:
	virtual IGLWidget* GetGLWidget() = 0;
	virtual ILAppModel* GetModel() = 0;
	virtual void Hold() = 0;
	virtual void ReleaseHold() = 0;
	virtual int Holding() = 0;

	virtual string _ConfigPath() = 0;

	virtual int _WindowWidth() = 0;
	virtual int _WindowHeight() = 0;
	virtual int _LastPosX() = 0;
	virtual int _LastPosY() = 0;
	virtual int _FPS() = 0;
	virtual string _AppName() = 0;
	virtual string _IconPath() = 0;
	virtual string _ModelName() = 0;
	virtual string _ModelDir() = 0;
	virtual bool _KeepQuiet() = 0;
	virtual bool _MouseTrack() = 0;
	virtual bool _StayOnTop() = 0;
	virtual bool _NoSound() = 0;
	virtual bool _ShowText() = 0;
	virtual bool _ShowBgmList() = 0;

	virtual int _DialogFontSize() = 0;
	virtual string _DialogFontFamily() = 0;
	virtual string _DialogFontColor() = 0;
	virtual string _DialogBackgroundColor() = 0;
	virtual int _DialogYOffset() = 0;
	virtual int _DialogMaxWidth() = 0;
	virtual int _DialogXPadding() = 0;
	virtual int _DialogYPadding() = 0;

	virtual int _ChatWidgetFontSize() = 0;
	virtual string _ChatWidgetFontFamily() = 0;
	virtual string _ChatWidgetFontColor() = 0;
	virtual string _ChatWidgetBackgroundColor() = 0;
	virtual int _ChatWidgetWidth() = 0;
	virtual int _ChatWidgetHeight() = 0;

	virtual int _TextFadeOutTime() = 0;
	virtual int _BgmListLastPosX() = 0;
	virtual int _BgmListLastPosY() = 0;
	virtual float _LipSyncMagnification() = 0;
	virtual string _ApiKey() = 0;
	virtual string _ApiSecret() = 0;
	virtual float _MotionInterval() = 0;
	virtual string _UserName() = 0;
	virtual bool _ShowBackground() = 0;
	virtual bool _TransparentBackground() = 0;
	virtual bool _MouseOn() = 0;
	virtual bool _TransparentCharacter() = 0;

	virtual bool _WaitChatResponse() = 0;
	virtual double _SoundVolume() = 0;
	virtual string _ChatSavePath() = 0;
	virtual string _CustomChatServerHostPort() = 0;
	virtual string _CustomChatServerRoute() = 0;
	virtual bool _CustomChatServerOn() = 0;
	virtual int _CustomChatServerReadTimeOut() = 0;
	virtual bool _RepairModeOn() = 0;

	virtual string _BaiduSpeechClientId() = 0;
	virtual string _BaiduSpeechClientSecret() = 0;
	virtual string _CustomVoiceChatRoute() = 0;
	virtual bool _CustomVoiceChatOn() = 0;

	virtual float _CharacterX() = 0;
	virtual float _CharacterY() = 0;

	virtual string _AssetsDir() = 0;

	virtual Json::Value _Plugins() = 0;
};