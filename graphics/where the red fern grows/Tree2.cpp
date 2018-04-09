#include "Tree2.h"
#include "LSystem.h"

#include <time.h>

#define ITERATIONS 5
#define THETA 35.0f
#define DISTANCE 0.3f
#define WIDTH 5.0f
#define AXIOM std::string("FX")
#define RULE_1_KEY 'F'
#define RULE_1_VAL std::string("0FF-[3-F+F^F&F]+[1+F-F^F&F]")
#define RULE_2_KEY 'X'
#define RULE_2_VAL std::string("0FF+[1+F]+[3-F]");

#include <iostream>

Tree2::Tree2() : Tree2(time(NULL)) {

}

Tree2::Tree2(unsigned int seed) {
	std::map<char, std::string> rules;
	rules[RULE_1_KEY] = RULE_1_VAL;
	rules[RULE_2_KEY] = RULE_2_VAL;
	LSystem lsys(rules, ITERATIONS, AXIOM);
	generateGeometry(DISTANCE, THETA, WIDTH, lsys.generateCommandString());
}

Tree2::~Tree2() {
	ProceduralPlant::~ProceduralPlant();
}