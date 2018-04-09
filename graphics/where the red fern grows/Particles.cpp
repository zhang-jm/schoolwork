#include "Particles.h"
#include "Window.h"
#include "Terrain.h"

Particles::Particles(GLuint shaderProg)
{
	toWorld = glm::mat4(1.0f);

	shader = shaderProg;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	srand(time(NULL));
}

Particles::~Particles()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Particles::draw(GLuint shaderProgram)
{
	glm::mat4 view = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &view[0][0]);

	glPointSize(3.0f);

	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, points.size());
	glBindVertexArray(0);
}

void Particles::setup(int n) {
	size = pow(2, n) + 1;
	
	//create 1000 particles
	for (int i = 0; i < 1000; i++) {
		//find a random spot for the particle to drop
		double x = dRand(0, size);
		double y = dRand(10, 200);
		double z = dRand(0, size);

		points.push_back(glm::vec3(x, y, z));
		times.push_back(0);
	}

	//update the buffers
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Particles::update() {
	for (int i = 0; i < 1000; i++) {
		float height = -30.0f;

		//if point falls too far wait for a bit, then reset
		if (points.at(i).y <= height) {
			
			//wait some amount of frames
			if (times.at(i) < dRand(100, 1000)) {
				times.at(i) = times.at(i) + 1;

				points.at(i).x = points.at(i).x;
				points.at(i).y = height;
				points.at(i).z = points.at(i).z;
			}
			
			//reset
			else {
				points.at(i).x = dRand(0, size);
				points.at(i).y = 200;
				points.at(i).z = dRand(0, size);

				times.at(i) = 0;
			}
		}

		//drop point
		else {
			points.at(i).x = points.at(i).x - dRand(0.001, 0.1);
			points.at(i).y = points.at(i).y - dRand(0.01, 0.2);
			points.at(i).z = points.at(i).z - dRand(0.001, 0.1);
		}
	}

	//update the buffers
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}


//taken from https://github.com/ROUKIEN/DiamondSquare/blob/master/src/DiamondSquare.cpp
//generates random double within the range
double Particles::dRand(double dMin, double dMax)
{
	double d = (double)rand() / RAND_MAX;
	return dMin + (d * (dMax - dMin));
}
