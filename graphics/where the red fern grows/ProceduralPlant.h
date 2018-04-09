#ifndef PROCEDURAL_PLANT_H_
#define PROCEDURAL_PLANT_H_

#include <GL/glew.h>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

#include <vector>
#include <string>

class ProceduralPlant {
public:
	~ProceduralPlant();
	void draw(GLuint shader);
	void regenerate();
	void regenerate(unsigned int seed);
	void setToWorld(glm::mat4);
protected:
	void generateGeometry(float distance, float theta, float lineWidth, std::string commandString);
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<std::vector<glm::vec3>> cylinders_v;
	std::vector<std::vector<glm::vec3>> cylinders_n;
	std::vector<std::vector<glm::vec3>> caps_v;
	std::vector<std::vector<glm::vec3>> caps_n;
	std::vector<int> indices;
	std::string commandString;
	float distance;
	float theta;
	float lineWidth;
	bool generated = false;
	glm::mat4 toWorld;
	GLuint VBO, VAO, EBO, VBON;
	void drawCylinderAroundLine(glm::vec3 v1, glm::vec3 v2, float radius, float length, glm::vec3 color);
};

#endif