#ifndef _SKELETON_H_
#define _SKELETON_H_

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
#include <iostream>
#include "Joint.h"
#include <string>

class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	int jointnum;
	int direction;
	glm::mat4 toWorld;
	Joint * root;
	std::vector<Joint *> joints;

	void draw(GLuint);
	void update();
	void parse(char * filename);
	glm::mat4 getWorldMatrix(int jointNum);
	void nextJoint();
	void increaseDOF();
	void decreaseDOF();
	void changeDirection();

	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
	GLuint shaderProgram;
};

#endif
