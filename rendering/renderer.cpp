#include "renderer.hpp"
int RenderEngine::render() {
		glm::mat4 tMat = glm::translate(glm::mat4(), glm::vec3(0, 0, 0));
		glm::mat4 rMat = glm::rotate((float)0, glm::vec3(0, 0, 1)) * glm::rotate((float)0, glm::vec3(0, 1, 0)) * glm::rotate((float)0, glm::vec3(1, 0, 0));
		glm::mat4 mat = tMat * rMat;
		//Loading buffer data for vertexes into GPU
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangles) * 3, &triangles, GL_STREAM_DRAW);
		//Same for UV data (texture mapping)
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uvdata), &uvdata, GL_STREAM_DRAW);	
		//...and for normal maps
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * 3, &normals, GL_STREAM_DRAW);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		 // 3rd attribute buffer : normals
	 	glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
	    	 2,                                // attribute
		     3,                                // size
	    	 GL_FLOAT,                         // type
		     GL_FALSE,                         // normalized?
		     0,                                // stride
		     (void*)0                          // array buffer offset
		 );
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3); // 12*3 indices starting at 0 -> 12 triangles
		GLenum err = glGetError();
		while((err = glGetError()) != GL_NO_ERROR) {
			// Process/log the error.
			std::cout << "ERROR: " << err << std::endl;
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		return 0;
	};