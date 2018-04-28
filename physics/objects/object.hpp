#ifndef OBJECT_H
#define OBJECT_H
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
class Object {
    public:
		// The coords and rotation for the object
		GLuint vertexbuffer;
		GLuint uvbuffer;
		GLuint normalbuffer;
		double x, y, z, xR, yR, zR;
		// The vertex buffer (necessary for _render calls)
		std::vector<glm::vec4> vertexes;
		std::vector<glm::vec2> uvdata;
		std::vector<glm::vec3> normals;


        Object(double _x, double _y, double _z, const char * model);

		/* The "lightweight" update method. 
		 * This is intended for updates that are both fast and need to happen every render cycle.
		 * NOTE: Be VERY careful with this - It runs on the render thread, so any delays in here will cause the renderer to slow down as well.
		*/
		virtual void rUpdate();

		/* The "heavy" update method.
		 * This is intended for updates that are not as critical and can happen slightly less often.
		 * Note: This update cycle takes place on a secondary thread so as to not slow down the render thread.
		*/
		virtual void update(double delta);
};
#endif