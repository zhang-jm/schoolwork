#include "Flower.h"
#include "LSystem.h"

#include <time.h>

#define ITERATIONS 5
#define THETA 10.0f
#define DISTANCE 0.3f
#define WIDTH 2.3f
#define AXIOM std::string("F")
#define RULE_1_KEY 'F'
#define RULE_1_VAL std::string("[+FF]FF[&&&A][^^^A]")
#define RULE_2_KEY 'A'
#define RULE_2_VAL std::string("[+++++3FF][++3FF][5FFFFF][-5FFF][---3FF]")

#include <iostream>

Flower::Flower() : Flower(time(NULL)) {

}

Flower::Flower(unsigned int seed) {
	std::map<char, std::string> rules;
	rules[RULE_1_KEY] = RULE_1_VAL;
	rules[RULE_2_KEY] = RULE_2_VAL;
	LSystem lsys(rules, ITERATIONS, AXIOM);
	generateGeometry(DISTANCE, THETA, WIDTH, lsys.generateCommandString());
}

Flower::~Flower() {
	ProceduralPlant::~ProceduralPlant();
}