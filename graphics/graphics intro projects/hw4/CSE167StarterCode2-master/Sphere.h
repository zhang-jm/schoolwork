#ifndef _SPHERE_H_
#define _SPHERE_H_

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
#include "Geode.h"
#include <vector>

class Sphere : public Geode 
{
public: 
	Sphere(GLuint shaderProgram, float color);
	~Sphere();

	std::vector<glm::vec3> sphereVertices;
	std::vector<glm::vec3> sphereNormals;
	int prevStacks = 0;
	int prevSlices = 0;
	float pi = 3.14159;
	bool drawn = 1;
	
	glm::mat4 toWorld;
	GLuint shaderProgram;
	GLuint textureID;
	float color;

	GLuint VBO, VBON, VAO, EBO;
	GLuint uProjection, uModelview;

	void draw(glm::mat4 C);
	void render();
	void update(glm::mat4 C);
	void setUpSphere(float radius, int stacks, int slices);
	void loadTextures();
};
#endif
