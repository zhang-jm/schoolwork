#include "Snowball.h"
#include "Window.h"

Snowball::Snowball(Model * model, bool over)
{
	this->model = model;
	this->over = over;
	this->grabbed = false;
	toWorld = glm::mat4(1.0f);
	this->outOfBounds = false;

	this->shooting = false;
	this->aim = glm::vec3(0.0f);	

	if (!over) {
		position = glm::vec3(-4.0f, -10.0f, 0.0f);
		velocity = glm::vec3(RandomNumber(-1.0f, 1.0f) + 0.05f, RandomNumber(0.0f, 1.0f) + 0.05f, 0.0f);

		spinAmount = (float)(RandomNumber(-1.0f, 1.0f));
		spinAxis = glm::vec3(RandomNumber(-1.0f, 1.0f), RandomNumber(-1.0f, 1.0f), RandomNumber(-1.0f, 1.0f));
	}
	else {
		position = glm::vec3(RandomNumber(-20.0f, 20.0f), RandomNumber(-20.0f, 20.0f), RandomNumber(-20.0f, 20.0f));
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		spinAmount = 0.0f;
		spinAxis = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// If you haven't figured it out from the last project, this is how you fix spin's behavior
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	toWorld = toWorld * glm::translate(glm::mat4(1.0f), position);
}

Snowball::~Snowball()
{
}


void Snowball::draw(float time, GLuint shaderProgram, glm::vec3 handPos, bool inputDown, glm::vec3 direction)
{
	if (inputDown) {
		aim = direction;
		shooting = true;
		//shooting = !shooting;
	}


	if (!over) {
		//updatePosition(time);
		//spin();
		if (!(handPos.x == -5000)) {
			outOfBounds = move(handPos, time, aim);
		}
		spin();

	}

	model->Draw(toWorld, shaderProgram, false);
}

void Snowball::updatePosition(glm::vec3 delta)
{
	position = delta;
	if (position.x < -50.0f || position.x > 50.0f || position.y < -10.0f || position.y > 50.0f || position.z < -50.0f) {
		position = glm::vec3(0.0f, 0.0f, -40.0f);
	}
	toWorld = glm::mat4(1.0f);
	toWorld = toWorld * glm::translate(glm::mat4(1.0f), position);
	
	// x_new = x_old + vt
	/*position = position + (time * velocity);
	toWorld = glm::mat4(1.0f);
	toWorld = toWorld * glm::translate(glm::mat4(1.0f), position) * rotate;
	if (position.x > 3.0f || position.x < -3.0f) {
		velocity.x = -velocity.x;
		if (position.x > 3.0f) {
			position.x = 3.0f;
		}
		else {
			position.x = -3.0f;
		}
	}

	if (position.y > 3.0f || position.y < -3.0f) {
		velocity.y = -velocity.y;
		if (position.y > 3.0f) {
			position.y = 3.0f;
		}
		else {
			position.y = -3.0f;
		}
	}

	if (position.z > 3.0f || position.z < -3.0f) {
		velocity.z = -velocity.z;
		if (position.z > 3.0f) {
			position.z = 3.0f;
		}
		else {
			position.z = -3.0f;
		}
	}*/
	//std::cout << position.x << " " << position.y << " " << position.z << std::endl;
}

bool Snowball::move(glm::vec3 handPos, float time, glm::vec3 direction) {
	
	glm::vec3 cameraPos = glm::vec3(Window::V[3]);
	if (!shooting && grabbed) {
		position = handPos / 15.0f - glm::vec3(0.0f, 11.0f, 5.0f) - cameraPos;
		//position = handPos - glm::vec3(0.0f, 101.0f, -50.0f);
		toWorld = glm::mat4(1.0f);
		toWorld = toWorld * glm::translate(glm::mat4(1.0f), position );// * rotate;
		return false;
	}
	else if (shooting){
		grabbed = false;
		//position = position + glm::vec3(0.0f, 0.1f, -0.5f);// +(time / 100.0f * direction);
		position = position +(time*10.0f * direction);
		//std::cout << direction.x << " " << direction.y << " " << direction.z << std::endl;
		toWorld = glm::mat4(1.0f);// *glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.1f, -0.5f));

		toWorld = toWorld * glm::translate(glm::mat4(1.0f), position);// *rotate;
		//toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
		if (position.x < -50.0f || position.x > 50.0f || position.y < -10.0f || position.y > 50.0f || position.z < -50.0f) {
			//position = handPos / 15.0f - glm::vec3(0.0f, 11.0f, 5.0f) - cameraPos;
			shooting = false;
			return true;
			//grabbed = true;
		}
		else {
			return false;
		}
	}
}

void Snowball::spin()
{
	// If you haven't figured it out from the last project, this is how you fix spin's behavior
	rotate = rotate * glm::rotate(glm::mat4(1.0f), spinAmount / 180.0f * glm::pi<float>(), spinAxis);
}

float Snowball::RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}