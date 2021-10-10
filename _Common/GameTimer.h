//***************************************************************************************
// GameTimer.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer
{
public:
	GameTimer();

	float TotalTime()const; // in seconds
	float DeltaTime()const; // in seconds

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.

private:
	double mSecondsPerCount;	//틱당 초 수
	double mDeltaTime;			//이전 프레임과 현재프레임의 경과 시간

	__int64 mBaseTime;			//타이머가 시작된 시간
	__int64 mPausedTime;		//일시 정지된 시간(누적된 시간)
	__int64 mStopTime;			//정지(일시 정지)된 시점의 시간
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};

#endif // GAMETIMER_H