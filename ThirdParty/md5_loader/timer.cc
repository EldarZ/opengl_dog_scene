#include "timer.h"
#include <chrono>
using namespace std::chrono;

Timer::Timer ()
{
	fFps = 0;
	fFpsCounter = 0;
	fFrameInterval = 0;
	fFrameTime = 0;
	fLastFrameTime = 0;
	fCurrentTime = 0;
}

Timer::~Timer ()
{

}

void Timer::CalculateFrameRate()
{
	fCurrentTime = (float)(high_resolution_clock::now().time_since_epoch()/1000).count();
	//fCurrentTime = (float)SDL_GetTicks() / 1000;
	
	fFrameInterval = fCurrentTime - fFrameTime;
	
	fFrameTime = fCurrentTime;
	
	++fFpsCounter;

	if(fCurrentTime - fLastFrameTime > 1.0) 
	{
		fLastFrameTime = fCurrentTime;

		fFps = fFpsCounter;

		fFpsCounter = 0;
	}
}

float Timer::GetTime() {
	return  (float)(high_resolution_clock::now().time_since_epoch() / 1000).count();
}
//float Timer::GetTime() { return (float)SDL_GetTicks() / 1000; }
