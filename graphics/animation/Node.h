#ifndef _NODE_H_
#define _NODE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Node {
public:
	glm::mat4 toWorld;
	glm::mat4 toWorld2;

	virtual void update(glm::mat4) = 0;
	virtual void draw(GLuint) = 0;
	virtual void validateJoint() = 0;
};
#endif
