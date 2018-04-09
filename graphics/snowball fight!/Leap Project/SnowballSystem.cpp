#include "SnowballSystem.h"
#include "Window.h"

SnowballSystem::SnowballSystem(bool isOpponent)
{
	//snowball = new Model("../assets/articuno/articuno-pokemon-go.obj");
	snowball = new Model("../assets/snowball/Snowball.obj");	

	this->holdingSnowball = false;
	this->isEnemy = isOpponent;

	spawnSnowball();


	//snowballInHand = new Snowball(snowball, false);
	//snowballs.push_back(snowballInHand);
}

SnowballSystem::~SnowballSystem()
{
	for (Snowball * m : snowballs) {
		delete(m);
	}

	delete(snowball);
}

void SnowballSystem::drawParticles(float time, GLuint shaderProgram, glm::vec3 handPos, bool inputDown, glm::vec3 direction)
{
	//std::cout << snowballs.size() << std::endl;
	if (holdingSnowball) {

		//draw non-grabbed snowballs on the ground
		for (Snowball * sb : snowballs) {			
			if (sb != snowballInHand && !isEnemy) sb->draw(time, shaderProgram, glm::vec3(0.0f, -50.0f, 0.0f), false, direction);
		}

		if (!isEnemy) {
			//draw grabbed snowball on hand
			snowballInHand->draw(time, shaderProgram, handPos, inputDown, direction);
		}
		else {
			snowballInHand->draw(time, shaderProgram, glm::vec3(-5000.0f, 0.0f, 0.0f), false, glm::vec3(0.0f, 0.0f, 0.0f));
		}

		//Once shot, stop keeping track of snowball
		if (snowballInHand->outOfBounds) removeSnowball();

	}
	else {
		for (Snowball * sb : snowballs) {
			sb->draw(time, shaderProgram, glm::vec3(0.0f, -50.0f, 0.0f), false, direction);
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
		snowballs.back()->updatePosition(glm::vec3(0.0f, -10.0f, 0.0f));
		snowballs.push_back(new Snowball(snowball, false));
		snowballs.back()->updatePosition(glm::vec3(4.0f, -10.0f, 0.0f));
		particleCount = 3;
	}
	else {
		particleCount = 1;
		snowballs.push_back(new Snowball(snowball, false));
		snowballs.back()->updatePosition(glm::vec3(-10.0f, 0.0f, -10.0f));
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