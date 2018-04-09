#ifndef _SCREEN_H_
#define _SCREEN_H_

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

class Screen
{
public:
	Screen();
	~Screen();

	glm::mat4 toWorld;
	int face;

	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelview);
	void loadTexture();

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
	GLuint textureID;
	GLuint lineShader;

	bool debugOn = false;
};

// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
// This just looks nicer since it's easy to tell what coordinates/indices belong where.
const GLfloat vertices2[8][3] = {
	// "Front" vertices
	{ -100.0, -12.0,  50.0 },{ 100.0, -12.0,  50.0 },{ 100.0,  5.0,  50.0 },{ -100.0,  5.0,  50.0 },
	// "Back" vertices
	{ -100.0, -12.0, -150.0 },{ 100.0, -12.0, -150.0 },{ 100.0,  5.0, -150.0 },{ -100.0,  5.0, -150.0 }
};

// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral
const GLuint front_indices[6] = { 4, 0, 1, 1, 5, 4 };
#endif
