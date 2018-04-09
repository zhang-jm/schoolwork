#ifndef _POINT_H_
#define _POINT_H_

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
#include <glm/mat4x3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Point
{
public:
	Point(float x, float y, float z, int id, float color);
	~Point();

	glm::mat4 toWorld;
	float color;
	int ID;
	std::vector<glm::vec3> location;

	GLuint VAO, VBO;

	void draw(GLuint shaderProgram);
	void selectionDraw(GLuint shaderProgram);
	void update(float x, float y, float z);
};
#endif
