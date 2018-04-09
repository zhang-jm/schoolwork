#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <vector>
#include <time.h>

using namespace std;

class Particles
{
public:
	Particles(GLuint shaderProg);
	~Particles();

	glm::mat4 toWorld;

	std::vector<glm::vec3> points;
	std::vector<int> times;

	int size;

	void draw(GLuint);
	void setup(int n);
	void update();
	double dRand(double dMin, double dMax);

	// These variables are needed for the shader program
	GLuint VBO, VAO;
	GLuint uProjection, uModelview;
	GLuint textureID;
	GLuint shader;
};
#endif
