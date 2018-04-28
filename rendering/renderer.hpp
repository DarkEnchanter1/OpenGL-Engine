#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <vector>
#include <string>
#include <iostream>
#include "common/primitives.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
class RenderEngine {
	public:
		std::vector<Triangle> triangles;
		std::vector<UVData> uvdata;
		std::vector<Normals> normals;
		GLuint vertexbuffer, uvbuffer, normalbuffer;
		int render();
};
#endif