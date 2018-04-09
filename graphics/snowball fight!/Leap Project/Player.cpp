#include "Player.h"
//#include "Window.h"

Player::Player(glm::vec3 initPos, bool isOpponent) {
	
	mySnowballs = new SnowballSystem(isOpponent);
	this->handPosition = glm::vec3(0.0f);
	this->handAngle = glm::vec3(0.0f);
	this->headPosition = initPos;
	this->isOpponent = isOpponent;
	
	toWorld = toWorld * glm::translate(glm::mat4(1.0f), headPosition);
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 0.15f));
	//if (isOpponent) toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 0.0f , glm::vec3(0.0f, 1.0f, 0.0f));
}

Player::~Player() {

}

void Player::setModel() {
	this->model = new Model("../assets/articuno2/articuno.obj");
}

void Player::draw(GLuint snowballShaderProgram, float time, bool inputDown, bool isArticuno) {
	
	if (isOpponent) {		
		glUniform1i(glGetUniformLocation(snowballShaderProgram, "useNormals"), 1);
		model->Draw(toWorld * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -25.0f, 0.0f)), snowballShaderProgram, isArticuno);
	}
	//else {
		glUniform1i(glGetUniformLocation(snowballShaderProgram, "useNormals"), 0);
		if (isArticuno)mySnowballs->drawParticles(time, snowballShaderProgram, handPosition, inputDown, handAngle);
	//}
	
}

void Player::move(glm::vec3 newPos) {
	
	if (!isOpponent) {
		if (newPos.x < -50.0f || newPos.x > 50.0f || newPos.z < 25.0f || newPos.z > 75.0f) {

		}
		/*if (newPos.x < -50.0f || newPos.x > 50.0f ) {

		}*/
		else {
			//toWorld = toWorld * glm::translate(glm::mat4(1.0f), 5.0f*(newPos - headPosition));
			this->headPosition = newPos;
			toWorld = glm::mat4(1.0f);
			toWorld = toWorld * glm::translate(glm::mat4(1.0f), headPosition);
			toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 0.15f));

		}
	}
	else {
		this->headPosition = newPos;
		toWorld = glm::mat4(1.0f);
		toWorld = toWorld * glm::translate(glm::mat4(1.0f), headPosition);
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 0.15f));
	}
	
}

bool Player::snowballCollision() {
	return false;
}

void Player::grabSnowball() {
	mySnowballs->grabNextSnowball();
}

