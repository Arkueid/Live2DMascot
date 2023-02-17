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

	//茉莉云api
	void AskMlyai(const string& msg, string& resText)
	{
		Client cli("https://api.mlyai.com");
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
				resText = QString::fromUtf8(json["data"][0]["content"].asCString()).toStdString();
			else {
				resText = QString::fromUtf8(json["message"].asCString()).toStdString();
			}
		}
		catch (...)
		{
			resText = QString::fromLocal8Bit("网络连接有误哦~").toStdString();
		}
	}

	//自定义聊天服务器接口
	void Chat(const string& text, string& resText, string& soundPath)
	{
		Client cli(LAppConfig::_CustomChatServerHostPort);
		Headers headers = {
			{"User-Agent", "DesktopLive2D/v0.1.1"},
			{"Accept-Charset", "UTF-8"}
		};
		Params params = {
			{"Text", text}
		};
		string body;
		cli.set_read_timeout(LAppConfig::_CustomChatServerReadTimeOut);
		try {
			auto res = cli.Get(
				LAppConfig::_CustomChatServerRoute, params, headers,
				[&](const char* data, size_t data_length)->bool
				{
					body.append(data, data_length);
					return true;
				}
			);
			if (res.error() != Error::Success)
			{
				Log("ChatApi", "请求失败!");
				resText = QString::fromLocal8Bit("无法连接到服务器!").toStdString();
				soundPath.clear();
				return;
			};
			QString filename = QDateTime::currentDateTime().toString("yyyyMMddhhmmss").append(".wav");
			soundPath = QString::fromUtf8(LAppConfig::_ChatSavePath.c_str()).append("/").append(filename).toLocal8Bit().constData();
			if (!body.empty())
			{
				ofstream ofs(soundPath.c_str(), ios::binary);
				ofs << body;
				ofs.close();
			}
			else {
				soundPath.clear();
			}
			resText = res.value().get_header_value("Text");
		}
		catch (...)
		{
			resText = QString::fromLocal8Bit("网络连接错误，请检查网络状况!").toStdString();
			soundPath.clear();
		}
		
	}

}


//不实用的to do api接口
#if 0

