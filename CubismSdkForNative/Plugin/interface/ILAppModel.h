#pragma once
#include <iostream>

typedef void* CubismMotionQueueEntryHandle;

class IMotion;

class ILAppModel {
public:
	virtual CubismMotionQueueEntryHandle Speak(const char* text, const char* soundPath, void (*onFinishedMotionHandler)(IMotion* self) = NULL) = 0;
	virtual CubismMotionQueueEntryHandle StartMotion(const char* group, signed int no, signed int priority, void (*onFinishedMotionHandler)(IMotion* self) = NULL) = 0;
	virtual CubismMotionQueueEntryHandle StartRandomMotion(const char* group, signed int priority, void (*onFinishedMotionHandler)(IMotion* self) = NULL) = 0;
    virtual void SetExpression(const char* expressionID) = 0;
    virtual void SetRandomExpression() = 0;
	virtual void StopLipSync() = 0;
	virtual bool IsMotionFinished() = 0;
};