#include "cube.hpp"
#include <iostream>
#include <random>
#include <chrono>
Cube::Cube(double rotSpeed, double _x, double _y, double _z):
		 Object(_x, _y, _z, "resources/lunarlandernofoil_carbajal.3ds") {
	ran = rotSpeed;
	

}
void Cube::rUpdate() {

}
void Cube::update(double delta) {
	//std::cout << combined / 90 << std::endl;
	if (combined >= 10000) combined -= 10000;
	zR += (ran/10) * delta;
	yR += ran * delta;
	//combined += delta * 90;
	//y += delta * ran;

}