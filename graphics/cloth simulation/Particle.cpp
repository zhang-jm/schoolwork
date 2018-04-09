#include "Particle.h"

Particle::Particle() {
	mass = 1.0f;
	position = glm::vec3(0.0f);
	velocity = glm::vec3(0.0f);
	fixed = false;
	friction = 0.5f;
	elasticity = 0.1f;


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

Particle::~Particle() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Particle::draw(GLuint shaderProgram) {
	glm::mat4 modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	glPointSize(5.0f);

	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void Particle::update(float deltaTime) {
	if (!fixed) {
		glm::vec3 accel = (1 / mass) * force;

		velocity = velocity + (accel * deltaTime);
		position = position + (velocity * deltaTime);
		//cout << position.x << " " << position.y << " " << position.z << endl;

		if (position.y < 0) {
			position.y = 0 - position.y;

			velocity.y = -elasticity * velocity.y;
			velocity.x = (1 - friction) * velocity.x;
			velocity.z = (1 - friction) * velocity.z;
		}
	}

	force = glm::vec3(0.0f);

	positions.clear();
	positions.push_back(position);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Particle::applyForce(glm::vec3 f) {
	force = force + f;
}

void Particle::applyGravity() {
	glm::vec3 gravity = mass * glm::vec3(0.0, -0.05f, 0.0f);
	applyForce(gravity);
}