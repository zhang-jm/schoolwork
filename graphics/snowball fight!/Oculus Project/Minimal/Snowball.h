#ifndef _SNOWBALL_H_
#define _SNOWBALL_H_

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
#include "Model.h"

class Snowball
{
public:
	Snowball(Model * model, bool over);
	~Snowball();

	bool over;
	bool grabbed;
	bool shooting;
	bool outOfBounds;

	glm::mat4 toWorld;
	glm::mat4 rotate;
	//Skybox * cube;
	Model * model;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 spinAxis;
	float spinAmount;

	float radius = 5.0f;
	bool enemy = false;

	glm::vec3 aim;

	//void draw(float time, GLuint shaderProgram, glm::vec3 handPos, bool inputDown, glm::vec3 direction);
	void draw(float time, GLuint shaderProgram, const glm::mat4 & projection, const glm::mat4 & modelview, glm::vec3 handPos, bool triggerDown, glm::vec3 direction);
	void updatePosition(glm::vec3 delta);
	void spin();
	bool move(glm::vec3 handPos, float time, glm::vec3 direction);
	float RandomNumber(float Min, float Max);

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
};
#endif