#include "SnowballSystem.h"
#include "Window.h"
#include <Windows.h>

SnowballSystem::SnowballSystem(bool isOpponent)
{
	snowball = new Model("./assets/snowball/Snowball.obj");

	this->holdingSnowball = false;
	this->isEnemy = isOpponent;

	spawnSnowball();
}

SnowballSystem::~SnowballSystem()
{
	for (Snowball * m : snowballs) {
		delete(m);
	}

	delete(snowball);	
}

void SnowballSystem::drawParticles(float time, GLuint shaderProgram, const glm::mat4 & projection, const glm::mat4 & modelview, Line* rightController)
{
	char buff[100];
	sprintf_s(buff, "Particle count: %d\n", particleCount);
	OutputDebugStringA(buff);

	float distl, distr;	
	std::vector <glm::vec3> rightPts = rightController->endpoints;

	//std::cout << molecules.size() << std::endl;
	/*for (Molecule * m : molecules) {
		if (!gameOver && m->model != o2) {

			glm::vec3 x1 = leftPts[0];
			glm::vec3 x2 = leftPts[1];
			glm::vec3 x0 = m->position;
			distl = glm::length(glm::cross(x2 - x1, x1 - x0)) / glm::length(x2 - x1);
			x1 = rightPts[0];
			x2 = rightPts[1];
			distr = glm::length(glm::cross(x2 - x1, x1 - x0)) / glm::length(x2 - x1);
			
			char buff[100];
			sprintf_s(buff, "Distr distl: %f %f\n", distr, distl);
			OutputDebugStringA(buff);

			sprintf_s(buff, "Length of line: %f\n", glm::length(rightPts[1] - rightPts[0]));
			//OutputDebugStringA(buff);

			sprintf_s(buff, "Length of cross: %f\n", glm::length(glm::cross(rightPts[1] - rightPts[0], rightPts[0] - m->position)));
			//OutputDebugStringA(buff);

			if (rightController->pressed && distr < m->radius) {
				m->model = o2;
				particleCount = particleCount - 1;

				//char buff[100];
				sprintf_s(buff, "Particle count: %d\n", particleCount);
				//OutputDebugStringA(buff);

				sprintf_s(buff, "Rad: %f\n", m->radius);
				//OutputDebugStringA(buff);

				return true;
			}
		}
		m->draw(time, shaderProgram, projection, modelview);
	}*/

	//std::cout << snowballs.size() << std::endl;
	if (holdingSnowball) {

		if (!isEnemy) {
			//draw grabbed snowball on hand
			snowballInHand->draw(time, shaderProgram, projection, modelview, rightPts[0], rightController->pressed, rightPts[1] - rightPts[0]);
		}
		else {
			snowballInHand->draw(time, shaderProgram, projection, modelview, glm::vec3(-5000.0f, 0.0f, 0.0f), rightController->pressed, glm::vec3(0.0f, 0.0f, 0.0f));
		}

		//draw non-grabbed snowballs on the ground
		for (Snowball * sb : snowballs) {
			if (sb != snowballInHand && !isEnemy) sb->draw(time, shaderProgram, projection, modelview, glm::vec3(0.0f, -50.0f, 0.0f), false, rightPts[1] - rightPts[0]);
		}

		//Once shot, stop keeping track of snowball
		if (snowballInHand->outOfBounds) removeSnowball();
	}
	else {
		for (Snowball * sb : snowballs) {
			sb->draw(time, shaderProgram, projection, modelview, glm::vec3(0.0f, -50.0f, 0.0f), false, rightPts[1] - rightPts[0]);
		}
	}

	if (particleCount == 0) spawnSnowball();
	

}

void SnowballSystem::spawnSnowball()
{
	if (!isEnemy) {
		//Create 3 extra snowballs
		snowballs.push_back(new Snowball(snowball, false));
		snowballs.push_back(new Snowball(snowball, false));
		snowballs.back()->updatePosition(glm::vec3(0.0f, -1.0f, -1.0f));
		snowballs.push_back(new Snowball(snowball, false));
		snowballs.back()->updatePosition(glm::vec3(0.2f, -1.0f, -1.0f));
		particleCount = 3;
	}
	else {
		particleCount = 1;
		snowballs.push_back(new Snowball(snowball, false));
		snowballs.back()->updatePosition(glm::vec3(0.0f, 0.0f, -40.0f));
	}
}

void SnowballSystem::grabNextSnowball() {
	if (!snowballs.empty()) {
		snowballInHand = snowballs.back();
		//snowballs.pop_back();
		snowballInHand->grabbed = true;
		holdingSnowball = true;
	}

}


void SnowballSystem::removeSnowball()
{
	//Crashes program
	if (particleCount != 0) {
		snowballInHand->grabbed = false;
		free(snowballInHand);
		holdingSnowball = false;
		snowballs.pop_back();
		particleCount--;
	}
	else {
		spawnSnowball();
	}


	//grabNextSnowball();

}