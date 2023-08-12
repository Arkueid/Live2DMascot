#include "RandomTalk.h"
#include "json/json.h"
#include "LApp.h"
#include <fstream>
#include <time.h>
#include <QtCore/qdir.h>

namespace RandomTalk {
	string GetRandomSentence() {
		QDir dir("assets/sentences");
		QFileInfoList ls = dir.entryInfoList(QDir::Files);
		ifstream ifs("assets/sentences/" + ls[rand() % ls.size()].fileName().toStdString());
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
