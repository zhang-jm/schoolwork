#include "Snowball.h"
#include "Window.h"

Snowball::Snowball(Model * model, bool over)
{
	this->model = model;
	this->over = over;
	toWorld = glm::mat4(1.0f);
	this->radius = 4.0f;
	this->shooting = false;
	this->aim = glm::vec3(0.0f);	
	this->outOfBounds = false;


	if (!over) {
		//position = glm::vec3(0.0f, 0.1f, -0.5f);
		position = glm::vec3(-0.2f, -1.0f, -1.0f);
		velocity = glm::vec3(RandomNumber(-1.0f, 1.0f) + 0.05f, RandomNumber(0.0f, 1.0f) + 0.05f, RandomNumber(0.0f, 1.0f) + 0.05f);

		spinAmount = (float)(RandomNumber(-1.0f, 1.0f));
		spinAxis = glm::vec3(RandomNumber(-1.0f, 1.0f), RandomNumber(-1.0f, 1.0f), RandomNumber(-1.0f, 1.0f));

		
		toWorld = toWorld * glm::translate(glm::mat4(1.0f), position);
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
		//toWorld = toWorld * glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		/*position = glm::vec3(RandomNumber(-20.0f, 20.0f), RandomNumber(-20.0f, 20.0f), RandomNumber(-20.0f, 20.0f));
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		spinAmount = (float)(RandomNumber(-180.0f, 180.0f));
		spinAxis = glm::vec3(RandomNumber(-1.0f, 1.0f), RandomNumber(-1.0f, 1.0f), RandomNumber(-1.0f, 1.0f));

		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
		toWorld = toWorld * glm::translate(glm::mat4(1.0f), position);
		toWorld = toWorld * glm::rotate(glm::mat4(1.0f), spinAmount, spinAxis);*/
	}


}

Snowball::~Snowball()
{
}

void Snowball::draw(float time, GLuint shaderProgram, const glm::mat4 & projection, const glm::mat4 & modelview, glm::vec3 handPos, bool triggerDown, glm::vec3 direction)
{
	if (triggerDown) {
		aim = direction;
		shooting = true;
	}

	if (!over) {
		if (!(handPos.x == -5000)) {
			outOfBounds = move(handPos, time, aim);
		}
		else {
			enemy = true;
		}
		spin();
	}
	
	model->Draw(toWorld, shaderProgram, projection, modelview, false);
}

void Snowball::updatePosition(glm::vec3 delta)
{
	position = delta;
	if (position.x < -50.0f || position.x > 50.0f || position.y < -10.0f || position.y > 50.0f || position.z < -50.0f) {
		position = glm::vec3(0.0f, 0.0f, -40.0f);
	}
	toWorld = glm::mat4(1.0f);
	toWorld = toWorld * glm::translate(glm::mat4(1.0f), position);
	if (!enemy) {
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
	}
	else {
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	}
}


/*void Snowball::updatePosition(float time)
{
	// x_new = x_old + vt
	position = position + (time * velocity);
	toWorld = glm::mat4(1.0f) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.1f, -0.5f));
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
	toWorld = toWorld * glm::translate(glm::mat4(1.0f), position) * rotate;
	if (position.x > 15.0f || position.x < -15.0f) {
		velocity.x = -velocity.x;
		if (position.x > 15.0f) {
			position.x = 15.0f;
		}
		else {
			position.x = -15.0f;
		}
	}

	if (position.y > 25.0f || position.y < 1.0f) {
		velocity.y = -velocity.y;
		if (position.y > 25.0f) {
			position.y = 25.0f;
		}
		else {
			position.y = 1.0f;
		}
	}

	if (position.z > 1.0f || position.z < -20.0f) {
		velocity.z = -velocity.z;
		if (position.z > 1.0f) {
			position.z = 1.0f;
		}
		else {
			position.z = -20.0f;
		}
	}
	std::cout << position.x << " " << position.y << " " << position.z << std::endl;
}*/

void Snowball::spin()
{
	// If you haven't figured it out from the last project, this is how you fix spin's behavior
	rotate = rotate * glm::rotate(glm::mat4(1.0f), spinAmount / 180.0f * glm::pi<float>(), spinAxis);
}

/*void Snowball::move(glm::vec3 handPos,float time, glm::vec3 direction) {
	if (!shooting) {
		position = handPos;
		toWorld = glm::mat4(1.0f);
		toWorld = toWorld * glm::translate(glm::mat4(1.0f), position - glm::vec3(0.0f, 0.1f, 0.0f));// * rotate;.
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
	}
	else {
		position = position + (time/10.0f * direction);
		toWorld = glm::mat4(1.0f);// *glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.1f, -0.5f));
		
		toWorld = toWorld * glm::translate(glm::mat4(1.0f), position)*rotate;
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
		if (position.z < -30.0f) {
			position = handPos;
			shooting = false;
		}
	}
}*/

bool Snowball::move(glm::vec3 handPos, float time, glm::vec3 direction) {

	glm::vec3 cameraPos = glm::vec3(Window::V[3]);
	if (!shooting && grabbed) {
		position = handPos; 
		//position = handPos - glm::vec3(0.0f, 101.0f, -50.0f);
		toWorld = glm::mat4(1.0f);
		toWorld = toWorld * glm::translate(glm::mat4(1.0f), position - glm::vec3(0.0f, 0.1f, 0.0f));// * rotate;.
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
		return false;
	}
	else if (shooting) {
		grabbed = false;
		//position = position + glm::vec3(0.0f, 0.1f, -0.5f);// +(time / 100.0f * direction);
		position = position + (time / 40.0f * direction);
		//std::cout << direction.x << " " << direction.y << " " << direction.z << std::endl;
		toWorld = glm::mat4(1.0f);// *glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.1f, -0.5f));

		toWorld = toWorld * glm::translate(glm::mat4(1.0f), position);// *rotate;
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
		if (position.x < -50.0f || position.x > 50.0f || position.y < -10.0f || position.y > 50.0f || position.z < -30.0f) {
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

float Snowball::RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}