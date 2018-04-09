#include "ProceduralPlant.h"
#include "Window.h"

#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <time.h>
#include <stack>
#include <vector>

#define SLICES 44

#define BROWN glm::vec3(0.2f, 0.1f, 0.0f)
#define RED glm::vec3(0.3f, 0.05f, 0.05f)
#define PINK glm::vec3(0.3f, 0.0f, 0.1f)
#define PURPLE glm::vec3(0.3f, 0.0f, 0.3f)
#define YELLOW glm::vec3(0.3f, 0.3f, 0.1f)
#define GREEN glm::vec3(0.0f, 0.2f, 0.0f)

float myrand() {
	return (rand() & 1000) / 800;
}

glm::vec3 fudge(glm::vec3 vec) {
	glm::vec3 fudger = glm::vec3(myrand(), myrand(), myrand());
	return vec + fudger;
}

glm::vec3 applyTransformation(glm::vec3 vec, glm::vec3 v1, glm::vec3 v2, float radius, float length) {
	glm::vec3 line = v2 - v1;
	glm::vec3 line_u = line / glm::length(line_u);
	glm::vec3 ret = vec;
	float vec_len = glm::length(vec);
	glm::vec3 vec_u = vec / vec_len;

	// Rotate


	// Scale
	ret.y *= length;
	ret.x *= radius;
	ret.z *= radius;
	
	// Translate
	ret += glm::vec3((v1.x + v2.x) / 2.0f, (v1.y + v2.y) / 2.0f, (v1.z + v2.z) / 2.0f);

	// Return
	return ret;
}

void ProceduralPlant::drawCylinderAroundLine(glm::vec3 v1, glm::vec3 v2, float radius, float length, glm::vec3 color) {

	float fslices = (float)SLICES;
	float pi = glm::pi<float>();

	std::vector<glm::vec3> cylVertices;
	std::vector<glm::vec3> cylNormals;

	// Draw middle
	for (int i = 0; i < SLICES; ++i)
	{
		const float step = 2.0f * pi / fslices;
		cylVertices.emplace_back(0.25f * glm::cos(i * step), 0.25f * glm::sin((i * step + 0.5f * step)), 1.0f * (i % 2 == 0 ? -0.5f : 0.5f));
		cylNormals.push_back(color);
	}

	// Close off middle
	for (int i = 0; i < 2; i++) {
		cylVertices.push_back(cylVertices[i]);
		cylNormals.push_back(cylNormals[i]);
	}

	// Now do caps of cylinder
	std::vector<glm::vec3> capVertices1;
	std::vector<glm::vec3> capNormals1;
	std::vector<glm::vec3> capVertices2;
	std::vector<glm::vec3> capNormals2;
	float z = 0.5f;
	capVertices1.emplace_back(0.0f, 0.0f, z);
	capNormals1.emplace_back(0.0f, 0.0f, 0.0f);
	for (int i = 0; i <= SLICES; i++) {
		capVertices1.emplace_back(
			0.0f/*x*/ + ((radius / 4.0f) * cos(i *  2.0f * pi / SLICES)),
			0.0f/*y*/ + ((radius / 4.0f) * sin(i * 2.0f * pi / SLICES)),
			z
		);
		//capNormals1.emplace_back(0.0f, 0.0f, 1.0f);
		capNormals1.push_back(color);
	}
	z = -z;
	capVertices2.emplace_back(0.0f, 0.0f, z);
	capNormals2.emplace_back(0.0f, 0.0f, 0.0f);
	for (int i = 0; i <= SLICES; i++) {
		capVertices2.emplace_back(
			0.0f/*x*/ + ((radius / 3.95f) * cos(i *  2.0f * pi / SLICES)),
			0.0f/*y*/ + ((radius / 3.95f) * sin(i * 2.0f * pi / SLICES)),
			z
		);
		//capNormals2.emplace_back(0.0f, 0.0f, -1.0f);
		capNormals2.push_back(color);
	}



	// Translate and push middle
	for (int i = 0; i < cylVertices.size(); ++i) {
		glm::vec3 vertex = cylVertices[i];
		vertex = glm::vec3(vertex.x, vertex.z, vertex.y);
		vertex = applyTransformation(vertex, v1, v2, radius, length);
		cylVertices[i] = vertex;
	}

	// Translate and push cap 1
	for (int i = 0; i < capVertices1.size(); ++i) {
		glm::vec3 vertex = capVertices1[i];
		vertex = glm::vec3(vertex.x, vertex.z, vertex.y);
		vertex = applyTransformation(vertex, v1, v2, radius, length);
		capVertices1[i] = vertex;
	}

	// Translate and push cap 2
	for (int i = 0; i < capVertices2.size(); ++i) {
		glm::vec3 vertex = capVertices2[i];
		vertex = glm::vec3(vertex.x, vertex.z, vertex.y);
		vertex = applyTransformation(vertex, v1, v2, radius, length);
		capVertices2[i] = vertex;
	}

	cylinders_v.push_back(cylVertices);
	cylinders_n.push_back(cylNormals);

	caps_v.push_back(capVertices1);
	caps_n.push_back(capNormals1);

	caps_v.push_back(capVertices2);
	caps_n.push_back(capNormals2);
}

void ProceduralPlant::regenerate() {
	if (!this->generated) return;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBON);
	glDeleteBuffers(1, &EBO);
	generateGeometry(this->distance, this->theta, this->lineWidth, this->commandString);
}

void ProceduralPlant::regenerate(unsigned int seed) {
	if (!this->generated) return;
	srand(seed);
	this->regenerate();
}

