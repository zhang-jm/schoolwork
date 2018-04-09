#include "Skeleton.h"

Skeleton::Skeleton() {
	jointnum = 0;
}

Skeleton::~Skeleton() {

}

void Skeleton::draw(GLuint shaderProgram) {
	//update();
	root->draw(shaderProgram);
}

void Skeleton::update() {
	root->update(glm::mat4(1.0f));
}

void Skeleton::parse(char * filename) {
	FILE* fp;
	char string[100];
	fpos_t pos;

	Joint * current = 0;

	fopen_s(&fp, filename, "rb");
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }

	do {
		fscanf_s(fp, "%s", string);
		std::cout << string << std::endl;

		if (feof(fp)) {
			break;
		}

		if ((string[0] == 'b') && (string[1] == 'a')) //balljoint
		{
			std::cout << "New Balljoint" << std::endl;
			if (root == NULL) {
				current = new Joint();
				root = current;
				joints.push_back(current);

				fscanf_s(fp, "%s", string);
				std::cout << string << std::endl;
				current->name = string;
				fscanf_s(fp, "%s", string);
				std::cout << string << std::endl;
			}
			else {
				Joint * newJoint = new Joint();
				joints.push_back(newJoint);
				newJoint->parent = current;
				current->addChild(newJoint);
				current = newJoint;

				fscanf_s(fp, "%s", string);
				std::cout << string << " ";
				current->name = string;
				fscanf_s(fp, "%s", string);
				std::cout << string << std::endl;
			}
		}
		else if ((string[0] == 'o')) { //offset
			float x, y, z;

			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			std::cout << "Offset: " << x << " " << y << " " << z << std::endl;

			current->setOffset(glm::vec3(x, y, z));
		}
		else if ((string[0] == 'b') && (string[4] == 'i')) { //boxmin
			float x, y, z;

			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			std::cout << "Boxmin: " << x << " " << y << " " << z << std::endl;

			current->setBoxmin(glm::vec3(x, y, z));
		}
		else if ((string[0] == 'b') && (string[4] == 'a')) { //boxmax
			float x, y, z;

			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			std::cout << "Boxmax: " << x << " " << y << " " << z << std::endl;

			current->setBoxmax(glm::vec3(x, y, z));
		}
		else if ((string[0] == 'r') && (string[3] == 'x')) { //rotx
			float x, y;

			fscanf_s(fp, "%f %f", &x, &y);
			std::cout << "Rotx: " << x << " " << y << std::endl;

			current->setRotx(glm::vec2(x, y));
		}
		else if ((string[0] == 'r') && (string[3] == 'y')) { //roty
			float x, y;

			fscanf_s(fp, "%f %f", &x, &y);
			std::cout << "Roty: " << x << " " << y << std::endl;

			current->setRoty(glm::vec2(x, y));
		}
		else if ((string[0] == 'r') && (string[3] == 'z')) { //rotz
			float x, y;

			fscanf_s(fp, "%f %f", &x, &y);
			std::cout << "Rotz: " << x << " " << y << std::endl;

			current->setRotz(glm::vec2(x, y));
		}
		else if ((string[0] == 'p')) { //roty
			float x, y, z;

			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			std::cout << "Pose: " << x << " " << y << " " << z << std::endl;

			current->setPose(glm::vec3(x, y, z));
		}
		else if ((string[0] == '}')) {
			if (current->parent != NULL) {
				current = (Joint *)current->parent;
				std::cout << "Getting parent" << std::endl;
			}
		}
	} while (!feof(fp));

	fclose(fp);

	//root->validateJoint();
}

glm::mat4 Skeleton::getWorldMatrix(int jointNum) {
	return joints.at(jointNum)->toWorld2;
}

void Skeleton::nextJoint() {
	jointnum = (jointnum + 1) % joints.size();
	std::cout << "DOF: " << joints.at(jointnum)->name << std::endl;
}

void Skeleton::changeDirection() {
	direction = (direction + 1) % 3;
	if (direction == 0) {
		std::cout << "Rotating x axis" << std::endl;
	}
	if (direction == 1) {
		std::cout << "Rotating y axis" << std::endl;
	}
	if (direction == 2) {
		std::cout << "Rotating z axis" << std::endl;
	}
}

void Skeleton::increaseDOF() {
	Joint * current = joints.at(jointnum);
	if (direction == 0) {
		current->setPose(current->pose + glm::vec3(0.1f, 0.0f, 0.0f));
	}
	else if (direction == 1) {
		current->setPose(current->pose + glm::vec3(0.0f, 0.1f, 0.0f));
	}
	else if (direction == 2) {
		current->setPose(current->pose + glm::vec3(0.0f, 0.0f, 0.1f));
	}
	std::cout << current->pose.x << " " << current->pose.y << " " << current->pose.z << std::endl;
	update();
}

void Skeleton::decreaseDOF() {
	Joint * current = joints.at(jointnum);
	if (direction == 0) {
		current->setPose(current->pose - glm::vec3(0.1f, 0.0f, 0.0f));
	}
	else if (direction == 1) {
		current->setPose(current->pose - glm::vec3(0.0f, 0.1f, 0.0f));
	}
	else if (direction == 2) {
		current->setPose(current->pose - glm::vec3(0.0f, 0.0f, 0.1f));
	}
	std::cout << current->pose.x << " " << current->pose.y << " " << current->pose.z << std::endl;
	update();
}