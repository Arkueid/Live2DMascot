#pragma once
#include <iostream>
#include <fstream>
#include "json/json.h"
#include "LAppDefine.hpp"
#include "LApp.h"
#include <QtMultimedia/qaudioinput.h>
#include <QtCore/qfile.h>
#include <vector>
#include <QtCore/qtextcodec.h>
#include <QtCore/qdatetime.h>


namespace BgmListUtils
{
	extern std::string _BgmListJsonPath;
	bool ShouldUpdate();
	bool UpdateBgmList();
	void CheckUpdate();
}

namespace HolidayUtils
{
	bool GetHolidayJson();
	void CheckUpdate();
	bool ShouldUpdate();
	const char* WhatsToday();
}

namespace ChatAPI 
{
	void AskMlyai(const string& msg, string& resText);
	void Chat(const string& text, string& resText, string& soundPath);  //自定义聊天
	void VoiceChat(const char* filePath, string& text, string& soundPath);
}

/**
* 语音输入，需要联网
*/
#include <QtMultimedia/qaudiorecorder.h>

namespace {
	QAudioRecorder* audioRecorder = nullptr;
	QEventLoop* loop = nullptr;
}
namespace VoiceInputUtils {
	void Record();
	void Stop();
	const char* DetectSpeech(const char* filePath);
	void GetToken();
	void GetLocalToken();
	bool ShouldUpdateToken();
	void CheckUpdate();
	bool IsAvailable();
	bool HasRecord();
}