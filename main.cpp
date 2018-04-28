#include "main.hpp"
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <iomanip>
#include <memory>
#include <chrono>
#include <thread>
#include <random>
#include <sstream>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// Include local files
#include "rendering/utils/shader/shader.hpp"
#include "rendering/utils/texture/texture.hpp"
#include "common/controls.hpp"
#include "physics/objects/object.hpp"
#include "physics/objects/cube.hpp"
#include "common/delta.hpp"
std::minstd_rand0 re; //Generator for random number distribution
double fRand(double fMin, double fMax) {
   std::uniform_real_distribution<double> unif(fMin,fMax);
   return unif(re);

}
int rand(int min, int max) {
   std::uniform_int_distribution<int> unif(min, max);
   return unif(re);

}
void setWindowHints() {
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
void increaseWindowPos(int x, int y) {
	int xpos, ypos;
	glfwGetWindowPos(window, &xpos, &ypos);
	glfwSetWindowPos(window, xpos + x, ypos + y);
}
int initGLFW() {
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}
	setWindowHints();
	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 05 - Textured Cube", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	increaseWindowPos(20, 20);
	glfwMakeContextCurrent(window);
	return 0;
}
int initGLEW() {
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	return 0;
}
void setInputMode() {
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void openGlSetup(GLuint* VertexArrayID, GLuint* programID, GLuint* MatrixID, GLuint* ViewMatrixID, GLuint* ModelMatrixID, GLuint* LightID) {	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	glGenVertexArrays(1, VertexArrayID);
	glBindVertexArray(*VertexArrayID);
		// Create and compile our GLSL program from the shaders
	(*programID) = LoadShaders( "resources/vertexshader.vs", "resources/fragshader.fs" );

	// Get a handle for our "MVP" uniform
	(*MatrixID) = glGetUniformLocation(*programID, "MVP");
	(*ViewMatrixID) = glGetUniformLocation(*programID, "V");
	(*ModelMatrixID) = glGetUniformLocation(*programID, "M");
	(*LightID) = glGetUniformLocation(*programID, "LightPosition_worldspace");
}
static int get_size(const int fd, int *const rows, int *const cols) {
	struct winsize sz;

	int result;

	do {
		result = ioctl(fd, TIOCGWINSZ, &sz);
	} while (result == -1 && errno == EINTR);
	if (result == -1)
		return errno;

	if (rows)
		*rows = sz.ws_row;

	if (cols)
		*cols = sz.ws_col;

	return 0;
}

int width;
int height;

void clearAndPrint(const char* string) {
	std::cout << '\r';
	for (int i = 0; i < width; i++)
		std::cout << ' ';
	std::cout << '\r' << string;
	fflush(stdout);
}
void clearAndPrint(std::string string) {
	std::cout << '\r';
	for (int i = 0; i < width; i++)
		std::cout << ' ';
	std::cout << '\r' << string;
	fflush(stdout);
}
int main( void )
{
	initGLFW();
	initGLEW();
	setInputMode();
	GLuint VertexArrayID;
	GLuint programID;
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint LightID;
	openGlSetup(&VertexArrayID, &programID, &MatrixID, &ViewMatrixID, &ModelMatrixID, &LightID);

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								glm::vec3(12,6,6), // Camera is at (12,6,6), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// Load the texture using any two methods
	//GLuint Texture = loadBMP_custom("uvtemplate.bmp");
	GLuint Texture = loadDDS("resources/textures/concrete.DDS");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	
	using Clock = std::chrono::high_resolution_clock;
	double rat = ((double)Clock::period::num) / ((double)Clock::period::den);
	re = std::minstd_rand0(Clock::now().time_since_epoch().count());
	std::vector<Object*> objects = std::vector<Object*>();
	int count = rand(1, 3);
	std::cout << count << std::endl;
	for (int i = 0; i < count; i++) {
		objects.push_back(new Cube(fRand(0, 1), fRand(-5, 5), fRand(-5, 5), fRand(-5, 5)));
	}
	DeltaCalc deltaC;
	std::cout << "Your system's delta accuracy is " << rat << std::endl;
	double netDelta = 0;
	glm::vec3 lightPos = glm::vec3(4,4,4);
	get_size(STDOUT_FILENO, &width, &height);
	do{
		// Handles controls and view refactor
		computeMatricesFromInputs();
		double delta = deltaC.calcDelta(0);
		netDelta += delta;
		std::ostringstream strs;
		strs << "Program has been running for " << std::setprecision(3) << std::fixed << netDelta << " seconds";
		std::string str = strs.str();
		clearAndPrint(str);
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		
		//Recalculate view
			glm::mat4 Projection = getProjectionMatrix();
			// Camera matrix
			glm::mat4 View       = getViewMatrix();
			// Model matrix : an identity matrix (model will be at the origin)
			glm::mat4 Model      = glm::mat4(1.0f);
			// Our ModelViewProjection : multiplication of our 3 matrices
			MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
		
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Cull triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	for (Object* obj : objects) {
		delete obj;
	}
	return 0;
}

