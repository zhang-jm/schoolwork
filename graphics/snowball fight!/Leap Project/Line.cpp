#include "Line.h"
#include "Window.h"
#include <Windows.h>

Line::Line()
{
	toWorld = glm::mat4(1.0f);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Line::~Line()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Line::setEndPoints(glm::vec3 start, glm::vec3 directionVector) {
	this->endpoints.clear();
	glm::vec3 cameraPos = glm::vec3(Window::V[3]);
	glm::vec3 startPt = start / 15.0f - glm::vec3(0.0f, 10.0f, 8.0f) - cameraPos;

	//glm::vec3 startPt = start - glm::vec3(0.0f, 100.0f, -50.0f);

	//glm::quat testQuat = glm::toQuat(orientation);

	//glm::vec3 inverted = glm::vec3(glm::axis(testQuat).x, -glm::axis(testQuat).y * 10.0f, glm::axis(testQuat).z);
	//glm::vec3 end = start - (inverted * 30.0f);
	glm::vec3 end = startPt + glm::vec3(0.0f, 0.0f, -30.0f);

	//glm::vec3 euler = glm::eulerAngles(testQuat);
	//glm::vec3 directionVector = glm::vec3(sin(euler.y), -(sin(-euler.z) * cos(euler.y)) + 0.4, (cos(-euler.z) * cos(euler.y)));
	//glm::vec3 directionVector = glm::vec3(-(cos(-euler.x) * cos(euler.y)), -(sin(-euler.x) * cos(euler.y)) + 0.4, sin(euler.y));


	//char buff[100];
	//sprintf_s(buff, "Rad: %f %f %f \n", directionVector.x, directionVector.y, directionVector.z);
	//OutputDebugStringA(buff);
	
	end = startPt + (directionVector * 30.0f);
	//glm::vec4 end2 = glm::vec4(end, 1.0f)*orientation;
	//end = glm::vec3(end2.x, end2.y, end2.z);

	//calculate end

	endpoints.push_back(startPt);
	//endpoints.push_back(end);
	endpoints.push_back(end);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, endpoints.size() * sizeof(glm::vec3), endpoints.data(), GL_STATIC_DRAW);
}

void Line::draw(float color, GLuint shaderProgram)
{
	glm::mat4 mv = Window::V*toWorld;
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	GLuint uPressed = glGetUniformLocation(shaderProgram, "pressed");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &mv[0][0]);
	glUniform1f(uPressed, color);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, endpoints.size());
	glBindVertexArray(0);
}