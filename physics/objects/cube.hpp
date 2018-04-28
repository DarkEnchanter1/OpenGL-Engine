#ifndef CUBE_H
#define CUBE_H
#include "object.hpp"
class Cube : public Object {
	public:
	bool outYet = false;
	double combined = 0;
	double ran = 0;
	Cube(double rotSpeed, double x, double y, double z);
	void rUpdate();
	void update(double delta);
};

#endif