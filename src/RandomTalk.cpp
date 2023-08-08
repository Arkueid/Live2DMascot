#include "RandomTalk.h"
#include "json/json.h"
#include "LApp.h"
#include <fstream>
#include <time.h>

namespace RandomTalk {
	string GetRandomSentence() {
		char c = rand() % 12 + 97;
		ifstream ifs((string(LAppConfig::_AssetsDir).append("/sentences/") + c).append(".json"));
		if (ifs.fail()) {
			return "Failed to get random sentence";
		}
		Json::Value json;
		ifs >> json;
		ifs.close();
		int idx = rand() % json.size();
		return json[idx]["hitokoto"].asCString();
	}
}
