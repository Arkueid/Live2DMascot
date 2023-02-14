#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include "NetworkUtils.h"
#include <QtCore/qtextcodec.h>
#include <QtCore/qdatetime.h>


using namespace std;
using namespace httplib;

void Log(const char* handler, const char* msg)
{
	if (LAppDefine::DebugLogEnable) printf("[APP]%s: %s\n", handler, msg);
}

namespace BgmListUtils {

	string _BgmListJsonPath = "bgmlist.json";  //番剧列表路径

	//根据当前系统时间判断是否应该更新
	bool ShouldUpdate()
	{
		time_t t;
		time(&t);
		struct tm* now = localtime(&t);
		int weeks[7] = { 6, 0, 1, 2, 3, 4, 5};
		_BgmListJsonPath = string("bangumi.").append(to_string(now->tm_year + 1900)).append(to_string(now->tm_yday - weeks[now->tm_wday])).append(".json");
		if (_access(_BgmListJsonPath.c_str(), 0)==-1) return true;
		return false;
	}

	void UpdateBgmList()
	{
		Client cli("https://api.bgm.tv");
		Json::Reader reader;
		Json::Value json;
		cli.set_proxy("", -1);
		Result rsp = cli.Get("/calendar");
		reader.parse(rsp.value().body, json);
		cli.stop();
		ofstream ofs(_BgmListJsonPath);
		ofs << json;
		ofs.close();
	}

	void CheckUpdate()
	{
		if (ShouldUpdate())
		{
			try {
				Log("BgmListUtils", "正在拉取番剧列表...");
				UpdateBgmList();
				Log("BgmListUtils", string("番剧列表更新完毕: ").append(_BgmListJsonPath).c_str());
			}
			catch (...)
			{
				Log("UpdateBgmList", "无网络连接，更新已取消!");
			}
		}
		else {
			Log("BgmListUtils", "番剧列表已是最新，无需更新！");
		}
	}
}

namespace HolidayUtils
{
	string _HolidayJsonPath = "year.json";
	const bool _DebugLogEnable = true;
	void GetHolidayJson()
	{
		Log("BgmListUtils", "正在拉取节日列表...");
		Client cli("https://timor.tech");
		Json::Value json;
		Json::Reader reader;
		cli.set_proxy("", -1);
		Result res = cli.Get("/api/holiday/year");
		reader.parse(res.value().body, json);
		cli.stop();
		ofstream ofs(_HolidayJsonPath, ios::binary);
		ofs << json;
		ofs.close();
	}

	bool ShouldUpdate()
	{
		time_t t;
		time(&t);
		struct tm* now = localtime(&t);
		_HolidayJsonPath = string("holiday.") + to_string(now->tm_year + 1900).append(".json");
		if (_access(_HolidayJsonPath.c_str(), 0)==-1) return true;
		return false;
	}

	void CheckUpdate()
	{
		if (ShouldUpdate())
		{
			try {
				GetHolidayJson();
				Log("HolidayUtils", string("节日列表更新完毕: ").append(_HolidayJsonPath).c_str());
			}
			catch (...)
			{
				Log("HolidayUtils", "无网络连接，更新已取消!");
			}
		}
		else {
			Log("HolidayUtils", "节日列表已是最新，无需更新！");
		}
	}

	const char* WhatsToday()
	{
		ifstream ifs(_HolidayJsonPath);
		Json::Value json;
		ifs >> json;
		ifs.close();
		string td = QDateTime::currentDateTime().toString("MM-dd").toStdString();
		if (!json["holiday"][td].isNull() && json["holiday"][td]["holiday"].asBool())
		{
			return json["holiday"][td]["name"].asCString();
		}
		return NULL;
	}
}

namespace ChatAPI {
	Client cli("https://api.mlyai.com");
	const char* AskMlyai(const char* msg)
	{
		Json::Value data;
		data["content"] = msg;
		data["type"] = 1;
		data["from"] = LAppConfig::_UserName;
		try {
			Result res = cli.Post("/reply", { {"Api-Key", LAppConfig::_ApiKey}, {"Api-Secret", LAppConfig::_ApiSecret} }, data.toStyledString().c_str(), "application/json; charset=UTF-8");
			Json::Reader reader;
			Json::Value json;
			reader.parse(res.value().body, json);
			QTextCodec* codec = QTextCodec::codecForName("gbk");
			Log("ChatAI", codec->fromUnicode(json["message"].asCString()));
			if (strcmp(json["code"].asCString(), "00000") == 0)
				return codec->fromUnicode(json["data"][0]["content"].asCString());
			else {
				return codec->fromUnicode(json["message"].asCString());
			}
		}
		catch (...)
		{
			return "网络连接有错误哦~";
		}
	}
}