void ProceduralPlant::generateGeometry(float distance, float theta, float lineWidth, std::string commandString) {
	this->generated = true;
	this->distance = distance;
	this->theta = theta;
	this->lineWidth = lineWidth;
	this->commandString = commandString;

	float theta_rads = glm::radians(theta);

	vertices.clear();
	normals.clear();
	cylinders_v.clear();
	cylinders_n.clear();
	caps_v.clear();
	caps_n.clear();
	indices.clear();

	glm::vec3 currPos = glm::vec3(0.0f);
	glm::vec3 nextPos = glm::vec3(0.0f);
	glm::vec3 currAngle = glm::vec3(0.0f, 1.0f, 0.0f);
	std::stack<glm::vec3> stack;

	float xn, yn, zn;
	glm::vec3 color = BROWN;
	
	for (std::string::iterator it = commandString.begin(); it != commandString.end(); ++it) {
		switch (*it) {
		case('F'):
		case('f'):
			// move forward
			nextPos = currPos + (currAngle * distance);
			nextPos = fudge(nextPos);
			//drawCylinderAroundLine(currPos, nextPos, 1.0f /*TODO radius*/, distance, color);
			vertices.push_back(currPos);
			vertices.push_back(nextPos);
			normals.push_back(color);
			normals.push_back(color);
			currPos = nextPos;
			break;
		case('+'):
			// turn +
			xn = (currAngle.x * glm::cos(theta_rads)) - (currAngle.y * glm::sin(theta_rads));
			yn = (currAngle.x * glm::sin(theta_rads)) + (currAngle.y * glm::cos(theta_rads));
			currAngle.x = xn;
			currAngle.y = yn;
			break;
		case('-'):
			// turn -
			xn = (currAngle.x * glm::cos(-theta_rads)) - (currAngle.y * glm::sin(-theta_rads));
			yn = (currAngle.x * glm::sin(-theta_rads)) + (currAngle.y * glm::cos(-theta_rads));
			currAngle.x = xn;
			currAngle.y = yn;
			break;
		case('&'):
			// turn + in Z
			yn = (currAngle.y * glm::cos(theta_rads)) - (currAngle.z * glm::sin(theta_rads));
			zn = (currAngle.y * glm::sin(theta_rads)) + (currAngle.z * glm::cos(theta_rads));
			currAngle.y = yn;
			currAngle.z = zn;
			break;
		case('^'):
			// turn - in Z
			yn = (currAngle.y * glm::cos(-theta_rads)) - (currAngle.z * glm::sin(-theta_rads));
			zn = (currAngle.y * glm::sin(-theta_rads)) + (currAngle.z * glm::cos(-theta_rads));
			currAngle.y = yn;
			currAngle.z = zn;
			break;
		case('['):
			// save current position and angle on stack
			stack.push(currPos);
			stack.push(currAngle);
			stack.push(color);
			break;
		case(']'):
			// restore position and angle from stack
			color = stack.top();
			stack.pop();
			currAngle = stack.top();
			stack.pop();
			currPos = stack.top();
			stack.pop();
			break;
		case('0'):
			color = BROWN;
			break;
		case('1'):
			color = RED;
			break;
		case('2'):
			color = PINK;
			break;
		case('3'):
			color = PURPLE;
			break;
		case('4'):
			color = GREEN;
			break;
		case('5'):
			color = YELLOW;
			break;
		default:
			break;
		}
		currAngle = glm::normalize(currAngle);
	}

	std::vector<glm::vec3> vertices_compiled;
	std::vector<glm::vec3> normals_compiled;

	// Push cylinder vertices
	for (int i = 0; i < cylinders_v.size(); ++i) {
		for (int j = 0; j < cylinders_v[i].size(); ++j) {
			vertices_compiled.push_back(cylinders_v[i][j]);
		}
	}

	// Push cap vertices
	for (int i = 0; i < caps_v.size(); ++i) {
		for (int j = 0; j < caps_v[i].size(); ++j) {
			vertices_compiled.push_back(caps_v[i][j]);
		}
	}

	// Push cylinder normals
	for (int i = 0; i < cylinders_n.size(); ++i) {
		for (int j = 0; j < cylinders_n[i].size(); ++j) {
			normals_compiled.push_back(cylinders_n[i][j]);
		}
	}

	// Push cap normals
	for (int i = 0; i < caps_n.size(); ++i) {
		for (int j = 0; j < caps_n[i].size(); ++j) {
			normals_compiled.push_back(caps_n[i][j]);
		}
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBON);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBON);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_compiled.size() * sizeof(glm::vec3), vertices_compiled.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBON);
	glBufferData(GL_ARRAY_BUFFER, normals_compiled.size() * sizeof(glm::vec3), normals_compiled.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ProceduralPlant::~ProceduralPlant() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBON);
	glDeleteBuffers(1, &EBO);
}

void ProceduralPlant::draw(GLuint shader) {
	glm::mat4 view = Window::V * this->toWorld;

	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelview"), 1, GL_FALSE, &view[0][0]);

	glBindVertexArray(VAO);

	glLineWidth(lineWidth);
	glDrawArrays(GL_LINES, 0, vertices.size());
	/*
	for (int i = 0; i < cylinders_v.size(); ++i) {
		//glDrawArrays(GL_TRIANGLE_STRIP, cylinders_v[0].size() * i, cylinders_v[i].size());
	}
	int totalCylIndex = cylinders_v.size() * cylinders_v[0].size();
	for (int i = 0; i < caps_v.size(); ++i) {
		//glDrawArrays(GL_TRIANGLE_FAN, totalCylIndex + caps_v[0].size() * i, caps_v[i].size());
	}
	*/
	glBindVertexArray(0);
}

void ProceduralPlant::setToWorld(glm::mat4 M) {
	toWorld = M;
}