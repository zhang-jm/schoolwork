#include "Player.h"
//#include "Window.h"

Player::Player(glm::vec3 initPos, bool isOpponent) {
	
	mySnowballs = new SnowballSystem(isOpponent);
	this->handPosition = glm::vec3(0.0f);
	this->handAngle = glm::vec3(0.0f);
	this->headPosition = initPos;
	this->isOpponent = isOpponent;
	

	toWorld = glm::mat4(1.0f);	
	toWorld = toWorld * glm::translate(glm::mat4(1.0f), headPosition);
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	//if (isOpponent) toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 0.0f , glm::vec3(0.0f, 1.0f, 0.0f));
}

Player::~Player() {

}

void Player::setModel() {
	this->model = new Model("./assets/articuno/articuno.obj");
}

void Player::draw(GLuint snowballShaderProgram, float time, glm::mat4 projection, glm::mat4 modelview, Line* rightController, bool isArticuno) {
	
	if (isOpponent) {		
		model->Draw(toWorld * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -20.0f, 50.0f)), snowballShaderProgram,projection, modelview, isArticuno);
		
	}
	//else {
	if(isArticuno)mySnowballs->drawParticles(time, snowballShaderProgram, projection, modelview, rightController);
	//}
	
}

void Player::move(glm::vec3 newPos) {
	
	if (!isOpponent) {
		if (newPos.x < -50.0f || newPos.x > 50.0f || newPos.z < 25.0f || newPos.z > 75.0f) {

		}
		/*if (newPos.x < -50.0f || newPos.x > 50.0f || newPos.z < 0.0f || newPos.z > 50.0f) {

		}
		*/else {
		// toWorld = toWorld * glm::translate(glm::mat4(1.0f), (newPos - headPosition));
			this->headPosition = newPos;
			toWorld = glm::mat4(1.0f);
			toWorld = toWorld * glm::translate(glm::mat4(1.0f), headPosition);
			toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
		}
	}
	else {
		this->headPosition = newPos;
		toWorld = glm::mat4(1.0f);
		toWorld = toWorld * glm::translate(glm::mat4(1.0f), headPosition);
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	}
		
	
}

bool Player::snowballCollision() {
	return false;
}

void Player::grabSnowball() {
	mySnowballs->grabNextSnowball();
}

