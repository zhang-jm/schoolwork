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
#include <vector>

class Sphere
{
public: 
	Sphere(GLuint shaderProgram);
	~Sphere();

	std::vector<glm::vec3> sphereVertices;
	std::vector<glm::vec3> sphereNormals;
	bool drawn = 0;
	
	glm::mat4 toWorld;
	GLuint shaderProgram;
	int prevStacks = 0;
	int prevSlices = 0;

	GLuint VBO, VBON VAO;
	GLuint uProjection, uModelview;
	GLuint textureID;

	void setUp();
	void draw(glm::mat4 C);
	void render();
	void update(glm::mat4 C);
	void setUpSphere(float radius, int stacks, int slices);
};
#endif