namespace ToDoUtils
{
	Server* server = NULL;
	const char* client_id = "b86d70b9-affd-4f4c-8f65-28822dd1d5e7";
	string token;
	void CountTime()
	{
		if (server)
			Sleep(90000);
		else return;
		if (server)
			ShutDown();
		else return;
		Log("ToDoUtils", "auth time out!");
	}
	void FullFlowUpdateToken(int port)
	{
		//open the auth page in the default explorer
		system("explorer \"https://login.microsoftonline.com/common/oauth2/v2.0/authorize?client_id=b86d70b9-affd-4f4c-8f65-28822dd1d5e7&response_type=code&redirect_uri=http%3A%2F%2F127.0.0.1%3A50721&response_mode=query&scope=offline_access Tasks.Read&state=12345\"");
		//get an instance of http server to receive authorization code
		if (!server)
		{
			server = new Server();
		}
		else {
			return;
		}
		thread(CountTime).detach();
		//handler for GET request
		server->Get("/", [](const Request& req, Response& res)
			{
				//parse authorization code
				string code = req.get_param_value("code");
				if (code.empty())
				{
					Log("MS Graph Api", req.get_param_value("error_description").c_str());
					res.set_content("登录验证失败!", "text/plain");
					ShutDown();
					return;
				}
				Log("ToDoUtils", string("authorization_code is ").append(code).c_str());

				//use code to get access_token
				Client client("https://login.microsoftonline.com");
				httplib::Params data = {
					{"client_id", client_id},
					{"grant_type", "authorization_code"},
					{"code", code},
					{"redirect_uri", "http://127.0.0.1:50721"},
					{"scope", "Tasks.Read"}
				};
				Result rsp = client.Post("/common/oauth2/v2.0/token", data);
				Json::Value json;
				Json::Reader reader;
				reader.parse(rsp.value().body, json);
				if (json["access_token"].isNull())
				{
					Log("MS Graph Api", json["error_description"].asCString());
					res.set_content("登录验证失败!", "text/plain");
					ShutDown();
					return;
				}
				token = json["access_token"].asCString();
				res.set_content("登录成功，可关闭此页面。\n", "text/plain");
				json["expires_at"] = time(0) + json["expires_in"].asInt();
				ofstream ofs("token.json");
				ofs << json;
				ofs.close();
				ShutDown();
			}
		);
		server->listen("127.0.0.1", port);
	}
	void ShutDown()
	{
		if (server)
		{
			if (server->is_running()) server->stop();
			delete server;
			server = NULL;
		}
	}
	void Update()
	{
		ifstream ifs("token.json");
		bool ret = false;
		if (!ifs.good())
		{
			Log("ToDoUtils", "no local token, using full flow update...");
			FullFlowUpdateToken(50721);
			if (!token.empty())
			Log("ToDoUtils", "token updated!");
		}
		else
		{
			Json::Value tokenJson;
			ifs >> tokenJson;
			ret = !tokenJson["access_token"].isNull() && tokenJson["expires_at"].asInt() > time(0);
			if (ret)
			{
				Log("ToDoUtils", "access_token is valid, start updating task lists...");
				token = tokenJson["access_token"].asCString();
			}
			else if (tokenJson["refresh_token"].isNull())
			{
				Log("ToDoUtils", "local refresh token not found, start full-flow token update...");
				FullFlowUpdateToken(50721);
				Log("ToDoUtils", "token updated!");
			}
			else
			{
				Log("ToDoUtils", "local refresh token found, start refresh update...");
				ret = RefreshUpdateToken(tokenJson["refresh_token"].asCString());
				if (!ret)
				{
					Log("ToDoUtils", "local refresh token expired, start full-flow token update...");
					FullFlowUpdateToken(50721);
					Log("ToDoUtils", "token updated!");
				}
				else {
					Log("ToDoUtils", "token refreshed!");
				}
			}
		}
		if (ret)
		{
			GetTaskLists();
			Log("ToDoUtils", "task lists update finished!");
		}
	}
	std::string ISO8601ToLocaltime(const std::string& time)
	{
		struct std::tm time_struct;
		std::istringstream ss(time);
		ss >> std::get_time(&time_struct, "%Y-%m-%dT%H:%M:%SZ");
		std::time_t time_unix = _mkgmtime(&time_struct);
		if (time_unix == -1)
		{
			return "";
		}
		char current_time[32];
		strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", localtime(&time_unix));
		std::string local_time_str(current_time);

		return local_time_str;
	}
	bool RefreshUpdateToken(const char* refresh_token)
	{
		Client cli("https://login.microsoftonline.com");
		Params params = {
			{"scope", "Tasks.Read"},
			{"grant_type", "refresh_token"},
			{"refresh_token", refresh_token},
			{"client_id", client_id}
		};
		Result rsp = cli.Post("/common/oauth2/v2.0/token", params);
		Json::Value json;
		Json::Reader reader;
		reader.parse(rsp.value().body, json);
		if (json["access_token"].isNull())
		{
			return false;
		}
		ofstream ofs("token.json");
		json["refresh_token"].isNull() ? json["refresh_token"] = refresh_token : NULL;
		json["expires_at"] = time(0) + json["expires_in"].asInt();
		ofs << json;
		ofs.close();
		token = json["access_token"].asCString();
		return true;
	}
	bool GetTaskLists()
	{
		
		//using token to obtain task lists
		string base_url = string("/v1.0/me/todo/lists");
		Client cli("https://graph.microsoft.com");
		Headers headers = { {"authorization", token} };
		Json::Value json; Json::Reader reader;
		Result rsp = cli.Get(base_url, headers);
		reader.parse(rsp.value().body, json);
		if (json["value"].isNull())
		{
			Log("MS Graph Api", json.toStyledString().c_str());
			ShutDown();
			return false;
		}
		int size = json["value"].size();
		Json::Value taskLists;
		Json::Value batches;
		Json::Value req;
		Json::Value temp;
		for (int i = 0; i < size; i++)
		{
			req["id"] = i;
			req["method"] = "GET";
			req["url"] = string("/me/todo/lists/").append(json["value"][i]["id"].asCString()).append("/tasks");
			batches["requests"].append(req);
			req.clear();
		}
		rsp = cli.Post("/v1.0/$batch", headers, batches.toStyledString(), "application/json");
		reader.parse(rsp.value().body, batches);
		batches = batches["responses"];
		for (int i = 0; i < batches.size(); i++)
		{
			if (batches[i]["status"].asInt() == 200)
			{
				for (int j = 0; j < batches[i]["body"]["value"].size(); j++)
				{	
					if (strcmp(batches[i]["body"]["value"][j]["status"].asCString(), "completed") != 0)
					{
						taskLists.append(batches[i]["body"]["value"][j]);
					}
				}
			}
		}
		ofstream ofs("tasks.json");
		ofs << taskLists;
		ofs.close();
		return true;
	}
}

#endif