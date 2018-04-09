#ifndef _CLOTH_H_
#define _CLOTH_H_

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
#include "Particle.h"
#include "SpringDamper.h"
#include "Triangle.h"

class Cloth
{
public:
	Cloth();
	~Cloth();

	glm::mat4 toWorld;

	int usedParticles;
	std::vector<Particle *> particles;
	std::vector<SpringDamper *> dampers;
	std::vector<Triangle * > triangles;

	void init(float width, float height);
	void initFixTopRow(float width, float height);
	void initFixTopCorners(float width, float height);
	void draw(GLuint, GLuint);
	void update(float deltaTime);
	void moveFixedParticles(std::string s);
	void changeWindSpeed(bool increase);

	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
};
#endif
