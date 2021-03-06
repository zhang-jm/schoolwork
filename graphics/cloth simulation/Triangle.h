#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

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
#include "Particle.h"
#include <vector>

class Triangle
{
public:
	Triangle();
	~Triangle();

	glm::mat4 toWorld = glm::mat4(1.0f);

	float rho;
	float cdrag;
	glm::vec3 vair;

	glm::vec3 n;

	Particle * p1;
	Particle * p2;
	Particle * p3;

	void setParticles(Particle * p1, Particle * p2, Particle * p3);
	void computeForce();
	void draw(GLuint);

	GLuint VBO, VBON, VAO;
	GLuint uProjection, uModelview;
};
#endif
