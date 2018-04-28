#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP
#include <glm/glm.hpp>
struct UVData {
	glm::vec2 u, v;
	UVData(glm::vec2 _u, glm::vec2 _v) {
		u = _u;
		v = _v;
	}
};
struct Triangle {
	glm::vec3 a, b, c;
	Triangle(glm::vec3 _a, glm::vec3 _b, glm::vec3 _c) {
		a = _a;
		b = _b;
		c = _c;
	}
	//Dangerous: leaves a, b, and c as random memory
	Triangle() {
	}
};
//Identical to a triangle, because it IS a triangle - this is the normals for each point on the respective triangle
struct Normals {
	glm::vec3 x, y, z;
	Normals(glm::vec3 _x, glm::vec3 _y, glm::vec3 _z) {
		x = _x;
		y = _y;
		z = _z;
	}
};
#endif