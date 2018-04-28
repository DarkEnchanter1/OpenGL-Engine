#include "object.hpp"
#include "utils/objloader.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <GL/glew.h>
#include <iostream>
Object::Object(double _x, double _y, double _z, const char* model) {
	x = _x;
	y = _y;
	z = _z;
	xR = 0;
	yR = 0; 
	zR = 0;
	std::vector<glm::vec3> vertexes_;
	std::vector<unsigned short> indices;
	bool res = loadAssImp(model, indices, vertexes_, uvdata, normals);
//	std::cout << "Hello?" << std::endl;
	for (glm::vec3 vertex : vertexes_) {
		vertexes.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f));
	}
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
}
void Object::update(double delta) {

}
void Object::rUpdate() {

}