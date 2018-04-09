#ifndef _JOINT_H_
#define _JOINT_H_

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
#include "Node.h"

class Joint : public Node
{
public:
	Joint();
	~Joint();

	glm::mat4 toWorld;
	glm::mat4 toWorld2;
	Cube * c;

	std::string name;
	glm::vec3 offset;
	glm::vec3 boxmin;
	glm::vec3 boxmax;
	glm::vec2 rotx;
	glm::vec2 roty;
	glm::vec2 rotz;
	glm::vec3 pose;

	std::vector<Node *> children;
	Node * parent;

	void draw(GLuint);
	void update(glm::mat4);

	void addChild(Node *);
	void setOffset(glm::vec3);
	void setBoxmin(glm::vec3);
	void setBoxmax(glm::vec3);
	void setRotx(glm::vec2);
	void setRoty(glm::vec2);
	void setRotz(glm::vec2);
	void setPose(glm::vec3);
	void validateJoint();
	glm::vec3 getClampedPose();

	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
};
#endif
