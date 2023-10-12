#pragma once
#include <iostream>

typedef void* CubismMotionQueueEntryHandle;

const int PriorityNone = 0;             
const int PriorityIdle = 1;             
const int PriorityNormal = 2;           
const int PriorityForce = 3;

extern const int PriorityNone;             
extern const int PriorityIdle;             
extern const int PriorityNormal;       
extern const int PriorityForce;

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