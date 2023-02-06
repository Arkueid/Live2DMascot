#pragma once
#include "glwidget.h"
#include <QtCore/qtextcodec.h>
#include <QtWidgets/qapplication.h>

void Log(const char* handler, const char* msg);

namespace LAppConfig {

	extern int _WindowWidth;
	extern int _WindowHeight;
	extern int _LastPosX;
	extern int _LastPosY;
	extern int _FPS;
	extern string _AppName;
	extern string _IconPath;
	extern string _ModelName;
	extern string _ModelDir;
	extern bool _KeepQuiet;
	extern bool _MouseTrack;
	extern bool _StayOnTop;
	extern bool _NoSound;
	extern bool _ShowText;
	extern bool _ShowBgmList;
	extern const char* _ConfigPath;
	extern string _DialogStyleSheet;
	extern int _TextFadeOutTime;
	extern int _DialogWidth;
	extern int _DialogHeight;
	extern int _DialogWordInterval;
	extern int _BgmListLastPosX;
	extern int _BgmListLastPosY;
	extern string _NoteOutPath;
	extern string _ApiKey;
	extern string _ApiSecret;
	extern int _MotionInterval;
	extern string _UserName;
};

class LApp
{
private:
	QApplication* _app;
	GLWidget* _win;
public:
	LApp();
	~LApp();
	static LApp* GetInstance();
	void SaveConfig();
	static void warning(const char* x);
	void Initialize(int argc, char* argv[]);
	void Run();
	void Release();
	void LoadConfig();
	QApplication* GetApp();
	GLWidget* GetWindow();
};