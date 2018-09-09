#ifndef _TGTIMER_H
#define _TGTIMER_H

#include "defs.h"

class Timer
{
 public:
	Timer ();
	~Timer ();
	float GetFPS () { return fFps; }
	float GetFrameInterval () { return fFrameInterval; }
	float GetTime();
	void CalculateFrameRate ();
	
 protected:
	float fFps;
	float fFpsCounter;
	float fFrameInterval;
	float fFrameTime;
	float fLastFrameTime;
	float fCurrentTime;
};


#endif
