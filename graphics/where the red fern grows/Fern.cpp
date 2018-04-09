#include "Fern.h"
#include "LSystem.h"

#include <time.h>

/*
#define ITERATIONS 6
#define THETA 25.0f
#define DISTANCE 2.0f
#define AXIOM std::string("X")
#define RULE_1_KEY 'X'
#define RULE_1_VAL std::string("0F-[2[X]+3X]+1F[3+FX]-X")
#define RULE_2_KEY 'F'
#define RULE_2_VAL std::string("FF")


#define ITERATIONS 5
#define THETA 22.0f
#define DISTANCE 1.0f
#define AXIOM std::string("F")
#define RULE_1_KEY 'F'
#define RULE_1_VAL std::string("0FF-[1-F+F+F]+[2+F-F-F]")

*/


#define ITERATIONS 7
#define THETA 25.0f
#define DISTANCE 0.1f
#define WIDTH 2.0f
#define AXIOM std::string("X")
#define RULE_1_KEY 'X'
#define RULE_1_VAL std::string("0F-[2[X]+3X]+1F[3+FX]&1F[3&F]^X")
#define RULE_2_KEY 'F'
#define RULE_2_VAL std::string("FF")

Fern::Fern() : Fern(time(NULL)) {

}

Fern::Fern(unsigned int seed) {
	std::map<char, std::string> rules;
	rules[RULE_1_KEY] = RULE_1_VAL;
	rules[RULE_2_KEY] = RULE_2_VAL;
	LSystem lsys(rules, ITERATIONS, AXIOM);
	generateGeometry(DISTANCE, THETA, WIDTH, lsys.generateCommandString());
}

Fern::~Fern() {
	ProceduralPlant::~ProceduralPlant();
}