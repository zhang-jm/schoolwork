#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

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
//#include "Model.h"
#include "SnowballSystem.h"

class Player
{
public:
	Player(glm::vec3 initPos, bool isOpponent);
	~Player();

	bool over;
	bool grabbed;
	bool shooting;

	glm::mat4 toWorld;
	glm::mat4 rotate;	
	Model * model;

	bool isOpponent;

	glm::vec3 headPosition;
	glm::vec3 handPosition;
	glm::vec3 handAngle;

	SnowballSystem * mySnowballs;	

	float radius = 5.0f;

	glm::vec3 aim;

	void setModel();

	void draw(GLuint snowballShaderProgram, float time, bool inputDown, bool isArticuno);
	
	void move(glm::vec3 delta);

	void grabSnowball();

	bool snowballCollision();
	

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
};
#endif