#ifndef _SNOWBALLSYSTEM_H_
#define _SNOWBALLSYSTEM_H_

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
#include "Snowball.h"
#include "Model.h"

class SnowballSystem
{
public:
	SnowballSystem(bool isOpponent);
	~SnowballSystem();

	int particleCount = 0;
	int timer = 0;

	std::vector<Snowball *> snowballs;
	Snowball* snowballInHand;
	
	Model * snowball;

	bool holdingSnowball;

	bool isEnemy;

	bool gameOver = false;

	void drawParticles(float time, GLuint shaderProgram, glm::vec3 handPos, bool inputDown, glm::vec3 direction);
	void spawnSnowball();
	void removeSnowball();
	void grabNextSnowball();
};
#endif
