#include "Cloth.h"
#include <cmath>

using namespace std;

Cloth::Cloth() {
}

Cloth::~Cloth() {
	for (Particle * p : particles) {
		delete(p);
	}

	for (SpringDamper * s : dampers) {
		delete(s);
	}
}

void Cloth::init(float width, float height) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Particle * newParticle = new Particle();
			newParticle->position = glm::vec3(i, j + 2.0f, ((double)rand() / (RAND_MAX)));
			particles.push_back(newParticle);
		}
	}

	for (int i = 0; i < width - 1; i++) {
		for (int j = 0; j < height - 1; j++) {
			Particle * p1 = particles.at(i + j * width);
			Particle * p2 = particles.at((i + j * width) + 1);
			Particle * p3 = particles.at(i + (j + 1) * width);
			Particle * p4 = particles.at(((i + 1) + (j + 1) * width));

			SpringDamper * damper1 = new SpringDamper();
			SpringDamper * damper2 = new SpringDamper();
			SpringDamper * damper3 = new SpringDamper();

			damper1->setParticles(p1, p2);
			damper2->setParticles(p1, p3);
			damper3->setParticles(p1, p4);

			damper1->restLength = std::abs(glm::length(p1->position - p2->position));
			damper2->restLength = std::abs(glm::length(p1->position - p3->position));
			damper3->restLength = std::abs(glm::length(p1->position - p4->position));

			cout << std::abs(glm::length(p1->position - p2->position)) << " " << std::abs(glm::length(p1->position - p3->position))
				<< " " << std::abs(glm::length(p1->position - p4->position));

			dampers.push_back(damper1);
			dampers.push_back(damper2);
			dampers.push_back(damper3);

			Triangle * triangle1 = new Triangle();
			Triangle * triangle2 = new Triangle();

			triangle1->setParticles(p2, p1, p3);
			triangle2->setParticles(p2, p3, p4);

			triangles.push_back(triangle1);
			triangles.push_back(triangle2);
		}
	}

	for (int j = 0; j < height - 1; j++) {
		SpringDamper * damper1 = new SpringDamper();
		damper1->setParticles(particles.at(j + (height - 1) * width), particles.at((j + 1) + (height - 1) * width));

		dampers.push_back(damper1);
	}
	cout << "here" << endl;
	for (int i = 0; i < width - 1; i++) {
		SpringDamper * damper1 = new SpringDamper();

		cout << ((i + 1) * height) - 1 << " " << ((i + 2) * height) - 1 << endl;
		damper1->setParticles(particles.at(((i + 1) * height) - 1), particles.at(((i + 2) * height) - 1));

		dampers.push_back(damper1);
	}

	std::cout << particles.size() << std::endl;
}

void Cloth::initFixTopRow(float width, float height) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Particle * newParticle = new Particle();
			newParticle->position = glm::vec3(i, j + 2.0f, ((double)rand() / (RAND_MAX)));
			particles.push_back(newParticle);

			if (j == height - 1) {
				newParticle->fixed = true;
			}
		}
	}

	for (int i = 0; i < width - 1; i++) {
		for (int j = 0; j < height - 1; j++) {
			Particle * p1 = particles.at(i + j * width);
			Particle * p2 = particles.at((i + j * width) + 1);
			Particle * p3 = particles.at(i + (j + 1) * width);
			Particle * p4 = particles.at(((i + 1) + (j + 1) * width));

			SpringDamper * damper1 = new SpringDamper();
			SpringDamper * damper2 = new SpringDamper();
			SpringDamper * damper3 = new SpringDamper();
			SpringDamper * damper4 = new SpringDamper();

			damper1->setParticles(p1, p2);
			damper2->setParticles(p1, p3);
			damper3->setParticles(p1, p4);
			damper4->setParticles(p2, p3);

			damper1->restLength = std::abs(glm::length(p1->position - p2->position));
			damper2->restLength = std::abs(glm::length(p1->position - p3->position));
			damper3->restLength = std::abs(glm::length(p1->position - p4->position));
			damper4->restLength = std::abs(glm::length(p2->position - p3->position));

			cout << std::abs(glm::length(p1->position - p2->position)) << " " << std::abs(glm::length(p1->position - p3->position))
				<< " " << std::abs(glm::length(p1->position - p4->position));

			dampers.push_back(damper1);
			dampers.push_back(damper2);
			dampers.push_back(damper3);
			dampers.push_back(damper4);

			Triangle * triangle1 = new Triangle();
			Triangle * triangle2 = new Triangle();

			triangle1->setParticles(p2, p1, p3);
			triangle2->setParticles(p2, p3, p4);

			triangles.push_back(triangle1);
			triangles.push_back(triangle2);
		}
	}

	for (int j = 0; j < height - 1; j++) {
		SpringDamper * damper1 = new SpringDamper();
		damper1->setParticles(particles.at(j + (height - 1) * width), particles.at((j + 1) + (height - 1) * width));

		dampers.push_back(damper1);
	}
	cout << "here" << endl;
	for (int i = 0; i < width - 1; i++) {
		SpringDamper * damper1 = new SpringDamper();

		cout << ((i + 1) * height) - 1 << " " << ((i + 2) * height) - 1 << endl;
		damper1->setParticles(particles.at(((i + 1) * height) - 1), particles.at(((i + 2) * height) - 1));

		dampers.push_back(damper1);
	}

	std::cout << particles.size() << std::endl;
}

