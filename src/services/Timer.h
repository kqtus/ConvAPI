#pragma once

class CTimer
{
public:
	CTimer();

	float GetTime() const;
	float GetTotalTime() const;
	float GetDeltaTime() const;
	
	/// Invoked before events' loop
	void Reset();

	/// Invoked when app is running
	void Start();

	/// Winvoked when app is stopped.
	void Stop();

	/// Invoked along eatch frame
	void Tick();

private:
	double m_SecondsPerCount = 0.f;
	double m_DeltaTime = -1.f;

	__int64 m_BaseTime = 0;
	__int64 m_PausedTime = 0;
	__int64 m_StopTime = 0;
	__int64 m_PrevTime = 0;
	__int64 m_CurrTime = 0;

	bool m_Stopped = false;
};