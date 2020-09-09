#include "Time.h"

namespace ifr
{
	double time_since_epoch() {
		auto now = std::chrono::system_clock::now().time_since_epoch();
		return (double)std::chrono::duration_cast<std::chrono::microseconds>(now).count();
	}

	double Time::s_ElapsedTime = 0;
	double Time::s_TimeScale = 1;
	double Time::s_TimeStep = 0;

	double Time::s_InitialTime = -1;
	double Time::s_LastTrackedTime = 0;

	double Time::GetElapsedTime()
	{
		return s_ElapsedTime;
	}

	double Time::GetTimeScale()
	{
		return s_TimeScale;
	}

	void Time::SetTimeScale(double scale)
	{
		s_TimeScale = scale;
	}

	Timestep Time::GetTimestep()
	{
		return s_TimeStep;
	}

	void Time::FrameUpdate()
	{
		if (s_InitialTime == -1)
		{
			s_InitialTime = time_since_epoch();
		}

		double current_time = time_since_epoch();

		s_ElapsedTime = (current_time - s_InitialTime) / 1000000.0; // converting to seconds

		s_TimeStep = (s_ElapsedTime - s_LastTrackedTime) * 1000.0; // converting to miliseconds
		s_LastTrackedTime = s_ElapsedTime;
	}
}
