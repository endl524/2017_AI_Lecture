#include "PrecisionTimer.h"

PrecisionTimer::PrecisionTimer() : m_NormalFPS(0.0), m_SlowFPS(1.0),
m_TimeElapsed(0.0), m_FrameTime(0), m_LastTime(0), m_LastTimeInTimeElapsed(0),
m_PerfCountFreq(0), m_bStarted(false), m_StartTime(0), m_LastTimeElapsed(0.0), m_bSmoothUpdates(false)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_PerfCountFreq);
	m_TimeScale = 1.0 / m_PerfCountFreq;
}

PrecisionTimer::PrecisionTimer(double fps) : m_NormalFPS(0.0), m_SlowFPS(1.0),
m_TimeElapsed(0.0), m_FrameTime(0), m_LastTime(0), m_LastTimeInTimeElapsed(0),
m_PerfCountFreq(0), m_bStarted(false), m_StartTime(0), m_LastTimeElapsed(0.0), m_bSmoothUpdates(false)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_PerfCountFreq);
	m_TimeScale = 1.0 / m_PerfCountFreq;
	m_FrameTime = (LONGLONG)(m_PerfCountFreq / m_NormalFPS);
}

void PrecisionTimer::Start()
{
	m_bStarted = true;

	m_TimeElapsed = 0.0;

	QueryPerformanceCounter((LARGE_INTEGER*)&m_LastTime);

	m_StartTime = m_LastTimeInTimeElapsed = m_LastTime;

	m_NextTime = m_LastTime + m_FrameTime;

	return;
}