#ifndef _PARTICLE_H_
#define _PARTICLE_H_

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
#include <vector>

class Particle
{
public:
	Particle();
	~Particle();

	glm::mat4 toWorld = glm::mat4(1.0f);

	float mass;
	float elasticity = 0.01;
	float friction = 0.01;
	bool fixed;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;
	glm::vec3 normal;

	std::vector<glm::vec3> positions;

	void draw(GLuint);
	void update(float deltaTime);
	void applyForce(glm::vec3 f);
	void applyGravity();

	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
};
#endif
