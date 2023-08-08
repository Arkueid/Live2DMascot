/**
* 语音控制，包括播放，停止，和调整音量大小
*/

#include "AudioUtils.h"
#include "LAppDefine.hpp"

using namespace LAppDefine;


#if 0
char* AudioUtils::LoadWavAsBytes(const char* path, int* size) {
	struct stat statbuf;
	stat(path, &statbuf);
	*size = (int)statbuf.st_size;
	char* bytes = new char[*size];
	std::ifstream ifs(path, std::ios::in | std::ios::binary);
	if (ifs.fail()) {
		if (DebugLogEnable)
			printf("[APP][Audio]file open error: %s\n", path);
		return NULL;
	}
	ifs.read(bytes, *size);
	ifs.close();
	if (DebugLogEnable)
		printf("[APP][Audio]Load Bytes: %s\n", path);
	return bytes;
}

void AudioUtils::ReleaseWavBytes(char* bytes, const char* path) {
	delete[] bytes;
	if (path != NULL && DebugLogEnable) printf("[APP][Audio]Delete Bytes: %s\n", path);
}

void AudioUtils::ResizeVolume(char* bytes, int bufSize, double vol) {
	//读取pcm数据长度
	//pcm数据长度在第40个byte至第44个byte
	int pcmLength;
	char str[5];
	if (DebugLogEnable) {
		for (int i = 0, offset = 8; i < 4; i++) {
			str[i] = bytes[offset + i];
		}
		str[4] = '\0';
		printf("[APP][Audio]Format: %s\n", str);
		printf("[APP][Audio]Audio Format: %d\n", *(short*)(bytes + 20));
		printf("[APP][Audio]Num Channels: %d\n", *(short*)(bytes + 22));
		printf("[APP][Audio]Sample Rate: %d\n", *(int*)(bytes + 24));
		printf("[APP][Audio]Byte Rate: %d\n", *(int*)(bytes + 28));
		printf("[APP][Audio]Block Align: %d\n", *(short*)(bytes + 32));
		printf("[APP][Audio]Bits Per Sample: %d\n", *(short*)(bytes + 34));
	}

	//BitsPerSample
	int BitsPerSample = *(short*)(bytes + 34);
	//不是16位采样率
	if (BitsPerSample != 16) {
		if (DebugLogEnable)
			printf("[AudioUtils]Unhandled BitsPerSample: %d\n", BitsPerSample);
		return;
	}
	for (int i = 0, offset = 36; i < 4; i++) {
		str[i] = bytes[offset + i];
	}
	str[4] = '\0';
	if (DebugLogEnable)
		printf("[APP][Audio]SubChunk2ID: %s\n", str);
	short wData; long dwData;
	int offset, type;
	if (strcmp(str, "data") == 0) {
		pcmLength = bufSize;
		offset = 44;
		type = 0;
	}
	else if (strcmp(str, "LIST") == 0) {
		// 格式转换软件处理过
		// 36~40 LIST
		// 44~52 INFOISFT
		// 56~60 Lavf
		// 60~70 56.10.100\x00 {codec}
		pcmLength = bufSize;
		offset = 74;
		type = 1;
		return;
	}
	else {
		if (DebugLogEnable)
			printf("[APP][Audio]Unknown SubChunk2ID: %s\n", str);
		return;
	}
	//pcm数据在下标44到pcmLength-1下标处之间
	for (int pcmPtr = offset; pcmPtr < pcmLength; pcmPtr ++) {
		// pcm数据中两个byte表示一个振幅数据，可以用一个short储存
		// makeword 将高八位和低八位bit合成一个振幅数据
		wData = MAKEWORD(bytes[pcmPtr], bytes[pcmPtr + 1]);
		// 用long int 储存防止与vol相乘溢出
		dwData = wData;
		dwData = dwData * vol;
			
		// 控制音量范围
		if (dwData < -0x8000) dwData = -0x8000;
		else if (dwData > 0x7FFF) dwData = 0x7FFF;
		// 截取dwData的低16位得到一个振幅数据
		wData = LOWORD(dwData);
		// 低8位
		bytes[pcmPtr] = LOBYTE(wData);
		// 高8位
		bytes[pcmPtr + 1] = HIBYTE(wData);
	}
}
#endif

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