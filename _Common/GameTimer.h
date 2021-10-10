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
	double mSecondsPerCount;	//ƽ�� �� ��
	double mDeltaTime;			//���� �����Ӱ� ������������ ��� �ð�

	__int64 mBaseTime;			//Ÿ�̸Ӱ� ���۵� �ð�
	__int64 mPausedTime;		//�Ͻ� ������ �ð�(������ �ð�)
	__int64 mStopTime;			//����(�Ͻ� ����)�� ������ �ð�
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};

#endif // GAMETIMER_H