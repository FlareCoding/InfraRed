#pragma once
#include <core/IFRCore.h>
#include <chrono>

namespace ifr
{
	typedef double Timestep;

	class Time
	{
	public:
		static double GetElapsedTime();

		static double GetTimeScale();
		static void SetTimeScale(double scale);

		static Timestep GetTimestep();
		static void FrameUpdate();

	private:
		static double s_ElapsedTime;
		static double s_TimeScale;
		static double s_TimeStep;
		static double s_InitialTime;

	private:
		static double s_LastTrackedTime;
	};
}
