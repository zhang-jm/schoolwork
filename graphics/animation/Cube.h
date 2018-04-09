#ifndef _CUBE_H_
#define _CUBE_H_

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

class Cube
{
public:
	Cube(bool box);
	~Cube();

	glm::mat4 toWorld;
	bool isSolid;

	void draw(GLuint);
	void update();
	void spin(float);

	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
	GLuint shaderProgram;
};

// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
// This just looks nicer since it's easy to tell what coordinates/indices belong where.
const GLfloat vertices[8][3] = {
	// "Front" vertices
	{-0.5, -0.5,  0.5}, {0.5, -0.5,  0.5}, {0.5,  0.5,  0.5}, {-0.5,  0.5,  0.5},
	// "Back" vertices
	{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5,  0.5, -0.5}, {-0.5,  0.5, -0.5}
};

// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral
const GLuint boxIndices[6][6] = {
	// Front face
	{0, 1, 2, 2, 3, 0},
	// Top face
	{1, 5, 6, 6, 2, 1},
	// Back face
	{7, 6, 5, 5, 4, 7},
	// Bottom face
	{4, 0, 3, 3, 7, 4},
	// Left face
	{4, 5, 1, 1, 0, 4},
	// Right face
	{3, 2, 6, 6, 7, 3}
};

const GLuint wireIndices[6][8] = {
	// Front face
	{ 0, 1, 1, 2, 2, 3, 3, 0 },
	// Top face
	{ 2, 3, 3, 7, 7, 6, 6, 2 },
	// Back face
	{ 4, 5, 5, 6, 6, 7, 7, 4 },
	// Bottom face
	{ 0, 1, 1, 5, 5, 4, 4, 0 },
	// Left face
	{ 0, 4, 4, 7, 7, 3, 3, 0 },
	// Right face
	{ 1, 2, 2, 6, 6, 5, 5, 1 }
};

#endif

