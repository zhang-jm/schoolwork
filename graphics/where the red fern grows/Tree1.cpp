#include "Tree1.h"
#include "LSystem.h"

#include <time.h>

#define ITERATIONS 6
#define THETA 22.0f
#define DISTANCE 0.5f
#define WIDTH 3.0f
#define AXIOM std::string("F")
#define RULE_1_KEY 'F'
#define RULE_1_VAL std::string("0FF-[4-F+F]+[2+F-F]^[4^FF&F]&[2&FF^F]")

#include <iostream>

Tree1::Tree1() : Tree1(time(NULL)) {

}

Tree1::Tree1(unsigned int seed) {
	std::map<char, std::string> rules;
	rules[RULE_1_KEY] = RULE_1_VAL;
	LSystem lsys(rules, ITERATIONS, AXIOM);
	generateGeometry(DISTANCE, THETA, WIDTH, lsys.generateCommandString());
}

Tree1::~Tree1() {
	ProceduralPlant::~ProceduralPlant();
}