void Cloth::initFixTopCorners(float width, float height) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Particle * newParticle = new Particle();
			newParticle->position = glm::vec3(i / 2.0f, (j / 2.0f) + 3.0f, ((double)rand() / (RAND_MAX)));
			particles.push_back(newParticle);

			if ((j == height - 1 && i == 0) || (j == height - 1 && i == width - 1)) {
				newParticle->fixed = true;
			}
		}
	}

	for (int i = 0; i < width - 1; i++) {
		for (int j = 0; j < height - 1; j++) {
			Particle * p1 = particles.at(i + j * width);
			Particle * p2 = particles.at((i + j * width) + 1);
			Particle * p3 = particles.at(i + (j + 1) * width);
			Particle * p4 = particles.at(((i + 1) + (j + 1) * width));

			SpringDamper * damper1 = new SpringDamper();
			SpringDamper * damper2 = new SpringDamper();
			SpringDamper * damper3 = new SpringDamper();
			SpringDamper * damper4 = new SpringDamper();

			damper1->setParticles(p1, p2);
			damper2->setParticles(p1, p3);
			damper3->setParticles(p1, p4);
			damper4->setParticles(p2, p3);

			damper1->restLength = std::abs(glm::length(p1->position - p2->position));
			damper2->restLength = std::abs(glm::length(p1->position - p3->position));
			damper3->restLength = std::abs(glm::length(p1->position - p4->position));
			damper4->restLength = std::abs(glm::length(p2->position - p3->position));

			cout << std::abs(glm::length(p1->position - p2->position)) << " " << std::abs(glm::length(p1->position - p3->position))
				<< " " << std::abs(glm::length(p1->position - p4->position));

			dampers.push_back(damper1);
			dampers.push_back(damper2);
			dampers.push_back(damper3);
			dampers.push_back(damper4);

			Triangle * triangle1 = new Triangle();
			Triangle * triangle2 = new Triangle();

			triangle1->setParticles(p2, p1, p3);
			triangle2->setParticles(p2, p3, p4);

			triangles.push_back(triangle1);
			triangles.push_back(triangle2);
		}
	}

	for (int j = 0; j < height - 1; j++) {
		SpringDamper * damper1 = new SpringDamper();
		damper1->setParticles(particles.at(j + (height - 1) * width), particles.at((j + 1) + (height - 1) * width));

		dampers.push_back(damper1);
	}
	cout << "here" << endl;
	for (int i = 0; i < width - 1; i++) {
		SpringDamper * damper1 = new SpringDamper();

		cout << ((i + 1) * height) - 1 << " " << ((i + 2) * height) - 1 << endl;
		damper1->setParticles(particles.at(((i + 1) * height) - 1), particles.at(((i + 2) * height) - 1));

		dampers.push_back(damper1);
	}

	std::cout << particles.size() << std::endl;
}

void Cloth::draw(GLuint shaderProgram, GLuint triangleShader) {
	for (Particle * p : particles) {
		//p->draw(shaderProgram);
	}
	
	for (SpringDamper * s : dampers) {
		//s->draw(shaderProgram);
	}

	glUseProgram(triangleShader);

	for (Triangle * t : triangles) {
		t->draw(triangleShader);
	}
}

void Cloth::update(float deltaTime) {
	//std::cout << particles.size() << std::endl;
	for (int i = 0; i < particles.size(); i++) {
		Particle * p = particles.at(i);
		p->normal = glm::vec3(0.0f);
		p->applyGravity();
	}

	for (SpringDamper * s : dampers) {
		s->computeForce();
	}

	for (Triangle * t : triangles) {
		glm::vec3 pos = glm::cross((t->p2->position - t->p1->position), (t->p3->position - t->p1->position));
		t->n = glm::normalize(pos);
		t->p1->normal += t->n;
		t->p2->normal += t->n;
		t->p3->normal += t->n;
		t->computeForce();
	}

	for (Particle * p : particles) {
		p->normal = glm::normalize(p->normal);
		p->update(deltaTime);
	}
}

void Cloth::moveFixedParticles(std::string s) {
	for (Particle * p : particles) {
		if (p->fixed == true) {
			if (s == "up") {
				p->position += glm::vec3(0.0f, 0.1f, 0.0f);
			}
			else if (s == "down") {
				p->position -= glm::vec3(0.0f, 0.1f, 0.0f);
			}
			if (s == "left") {
				p->position -= glm::vec3(0.1f, 0.0f, 0.0f);
			}
			if (s == "right") {
				p->position += glm::vec3(0.1f, 0.0f, 0.0f);
			}
		}
	}
	
}

void Cloth::changeWindSpeed(bool increase) {
	for (Triangle * t : triangles) {
		if (increase) {
			t->vair += glm::vec3(0.0f, 0.0f, 0.5f);
		}
		else {
			t->vair -= glm::vec3(0.0f, 0.0f, 0.5f);
		}
	}
}