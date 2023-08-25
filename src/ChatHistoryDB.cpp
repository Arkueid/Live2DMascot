#include "ChatHistoryDB.h"
#include <iostream>
#include <string.h>
#include <QtCore/qdir.h>
using namespace std;

ChatHistoryDB::ChatHistoryDB(const char* db_path) {
	QDir dir("chat");
	if (!dir.exists()) 
	{
		dir.mkpath(".");
	}
	if (0 != sqlite3_open(db_path, &m_db)) {
		m_db = NULL;
	}
	create_chat_history();
}

void ChatHistoryDB::create_chat_history() {
	if (SQLITE_OK != sqlite3_exec(m_db, "create table if not exists chat_history("
		"id integer primary key autoincrement,"
		"name text,"
		"text text,"
		"soundPath text,"
		"ct text);",
		NULL, NULL, NULL) )
	{
#ifdef CONSOLE_FLAG
		cerr << "[ChatHistoryDB]:" << sqlite3_errmsg(m_db) << endl;
#endif // CONSOLE_FLAG
	}
}

ChatHistoryDB::~ChatHistoryDB() {

}

bool ChatHistoryDB::IsOpen() {
	return m_db != NULL;
}

void ChatHistoryDB::Close() {
	if (m_db) sqlite3_close(m_db);
}

void ChatHistoryDB::InsertMsg(const char* name, const char* text, const char* soundPath) {
	const char* stmt_str = "insert into chat_history(name, text, soundPath, ct) values(?,?,?,datetime('now', 'localtime'));";
	sqlite3_stmt* stmt = NULL;
	if (SQLITE_OK != sqlite3_prepare_v2(m_db, stmt_str, strlen(stmt_str), &stmt, NULL)) {
#ifdef CONSOLE_FLAG
		cerr << "[ChatHistoryDB]:" << sqlite3_errmsg(m_db) << endl;
#endif // CONSOLE_FLAG
		return;
	}
	sqlite3_bind_text(stmt, 1, name, strlen(name), NULL);
	sqlite3_bind_text(stmt, 2, text, strlen(text), NULL);
	sqlite3_bind_text(stmt, 3, soundPath, strlen(soundPath), NULL);
	if (SQLITE_DONE != sqlite3_step(stmt)) {
#ifdef CONSOLE_FLAG
		cerr << sqlite3_errmsg(m_db) << endl;
#endif // CONSOLE_FLAG
	}
	sqlite3_finalize(stmt);
}

vector<Msg> ChatHistoryDB::SelectByDate(const char* date) {
	vector<Msg> msg_ls;
	sqlite3_stmt* stmt = NULL;
	const char* sql;
	if (date) {
		sql = "select id,name,text,soundPath,ct from chat_history where date(ct)=? order by datetime(ct) asc";
	}
	else {
		sql = "select id,name,text,soundPath,ct from chat_history order by datetime(ct) asc";
	}
	if (SQLITE_OK != sqlite3_prepare_v2(m_db, sql, strlen(sql), &stmt, NULL)) {
#ifdef CONSOLE_FLAG
		cerr << "[ChatHistoryDB]:" << sqlite3_errmsg(m_db) << endl;
#endif // CONSOLE_FLAG
		return msg_ls;
	}
	if (date) sqlite3_bind_text(stmt, 1, date, strlen(date), NULL);
	while (SQLITE_ROW == sqlite3_step(stmt)) {
		Msg msg;
		msg.name = (const char*)sqlite3_column_text(stmt, 1);
		msg.text = (const char*)sqlite3_column_text(stmt, 2);
		msg.soundPath = (const char*)sqlite3_column_text(stmt, 3);
		msg.ct = (const char*)sqlite3_column_text(stmt, 4);
		msg_ls.push_back(msg);
	}
	sqlite3_finalize(stmt);
	return msg_ls;
}

QVector<QString> ChatHistoryDB::GetDates() {
	QVector<QString> dates;
	const char* sql = "select date(ct) from chat_history group by date(ct) order by date(ct) desc";
	sqlite3_stmt* stmt;
	int r = sqlite3_prepare_v2(m_db, sql, strlen(sql), &stmt, NULL);
	if (r != SQLITE_OK) {
#ifdef CONSOLE_FLAG
		cerr << "[ChatHistoryDB]:" << sqlite3_errmsg(m_db) << endl;
#endif // CONSOLE_FLAG
		return dates;
	}
	while (SQLITE_ROW == sqlite3_step(stmt))
	{
		QString d = (const char*)sqlite3_column_text(stmt, 0);
		dates.push_back(d);
	}
	sqlite3_finalize(stmt);
	return dates;
}

