#ifndef _NODE_H_
#define _NODE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Node {
public:
	//virtual ~Node() {}
	bool culling;
	virtual void draw(glm::mat4 C) = 0; 
	virtual void update(glm::mat4 C) = 0;
};
#endif