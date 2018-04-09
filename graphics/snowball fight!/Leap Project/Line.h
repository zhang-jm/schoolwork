#ifndef _LINE_H_
#define _LINE_H_

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
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Line
{
public:
	Line();
	~Line();

	glm::mat4 toWorld;
	bool pressed = false;

	std::vector<glm::vec3> endpoints;

	void draw(float color, GLuint shaderProgram);// , const glm::mat4 & projection, const glm::mat4 & modelview);
	void Line::setEndPoints(glm::vec3 start, glm::vec3 directionVector);

	// These variables are needed for the shader program
	GLuint VBO, VAO;
	GLuint uProjection, uModelview;
};
#endif
