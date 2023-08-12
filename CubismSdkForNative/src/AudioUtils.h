#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include <sys/stat.h>

namespace AudioUtils{
	/**
	* @brief	停止正在播放的音频
	*/
	void StopSound();

	/**
	* @brief	播放音频
	* @param	path	音频路径，仅wav
	* @param	vol		音量缩放倍数, 0-1之间的浮点数
	*/
	bool StartSound(const char* path, double vol, bool force = false);

	/**
	* @brief	是否有声音在播放
	*/
	bool IsSoundPlaying();
}