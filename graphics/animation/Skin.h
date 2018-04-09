#ifndef _SKIN_H_
#define _SKIN_H_

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
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include "Skeleton.h"

using namespace std;

class Skin
{
public:
	Skin(Skeleton * s);
	~Skin();

	Skeleton * skel;

	glm::mat4 toWorld;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<int>skinweightnums;
	std::vector<glm::vec2> skinweights;
	std::vector<glm::vec3> triangles;
	std::vector<int> triangleIndices;
	std::vector<glm::mat4x4> bindings;
	std::vector<glm::vec3> bindpos;
	std::vector<glm::vec3> bindnorms;
	std::vector<glm::mat4x4> Ms;

	void draw(GLuint);
	void update();
	void parse(char * filename);

	GLuint VBO, VBON, VAO, EBO;
	GLuint uProjection, uModelview;
	GLuint shaderProgram;
};
#endif
