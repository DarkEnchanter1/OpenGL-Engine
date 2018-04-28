#ifndef DELTA_HPP
#define DELTA_HPP

//A simple delta calculator that allows for a max value to be specified
#include <chrono>
using Clock = std::chrono::high_resolution_clock;
class DeltaCalc {
	private: //Explicit is cleaner than implicit
	double rat = ((double)Clock::period::num) / ((double)Clock::period::den);
	double currentTime;
	public:
	DeltaCalc();
	// Calculates the delta. Note that this is in real seconds, not whatever the accuracy of the clock is. (i.e. expect very small numbers)
	double calcDelta(double maxValue);
};
#endif