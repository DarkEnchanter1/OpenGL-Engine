#include "delta.hpp"
DeltaCalc::DeltaCalc() {
	currentTime = Clock::now().time_since_epoch().count() * rat;
}
double DeltaCalc::calcDelta(double maxValue) {
	double newTime = Clock::now().time_since_epoch().count() * rat;
	double delta = newTime - currentTime;
	currentTime = newTime;
	if (delta <= maxValue || maxValue <= 0)
		return delta;
	else return maxValue;
}