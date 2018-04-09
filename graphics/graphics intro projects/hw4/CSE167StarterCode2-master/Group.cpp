#include "Group.h"

Group::Group() {
	children = new std::vector<Node*>();
}

Group::~Group() {
	if (children) {
		for (int i = 0; i < children->size(); i++) {
			delete(children->at(i));
		}

		delete(children);
	}
}

void Group::draw(glm::mat4 C) {
	for(int i = 0; i < children->size(); i++){
		children->at(i)->draw(C);
	}

	update(C);
}

void Group::addChild(Node * child) {
	children->push_back(child); //add child at end
}

void Group::removeChild() {
	children->pop_back(); //delete last child
}

void Group::update(glm::mat4 C) {
	for (int i = 0; i < children->size(); i++) {
		children->at(i)->update(C);
	}
}