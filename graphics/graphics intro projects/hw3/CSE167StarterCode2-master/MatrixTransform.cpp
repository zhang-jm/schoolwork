#include "MatrixTransform.h"

MatrixTransform::MatrixTransform(glm::mat4 M, bool animate, bool forward, glm::vec3 scale) {
	this->M = M;
	this->animated = animate;
	this->forward = forward;
	this->scale = scale;
}

MatrixTransform::~MatrixTransform() {
	cout << "mt delete" << endl;
	if (children) {
		for (int i = 0; i < children->size(); i++) {
			delete(children->at(i));
		}

		delete(children);
	}
}

void MatrixTransform::draw(glm::mat4 C) {
	glm::mat4 mult = C * M;

	if (drawn){
		Group::draw(mult);
	}
}

void MatrixTransform::update(glm::mat4 C) {
	if (animated)
	{
		if(forward)
		{
			glm::vec3 t = glm::vec3(scale.x * 2, scale.y * 2, 0.0f) - glm::vec3(0.0f);
			glm::mat4 trans = glm::translate(glm::mat4(1.0f), t);
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), 0.000005f, glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 tinv = glm::inverse(trans);
			this->M = M * trans * rot * tinv;
			time++;

			if (time > 50000)
			{
				forward = 0;
			}
		}
		else {
			glm::vec3 t = glm::vec3(scale.x * 2, scale.y * 2, 0.0f) - glm::vec3(0.0f);
			glm::mat4 trans = glm::translate(glm::mat4(1.0f), t);
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -0.000005f, glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 tinv = glm::inverse(trans);
			this->M = M * trans * rot * tinv;

			time--;
			if (time < -50000)
			{
				forward = 1;
			}
		}

	}
}