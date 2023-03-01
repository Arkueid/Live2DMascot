#pragma once
#include <iostream>
#include <fstream>
#include "json/json.h"
#include "LAppDefine.hpp"
#include "LApp.h"


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
	struct ChatAPIResponse {
		const string text;
		const string soundPath;
	};
	void AskMlyai(const string& msg, string& resText);
	void Chat(const string& text, string& resText, string& soundPath);  //自定义聊天
}
#if 0
namespace ToDoUtils
{
	void Update();
	void ShutDown();
	void CheckUpdate();
	bool GetTaskLists();
	bool RefreshUpdateToken(const char* refresh_token);
	void FullFlowUpdateToken(int port);
	std::string ISO8601ToLocaltime(const string& time);
}
#endif