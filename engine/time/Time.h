#pragma once
#include <core/IFRCore.h>
#include <chrono>

namespace ifr
{
	typedef double Timestep;

	class Time
	{
	public:
		IFRAPI static double GetElapsedTime();

		IFRAPI static double GetTimeScale();
		IFRAPI static void SetTimeScale(double scale);

		IFRAPI static Timestep GetTimestep();
		IFRAPI static void FrameUpdate();

	private:
		static double s_ElapsedTime;
		static double s_TimeScale;
		static double s_TimeStep;
		static double s_InitialTime;

	private:
		static double s_LastTrackedTime;
	};
}
