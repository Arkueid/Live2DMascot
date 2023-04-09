#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include "NetworkUtils.h"
#include <QtCore/qdir.h>



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

	bool UpdateBgmList()
	{
		Client cli("https://api.bgm.tv");
		Json::Reader reader;
		Json::Value json;
		Result rsp = cli.Get("/calendar");
		if (rsp.error() != Error::Success) return false;
		reader.parse(rsp.value().body, json);
		cli.stop();
		ofstream ofs(_BgmListJsonPath);
		ofs << json;
		ofs.close();
		return true;
	}

	void CheckUpdate()
	{
		if (ShouldUpdate())
		{
			try {
				Log("[BgmList]Update", "正在拉取番剧列表...");
				bool ret = UpdateBgmList();
				if (ret)
					Log("[BgmList]Update", string("番剧列表更新完毕: ").append(_BgmListJsonPath).c_str());
				else Log("[BgmList]Update", "番剧列表更新失败!");
			}
			catch (...)
			{
				Log("[BgmList]Exception", "无网络连接，更新已取消!");
			}
		}
		else {
			Log("[BgmList]Update", "番剧列表已是最新，无需更新！");
		}
	}
}

namespace HolidayUtils
{
	string _HolidayJsonPath = "year.json";
	const bool _DebugLogEnable = true;
	bool GetHolidayJson()
	{
		Log("[Holiday]Update", "正在拉取节日列表...");
		Client cli("https://timor.tech");
		Json::Value json;
		Json::Reader reader;
		Result res = cli.Get("/api/holiday/year");
		if (res.error() != Error::Success) return false;
		reader.parse(res.value().body, json);
		cli.stop();
		ofstream ofs(_HolidayJsonPath, ios::binary);
		ofs << json;
		ofs.close();
		return true;
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
				bool ret = GetHolidayJson();
				if (ret)
					Log("[Holiday]Update", string("节日列表更新完毕: ").append(_HolidayJsonPath).c_str());
				else Log("[Holiday]Exception", "节日列表更新失败!");
			}
			catch (...)
			{
				Log("[Holiday]Exception", "无网络连接，更新已取消!");
			}
		}
		else {
			Log("[Holiday]Update", "节日列表已是最新，无需更新！");
		}
	}

	const char* WhatsToday()
	{
		ifstream ifs(_HolidayJsonPath);
		if (ifs.fail()) return NULL;
		Json::Value json;
		ifs >> json;
		ifs.close();
		string td = QDateTime::currentDateTime().toString("MM-dd").toStdString();
		Log("[HolidayUtils]Today", td.c_str());
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
			if (res.error() != Error::Success) {
				resText = QString::fromLocal8Bit("网络连接有误哦~").toStdString();
				return;
			}
			Json::Reader reader;
			Json::Value json;
			reader.parse(res.value().body, json);
			QTextCodec* codec = QTextCodec::codecForName("gbk");
			Log("[ChatAI]Response Msg", codec->fromUnicode(json["message"].asCString()));
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
			{"User-Agent", "DesktopLive2D/0.3.0(cpp; cpp-httplib; OpenSSL)"},
			{"Accept-Charset", "UTF-8"},
			{"Accept", "audio/wav"}
		};
		Params params = {
			{"Text", text}
		};
		string body;
		//最长连接时间
		cli.set_connection_timeout(LAppConfig::_CustomChatServerReadTimeOut);
		//最长等待响应时间
		cli.set_read_timeout(LAppConfig::_CustomChatServerReadTimeOut);
		try {
			auto res = cli.Get(
				LAppConfig::_CustomChatServerRoute, params, headers,
				[&body](const char* data, size_t data_length)->bool
				{
					body.append(data, data_length);
					return true;
				}
			);
			if (res.error() != Error::Success)
			{
				Log("[ChatApi]Exception", "请求失败!");
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

	void VoiceChat(const char* filePath, string& text, string& soundPath) {
		QFile voiceFile;
		voiceFile.setFileName(filePath);
		voiceFile.open(QIODevice::ReadOnly);
		QByteArray bytes = voiceFile.readAll();
		voiceFile.close();
		if (bytes.isEmpty()) {
#ifdef CONSOLE_FLAG
			printf("[VoiceInput]File opening error: %s\n", filePath);
#endif // CONSOLE_FLAG
			text = QString::fromLocal8Bit("你说啥?").toUtf8().constData();
			soundPath = "";
			return;
		}

		Json::Value json;
		json["Voice"] = bytes.toBase64().toStdString();

		httplib::Headers headers = {
			{"User-Agent", "DesktopLive2D/0.3.0(cpp; cpp-httplib; OpenSSL)"},
			{"Accept", "application/json"}
		};
		Client client(LAppConfig::_CustomChatServerHostPort);
		client.set_read_timeout(LAppConfig::_CustomChatServerReadTimeOut);
		Result rsp = client.Post(LAppConfig::_CustomVoiceChatRoute, 
			headers, 
			json.toStyledString(),
			"application/json"
		);
		if (rsp.error() != Error::Success) {
#ifdef CONSOLE_FLAG
			printf("[VoiceInput]Resopnse receiving error\n");
#endif // CONSOLE_FLAG
			text = QString::fromLocal8Bit("没有收到回复~").toUtf8().constData();
			soundPath = "";
			return;
		}
#ifdef CONSOLE_FLAG
		printf("[VoiceInput]Msg received success\n");
#endif // CONSOLE_FLAG
		Json::Value res;
		Json::Reader reader;
		reader.parse(rsp.value().body, res);
		printf("%s\n", res.toStyledString().c_str());
		if (res["Text"].isNull()) {
			text = QString::fromLocal8Bit("什么也没听到~").toUtf8().constData();
		}else text = res["Text"].asCString();
		if (!res["Sound"].isNull()) {
			QString filename = QDateTime::currentDateTime().toString("yyyyMMddhhmmss").append(".wav");
			soundPath = QString::fromUtf8(LAppConfig::_ChatSavePath.c_str()).append("/").append(filename).toLocal8Bit().constData();
			ofstream ofs(soundPath.c_str(), ios::binary);
			if (ofs.is_open()) {
				ofs << QByteArray::fromBase64(res["Sound"].asCString()).toStdString();
				ofs.close();
			}
			ofs.close();
		}
		else soundPath = "";
	}

}


/**
* @brief VoiceInputUtils 语音输入
*/
namespace {
	//token
	std::string _token;
	bool stopped = false;
	bool available = true;
	bool hasRecord = false;
}

void VoiceInputUtils::Record() {
	available = false;
	stopped = false;
	hasRecord = false;
	if (audioRecorder != nullptr) {
		audioRecorder->stop();
		audioRecorder->deleteLater();
		audioRecorder = nullptr;
	}

	QAudioEncoderSettings audioSettings;
	audioSettings.setCodec("audio/pcm");
	audioSettings.setQuality(QMultimedia::HighQuality);
	audioSettings.setChannelCount(1);
	audioSettings.setSampleRate(16000);
	QDir dir(LAppConfig::_ChatSavePath.c_str());
	if (!dir.exists()) dir.mkpath(".");
	QString path = dir.absolutePath().append("/voice-input-temp.wav");
	if (!stopped)
		audioRecorder = new QAudioRecorder;
	if (!stopped)
		audioRecorder->setEncodingSettings(audioSettings);

#ifdef CONSOLE_FLAG
	printf("[VoiceInput]Voice cached: %s\n", path.toStdString().c_str());
#endif // CONSOLE_FLAG

	if (!stopped)
		audioRecorder->setOutputLocation(QUrl::fromLocalFile(path));
	if (!stopped) {
		audioRecorder->record();
		printf("[VoiceInput]Start recording...\n");
		hasRecord = true;
	}

	if (!stopped)
	{
		loop = new QEventLoop;
		loop->exec();
	}

	if (audioRecorder != nullptr)
	{
		audioRecorder->stop();
		printf("[VoiceInput]Stop recording.\n");
		audioRecorder->deleteLater();
	}
	if (loop != nullptr) 
	{
		if (loop->isRunning()) loop->exit();
		loop->deleteLater();
	}
	audioRecorder = nullptr;
	loop = nullptr;
	available = true;
}

void VoiceInputUtils::Stop() {
	stopped = true;
	if (loop != nullptr) {
		if (loop->isRunning())
			loop->exit();
	}
}

bool VoiceInputUtils::IsAvailable() {
	return available;
}

bool VoiceInputUtils::HasRecord() {
	return hasRecord;
}

/**
* @brief 发送录音文件到语音识别服务器
*/
const char* VoiceInputUtils::DetectSpeech(const char* filePath) {
#ifdef CONSOLE_FLAG
	time_t start = time(0);
#endif // CONSOLE_FLAG

	httplib::Client client("http://vop.baidu.com");
	QFile file;
	file.setFileName(filePath);
	file.open(QIODevice::ReadOnly);
	QByteArray bytes = file.readAll();
	file.close();
	if (bytes.isEmpty()) {
#ifdef CONSOLE_FLAG
		printf("[VoiceInput]Empty record\n");
#endif // CONSOLE_FLAG
		return "";
	}
	std::string scope = std::string("/server_api");
	Json::Value json;
	json["format"] = "wav";
	json["rate"] = 16000;
	json["channel"] = 1;
	json["cuid"] = "admin-arkueid-0d000721";
	json["token"] = _token;
	json["dev_pid"] = 1537;
	json["len"] = bytes.length();
	json["speech"] = bytes.toBase64().toStdString();
	auto rsp = client.Post(scope, json.toStyledString(), "application/json");
	if (rsp.error() != httplib::Error::Success) {
#ifdef CONSOLE_FLAG
		printf("[VoiceInput]Response receiving error\n");
#endif // CONSOLE_FLAG
		return QString::fromLocal8Bit("无法接入百度语音识别服务").toUtf8().constData();
	}
	Json::Value res;
	Json::Reader reader;
	reader.parse(rsp.value().body, res);
	if (res["result"][0].isNull()) {
#ifdef CONSOLE_FLAG
		printf("[VoiceInput]Speech recognizing Fail\n");
		printf("[VoiceInput]Response: %s", rsp.value().body.c_str());
#endif // CONSOLE_FLAG

		return QString::fromLocal8Bit("语音识别失败").toUtf8().constData();
	}
#ifdef CONSOLE_FLAG
	printf("[VoiceInput]Speech recognition (%llds): %s\n", time(0) - start, QString::fromUtf8(res["result"][0].asCString()).toLocal8Bit().constData());
#endif // CONSOLE_FLAG
	return res["result"][0].asCString();
}

/**
* @brief 发送网络请求获取token
*/
void VoiceInputUtils::GetToken() {
	httplib::Client client("https://aip.baidubce.com");
	string scope = "/oauth/2.0/token";
	httplib::Headers headers = {
		{"User-Agent", "DesktopLive2D/0.3.0(cpp; cpp-httplib; OpenSSL)"}
	};
	httplib::Params params = {
		{"grant_type", "client_credentials"},
		{"client_id", LAppConfig::_BaiduSpeechClientId},
		{"client_secret", LAppConfig::_BaiduSpeechClientSecret}
	};
	auto rsp = client.Post(scope, headers, params);
	if (rsp.error() != httplib::Error::Success) {
#ifdef CONSOLE_FLAG
		printf("[VoiceInputUtils]Resopnse receiving error\n");
#endif // CONSOLE_FLAG
		return;
	}

	Json::Value token;
	Json::Reader reader;

	reader.parse(rsp.value().body, token);

	token["expires_in"].isNull() ? NULL : token["expires_at"] = time(0) + token["expires_in"].asInt64();

	std::ofstream ofs("baidu.speech.token.json");
	if (ofs.fail()) {
#ifdef CONSOLE_FLAG
		printf("[VloiceInput]Token saving error:\n%s\n", token.toStyledString().c_str());
#endif // CONSOLE_FLAG
		ofs.close();
		return;
	}
	ofs << token;
	ofs.close();

#ifdef CONSOLE_FLAG
	printf("[VoiceInput]Speech token update: baidu.speech.token.json\n");
#endif // CONSOLE_FLAG

	if (token["access_token"].isNull())
		return;
	else
		_token = token["access_token"].asCString();
}


/**
* @brief 检查是否需要更新token
*/
bool VoiceInputUtils::ShouldUpdateToken() {
	std::ifstream ifs("baidu.speech.token.json");
	if (ifs.fail()) {
		ifs.close();
		return true;
	}
	Json::Value token;
	ifs >> token;
	ifs.close();
	if (token["expires_at"].isNull()) return true;
	else if (token["access_token"].isNull()) return true;
	else if (token["expires_at"].asInt64() < time(0)) return true;
	return false;
}

/**
* @brief 检测本地token是否过期，过期就更新
*/
void VoiceInputUtils::CheckUpdate() {
	if (ShouldUpdateToken())
		GetToken();
	else {
		GetLocalToken();
#ifdef CONSOLE_FLAG
		printf("[VoiceInput]Speech token found in local file\n");
#endif // CONSOLE_FLAG

	}
}


/**
* @brief 读取本地储存的百度语音识别token
*/
void VoiceInputUtils::GetLocalToken() {
	std::ifstream ifs("baidu.speech.token.json");
	if (ifs.fail()) return GetToken();
	Json::Value token;
	ifs >> token;
	ifs.close();
	_token = token["access_token"].asCString();
}

