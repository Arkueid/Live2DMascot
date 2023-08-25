/**
* 语音控制，包括播放，停止，和调整音量大小
*/

#include "AudioUtils.h"
#include "LAppDefine.hpp"

using namespace LAppDefine;

void AudioUtils::StopSound() {
	PlaySound(NULL, NULL, SND_ASYNC| SND_NODEFAULT);
}

bool AudioUtils::StartSound(const char* path, double vol, bool force) {
	bool success;
	long lrVolume = MAKELONG(INT16_MAX * vol, INT16_MAX * vol);
	waveOutSetVolume(NULL, lrVolume);
	if (DebugLogEnable) {
		printf("[APP][Audio]Volume Set: %.2lf\n", vol);
		printf("[APP][Audio]Play Sound: %s\n", path);
	}
	if (force)
		success = PlaySound(path, NULL, SND_ASYNC | SND_NODEFAULT | SND_FILENAME);
	else
		 success = PlaySound(path, NULL, SND_ASYNC | SND_NOSTOP | SND_NODEFAULT | SND_FILENAME);
	return success;
}

bool AudioUtils::IsSoundPlaying() {
	return !PlaySound(NULL, NULL, SND_NODEFAULT | SND_FILENAME | SND_ASYNC | SND_NOSTOP);
}