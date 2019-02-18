#include "Timer.h"
#include <Windows.h>

CTimer::CTimer()
{
	__int64 counts_per_sec;
	QueryPerformanceCounter((LARGE_INTEGER*)&counts_per_sec);
	m_SecondsPerCount = 1.f / (double)counts_per_sec;
}

float CTimer::GetTime() const
{
	return m_CurrTime;
}

float CTimer::GetTotalTime() const
{
	return m_Stopped
		? (float)(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount)
		: (float)(((m_CurrTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
}

float CTimer::GetDeltaTime() const
{
	return m_DeltaTime;
}

void CTimer::Reset()
{
	__int64 curr_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&curr_time);

	m_BaseTime = curr_time;
	m_PrevTime = curr_time;
	m_StopTime = 0;
	m_Stopped = false;
}

void CTimer::Start()
{
	__int64 start_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&start_time);

	if (m_Stopped)
	{
		m_PausedTime += (start_time - m_StopTime);
		m_PrevTime = start_time;

		m_StopTime = 0;
		m_Stopped = false;
	}
}

void CTimer::Stop()
{
	if (!m_Stopped)
	{
		__int64 curr_time;
		QueryPerformanceCounter((LARGE_INTEGER*)&curr_time);

		m_StopTime = curr_time;
		m_Stopped = true;
	}
}

void CTimer::Tick()
{
	if (m_Stopped)
	{
		m_DeltaTime = 0.f;
		return;
	}

	__int64 curr_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&curr_time);
	m_CurrTime = curr_time;

	m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerCount;
	m_PrevTime = m_CurrTime;

	if (m_DeltaTime < 0.f)
		m_DeltaTime = 0.f;
}
