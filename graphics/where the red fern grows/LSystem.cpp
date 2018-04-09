#include "LSystem.h"

LSystem::LSystem() {
	this->rules['X'] = std::string("1F-[2[X]+3X]+1F[3+FX]-X");
	this->rules['F'] = std::string("FF");
	this->iterations = 6;
	this->axiom = std::string("X");
}

LSystem::LSystem(const std::map<char, std::string> & rules, int iterations, const std::string & axiom) : 
	rules(std::map<char, std::string>(rules)), iterations(iterations), axiom(std::string(axiom)) {

}

LSystem::~LSystem() {

}

void LSystem::addRule(char a, std::string b) {
	this->rules[a] = std::string(b);
}

void LSystem::removeRule(char a) {
	this->rules.erase(a);
}

std::string LSystem::generateCommandString() {
	std::string initial(this->axiom);
	std::string next("");
	for (int i = 0; i < iterations; ++i) {
		for (std::string::iterator it = initial.begin(); it != initial.end(); ++it) {
			if (this->rules.find(*it) != this->rules.end()) {
				next.append(this->rules.at(*it));
			}
			else {
				next.append(1, *it);
			}
		}
		initial = std::string(next);
		next = std::string("");
	}
	return initial;
}