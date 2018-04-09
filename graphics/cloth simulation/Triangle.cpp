#include "Triangle.h"

Triangle::Triangle() {
	rho = 1.22;
	cdrag = 0.5;
	vair = glm::vec3(0.0f, 0.0f, 0.0001f);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBON);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBON);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Triangle::~Triangle() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBON);
}

void Triangle::setParticles(Particle * p1, Particle * p2, Particle * p3) {
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
}

void Triangle::computeForce() {
	float onethird = 1.0f / 3.0f;
	//cout << onethird << endl;
	glm::vec3 vsurface = (p1->velocity + p2->velocity + p3->velocity) * onethird;
	//cout << vsurface.x << " " << vsurface.y << " " << vsurface.z << endl;

	glm::vec3 v = vsurface - vair;

	glm::vec3 pos = glm::cross((p2->position - p1->position), (p3->position - p1->position));
	n = glm::normalize(pos);
	//cout << pos.x << " " << pos.y << " " << pos.z << endl;
	//cout << n.x << " " << n.y << " " << n.z << endl;

	float aknot = glm::length(pos) / 2;
	//cout << glm::length(pos) << endl;
	//cout << aknot << endl;
	float a = aknot * (glm::dot(v, n) / glm::length(v));
	//cout << a << endl;

	float f = -0.5f * rho * pow(glm::length(v), 2) * cdrag * a;
	//cout << f << endl;
	glm::vec3 fair = n * f;

	glm::vec3 fairSplit = fair * onethird;
	//cout << fairSplit.x << " " << fairSplit.y << " " << fairSplit.z << endl;
	p1->applyForce(fairSplit);
	p2->applyForce(fairSplit);
	p3->applyForce(fairSplit);
}

void Triangle::draw(GLuint shaderProgram) {
	glm::mat4 modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	std::vector<glm::vec3> positions;
	positions.push_back(p1->position);
	positions.push_back(p2->position);
	positions.push_back(p3->position);

	std::vector<glm::vec3> normal;
	normal.push_back(p1->normal);
	normal.push_back(p2->normal);
	normal.push_back(p3->normal);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBON);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), normal.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}