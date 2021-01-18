#pragma once

class Timing
{
public:
	static Timing& Instance() {
		static Timing instance;
		return instance;
	}

	double CurrentFrame;
	double LastFrame;
	float  DeltaTime;

protected:
	Timing() = default;
};