#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class OBJObject
{
private:
std::vector<unsigned int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
glm::mat4 OBJObject::toWorld;
glm::mat4 OBJObject::modelview;



public:
	OBJObject(const char* filepath);
	~OBJObject();

	// These variables are needed for the shader program
	GLuint VBO, VBON, VAO, EBO;
	GLuint uProjection, uModelview;
	GLuint sp;

	float angle;
	int light = 0;
	bool lighton = 0;
	bool firstparse = 0;
	bool firstdraw = 0;
	bool firstvertice = 0;

	float xdir, ydir;
	float xp, yp, zp;
	float xs, ys, zs;
	float xsd, ysd, zsd;
	float cutoff, exp;

	float avgx = 0, avgy = 0, avgz = 0;
	float axis = 0;

	glm::vec3 dirLightDirection;

	void parse(const char* filepath);
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec3> getNormals();
	glm::mat4 getToWorld();
	void setToWorld(glm::mat4 tw);
	void draw(GLuint, int color, int light);
	void setUpMaterials(GLuint, int color);
	void initializeLights(GLuint shaderProgram);
	void setUpLights(GLuint, int light);
	void move(GLuint shaderProgram, float x, float y, float z);
	void scale(GLuint shaderProgram, float direction);
	void orbit(GLuint shaderProgram, float deg, float x, float y, float z);
	void reset();
};

#endif