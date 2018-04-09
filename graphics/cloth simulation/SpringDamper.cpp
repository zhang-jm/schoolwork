#include "SpringDamper.h"

SpringDamper::SpringDamper() {
	springConstant = 10.0f;
	dampingFactor = 10.0f;
	restLength = 1.0f;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

SpringDamper::~SpringDamper() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void SpringDamper::setParticles(Particle * p1, Particle * p2) {
	this->p1 = p1;
	this->p2 = p2;
}

void SpringDamper::computeForce() {
	glm::vec3 estar = p2->position - p1->position;
	float l = glm::length(estar);
	glm::vec3 e = glm::normalize(estar);

	float v1 = glm::dot(e, p1->velocity);
	float v2 = glm::dot(e, p2->velocity);

	float fsd = -(springConstant * (restLength - l)) - ((dampingFactor) * (v1 - v2));
	glm::vec3 f1 = fsd * e;
	glm::vec3 f2 = -f1;

	//cout << f1.x << " " << f1.y << " " << f1.z << endl;

	p1->applyForce(f1);
	p2->applyForce(f2);
}

void SpringDamper::draw(GLuint shaderProgram) {
	glm::mat4 modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	std::vector<glm::vec3> positions;
	positions.push_back(p1->position);
	positions.push_back(p2->position);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}