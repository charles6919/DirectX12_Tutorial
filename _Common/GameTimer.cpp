//***************************************************************************************
// GameTimer.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include <windows.h>
#include "GameTimer.h"

GameTimer::GameTimer()
: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), 
  mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
	__int64 countsPerSec;	//�ʴ� ƽ ��
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;	//�ʴ� ƽ ���� ������ �Ͽ� ƽ�� �� ���� ���Ѵ�.
}

// Returns the total time elapsed since Reset() was called, NOT counting any
// time when the clock is stopped.
float GameTimer::TotalTime()const
{
	// Ÿ�̸Ӱ� ���� �����̸�, ������ �������� �帥 �ð��� ������� ���ƾ� �Ѵ�.
	// ����, ������ �̹� �Ͻ� ������ ���� �ִٸ� �ð��� mStopTime - mBestTime���� 
	// �Ͻ� ���� ���� �ð��� ���ԵǾ� �ִµ�, �� ���� �ð��� ��ü �ð��� ���Ե��� ���ƾ� �Ѵ�.
	// �̸� �ٷ���� ����, mStopTime���� �Ͻ� ���� ���� �ð��� ����.
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

	if( mStopped )
	{
		return (float)(((mStopTime - mPausedTime)-mBaseTime)*mSecondsPerCount);
	}

	// �ð��� mCurrTime - mBaseTime���� �Ͻ� ���� ���� �ð��� ���ԵǾ� �ִ�. 
	// �̸� ��ü �ð��� �����ϸ� �� �ǹǷ�, �� �ð��� mCurrTime���� ����.
	// 
	//
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime
	
	else
	{
		return (float)(((mCurrTime-mPausedTime)-mBaseTime)*mSecondsPerCount);
	}
}

float GameTimer::DeltaTime()const
{
	return (float)mDeltaTime;
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped  = false;
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


	// ����(�Ͻ� ����)�� ����(�簳) ���̿� �帥 �ð��� �����Ѵ�.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime     

	//���� ���¿��� Ÿ�̸Ӹ� �簳�ϴ� ���
	if( mStopped )
	{
		// �Ͻ� ������ �ð��� �����Ѵ�.
		mPausedTime += (startTime - mStopTime);	

		// Ÿ�̸Ӹ� �ٽ� �����ϴ� ���̹Ƿ�, ������ mPrevTime��
		// ��ȿ���� �ʴ�(�Ͻ� ���� ���߿� ���ŵǾ��� ���̹Ƿ�).
		// ���� ���� �ð����� �ٽ� �����Ѵ�. 
		mPrevTime = startTime;

		//������ ���� ���°� �ƴϹǷ� ���� ����������� �����Ѵ�.
		mStopTime = 0;
		mStopped  = false;
	}
}

void GameTimer::Stop()
{
	//�̹� ���� �����̸� �ƹ� �ϵ� ���� �ʴ´�.
	if( !mStopped )
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		//�׷��� �ʴٸ� ���� �ð��� Ÿ�̸� ���� �������� �����ϰ�,
		// Ÿ�̸Ӱ� �����Ǿ����� ���ϴ� bool ������ �����Ѵ�.
		mStopTime = currTime;
		mStopped  = true;
	}
}

void GameTimer::Tick()
{
	if( mStopped )
	{
		mDeltaTime = 0.0;
		return;
	}

	// ���� �������� �ð��� ��´�.
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	// �̹� �������� �ð��� ���� �������� �ð��� ���̸� ���Ѵ�.
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;

	// ���� �������� �غ��Ѵ�.
	mPrevTime = mCurrTime;

	// ������ ���� �ʰ� �Ѵ�. SDK ������ CDXUTTimer �׸� ������,
	// ���μ����� ���� ���� ���ų� ������ �ٸ� ���μ�����
	// ��Ű�� ��� mDeltaTime�� ������ �� �� �ִ�.
	if(mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}

