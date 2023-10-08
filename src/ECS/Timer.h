#pragma once

#include <chrono>

namespace Core
{
	class Time
	{
	public:
		static double DeltaTime;
		static double ElapsedTime;
		static double UnscaledElapsedTime;
		static double UnscaledDeltaTime;
		static float Timescale;

		static void ComputeDeltas();

		static void Init();

	private:
		static std::chrono::steady_clock::time_point Last;

		Time() {};
	};
}