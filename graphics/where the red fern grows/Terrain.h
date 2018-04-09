#ifndef _TERRAIN_H_
#define _TERRAIN_H_

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
#include <stdlib.h>
#include <vector>
#include <time.h>

using namespace std;

class Terrain
{
public:
	Terrain();
	~Terrain();

	glm::mat4 toWorld;

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> grid;

	double ** height;
	int ogrange = 100;
	int range = ogrange;
	int size;

	void draw(GLuint);
	void generateTerrain(int n);
	void diamond(int sidelength, int half);
	void square(int sidelength, int half);
	double dRand(double dMin, double dMax);

	// These variables are needed for the shader program
	GLuint VBO, VAO, VBON, EBO;
	GLuint uProjection, uModelview;
	GLuint textureID;
};
#endif
