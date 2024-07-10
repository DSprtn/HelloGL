#include <Timer.h>

using namespace Core;

double Time::DeltaTime = 0.0f;
double Time::ElapsedTime = 0.0f;
double Time::UnscaledElapsedTime = 0.0f;
double Time::UnscaledDeltaTime = 0.0f;
float Time::Timescale = 1.0f;
std::chrono::steady_clock::time_point Time::Last = std::chrono::high_resolution_clock::now();

void Time::ComputeDeltas()
{

	auto now = std::chrono::high_resolution_clock::now();
	const auto elapsed = std::chrono::high_resolution_clock::now() - Last;
	const long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	double currentElapsed = ((double)microseconds / 1000000.0);
	Last = now;

	double deltaTime = currentElapsed;

	Time::DeltaTime = deltaTime * Timescale;
	Time::UnscaledDeltaTime = deltaTime;

	Time::ElapsedTime += deltaTime * Timescale;
	Time::UnscaledElapsedTime += deltaTime;
}

void Time::Init()
{
	Last = std::chrono::high_resolution_clock::now();
	DeltaTime = 0.0f;
	ElapsedTime = 0.0f;
	UnscaledElapsedTime = 0.0f;
	UnscaledDeltaTime = 0.0f;
	Timescale = 1.0f;
}
