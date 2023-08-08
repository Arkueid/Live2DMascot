#pragma once
#include "sqlite3.h"
#include <iostream>
#include <vector>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
using namespace std;

struct Msg {
	string name;
	string text;
	string soundPath;
	string ct;
};

class ChatHistoryDB {
private:
	sqlite3* m_db;
	void create_chat_history();
public:
	ChatHistoryDB(const char* db_path);
	~ChatHistoryDB();
	bool IsOpen();
	void Close();
	void InsertMsg(const char* name, const char * text, const char* soundPath);
	vector<Msg> SelectByDate(const char* date=NULL);
	QVector<QString> GetDates();
};