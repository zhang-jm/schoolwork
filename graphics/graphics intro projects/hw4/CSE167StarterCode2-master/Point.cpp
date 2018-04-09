#include "Point.h"
#include "Window.h"

Point::Point(float x, float y, float z, int id, float color)
{
	location.push_back(glm::vec3(x, y, z));
	this->ID = id;
	this->color = color;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, location.size() * sizeof(glm::vec3), location.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Point::~Point()
{
}

void Point::draw(GLuint shaderProgram)
{
	GLuint uColor = glGetUniformLocation(shaderProgram, "color");
	glUniform1i(uColor, color);

	glPointSize(7.0f);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, location.size() * sizeof(glm::vec3), location.data(), GL_STATIC_DRAW);
	glDrawArrays(GL_POINTS, 0, location.size());
	glBindVertexArray(0);
}

void Point::selectionDraw(GLuint shaderProgram) 
{
	glm::mat4 view = Window::V * toWorld;

	GLuint uProjection = glGetUniformLocation(shaderProgram, "projection");
	GLuint uModelview = glGetUniformLocation(shaderProgram, "modelview");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &view[0][0]);

	// << location.at(0).x << endl;
	glPointSize(10.0f);

	GLuint uColor = glGetUniformLocation(shaderProgram, "color");
	glUniform1i(uColor, 5);

	GLuint uid = glGetUniformLocation(shaderProgram, "id");
	glUniform1i(uid, ID + 1);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, location.size() * sizeof(glm::vec3), location.data(), GL_STATIC_DRAW);
	glDrawArrays(GL_POINTS, 0, location.size());
	glBindVertexArray(0);
}

void Point::update(float x, float y, float z) {
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	glm::vec4 pointloc = glm::vec4(location.at(0), 1.0f);
	glm::vec4 newloc = trans * pointloc;

	location.at(0) = glm::vec3(newloc.x, newloc.y, newloc.z);
}