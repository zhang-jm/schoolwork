#include "Joint.h"

Joint::Joint() {
	toWorld = glm::mat4(1.0f);

	offset = glm::vec3(0, 0, 0);
	boxmin = glm::vec3(-1, -1, -1);
	boxmax = glm::vec3(1, 1, 1);
	rotx = glm::vec2(-100000, 100000);
	roty = glm::vec2(-100000, 100000);;
	rotz = glm::vec2(-100000, 100000);;
	pose = glm::vec3(0, 0, 0);

	c = new Cube(false);
}

Joint::~Joint() {
	delete(c);

	for (int i = 0; i < children.size(); i++) {
		delete(children.at(i));
	}
}

void Joint::draw(GLuint shaderProgram) {
	c->toWorld = this->toWorld;
	c->draw(shaderProgram);

	for (int i = 0; i < children.size(); i++) {
		children.at(i)->draw(shaderProgram);
	}
}

void Joint::update(glm::mat4 initialtoWorld) {
	//std::cout << "update" << std::endl;

	glm::vec3 clampedPose = getClampedPose();
	//std::cout << pose.x << " " << pose.y << " " << pose.z << std::endl;

	glm::mat4 local = glm::mat4(1.0f);

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), boxmax - boxmin);
	toWorld = glm::mat4(1.0f);
	toWorld = scale * toWorld;

	glm::mat4 translation_local = glm::translate(glm::mat4(1.0f), glm::vec3((boxmax.x + boxmin.x) / 2,
		(boxmax.y + boxmin.y) / 2, (boxmax.z + boxmin.z) / 2) + offset);
	local = translation_local * local;

	glm::mat4 translation_toParent = glm::translate(glm::mat4(1.0f), offset);
	//local = translation_toParent * local;
	
	glm::mat4 rotation = glm::mat4(1.0f);

	glm::mat4 rotatex = glm::rotate(glm::mat4(1.0f), clampedPose.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//local = rotatex * local;
	//translation_toParent = rotatex * translation_toParent;

	glm::mat4 rotatey = glm::rotate(glm::mat4(1.0f), clampedPose.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//local = rotatey * local;
	//translation_toParent = rotatey * translation_toParent;

	glm::mat4 rotatez = glm::rotate(glm::mat4(1.0f), clampedPose.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//local = rotatez * local;
	//translation_toParent = rotatez * translation_toParent;

	rotation = rotatez * rotatey * rotatex;
	local = translation_toParent * rotation * glm::inverse(translation_toParent) * local;
	//local = local * rotation;
	translation_toParent = translation_toParent * rotation;

	toWorld = initialtoWorld * local * toWorld;

	toWorld2 = initialtoWorld * translation_toParent;

	for (int i = 0; i < children.size(); i++) {
		//std::cout << "child " << i << std::endl;
		children.at(i)->update(initialtoWorld * translation_toParent);
	}
}

glm::vec3 Joint::getClampedPose() {
	float x = pose.x;
	float y = pose.y;
	float z = pose.z;
	if (pose.x >= rotx.y) { x = rotx.y; }
	if (pose.x <= rotx.x) { x = rotx.x; }

	if (pose.y >= roty.y) { y = roty.y; }
	if (pose.y <= roty.x) { y = roty.x; }

	if (pose.z >= rotz.y) { z = rotz.y; }
	if (pose.z <= rotz.x) { z = rotz.x; }

	return glm::vec3(x, y, z);
}

void Joint::addChild(Node * child) {
	children.push_back(child);
}

void Joint::setOffset(glm::vec3 offset) {
	this->offset = offset;
}

void Joint::setBoxmin(glm::vec3 boxmin) {
	this->boxmin = boxmin;
}

void Joint::setBoxmax(glm::vec3 boxmax) {
	this->boxmax = boxmax;
}

void Joint::setRotx(glm::vec2 rotx) {
	this->rotx = rotx;
}

void Joint::setRoty(glm::vec2 roty) {
	this->roty = roty;
}

void Joint::setRotz(glm::vec2 rotz) {
	this->rotz = rotz;
}

void Joint::setPose(glm::vec3 pose) {
	this->pose = pose;
}

void Joint::validateJoint() {
	std::cout << "-- New Joint -- " << std::endl;
	std::cout << "Offset: " << offset.x << " " << offset.y << " " << offset.z << std::endl;
	std::cout << "Boxmin: " << boxmin.x << " " << boxmin.y << " " << boxmin.z << std::endl;
	std::cout << "Boxmax: " << boxmax.x << " " << boxmax.y << " " << boxmax.z << std::endl;
	std::cout << "Rotx: " << rotx.x << " " << rotx.y << std::endl;
	std::cout << "Roty: " << roty.x << " " << roty.y << std::endl;
	std::cout << "Rotz: " << rotz.x << " " << rotz.y << std::endl;
	std::cout << "Pose: " << pose.x << " " << pose.y << " " << pose.z << std::endl;

	for (int i = 0; i < children.size(); i++) {
		children.at(i)->validateJoint();
	}
}