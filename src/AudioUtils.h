#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include <sys/stat.h>

namespace AudioUtils{
	/** 
	* @brief	读取.wav文件
	* @param	path	wav文件路径
	* @param	size	wav文件大小
	* @param	Log		回调日志函数
	*/
	char* LoadWavAsBytes(const char* path, int* size);

	/**
	* @brief	释放储存wav数据的内存
	* @param	bytes	wav数据存放数组指针
	* @param	path	wav文件路径
	* @param	Log		回调日志函数
	*/
	void ReleaseWavBytes(char* bytes, const char* path = NULL);

	/**
	* @brief	调整wav音频音量
	* @param	bytes	wav数组指针
	* @param	bufSize	数组大小
	* @param	vol		音量缩放倍数, 0-1之间浮点数
	*/
	void ResizeVolume(char* bytes, int bufSize, double vol);

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