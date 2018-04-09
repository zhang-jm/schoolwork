#ifndef _CONTROL_H_
#define _CONTROL_H_

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
#include "Point.h"

class Control
{
public:
	Control(GLuint shaderProgram);
	~Control();

	glm::mat4 toWorld;
	GLuint shaderProgram;
	float color;

	glm::vec3 highest;
	glm::vec3 currPoint;
	float accel = -0.5f;
	float highestt;
	float velt = 0;
	int highestcurve;
	int currcurve;

	void draw();
	void update(glm::mat4 C);
	void setUpCurves();
	void addPoints();
	glm::vec3 calculateNextPoint();
	void reset();

	std::vector<glm::vec3> curve1;
	std::vector<Point * > pointsformatrix;
	std::vector<Point * > pointstodraw;

	glm::mat4 B = glm::mat4x4(glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f),
		glm::vec4(3.0f, -6.0f, 3.0f, 0.0f),
		glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

	// These variables are needed for the shader program
	GLuint VBO, VBO2, VAO, EBO;
	GLuint uProjection, uModelview;
	GLuint textureID;
};
#endif

