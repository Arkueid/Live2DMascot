#pragma once
#include <iostream>
#include <fstream>
#include "json/json.h"
#include "NetworkUtils.h"
#include "LAppDefine.hpp"
#include "LApp.h"

namespace BgmListUtils
{
	extern std::string _BgmListJsonPath;
	bool ShouldUpdate();
	void UpdateBgmList();
	void CheckUpdate();
}

namespace HolidayUtils
{
	void GetHolidayJson();
	void CheckUpdate();
	bool ShouldUpdate();
	const char* WhatsToday();
}

namespace ChatAPI 
{
	const char* AskMlyai(const char* msg);
}