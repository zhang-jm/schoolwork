#ifndef LSYSTEM_H_
#define LSYSTEM_H_

#include <map>
#include <string>

/*
	LSystems consist of a map of rules, a number of iterations, and a starting axiom.
	The rules are a mapping of a single character to a string of characters. No character can be mapped to more than one string.
	This class does not interpret the output, only generates the command string and leaves interpretation up to the accessor.
*/

class LSystem {
public:
	LSystem();
	LSystem(const std::map<char, std::string> & rules, int iterations, const std::string & axiom);
	~LSystem();
	void addRule(char, std::string);
	void removeRule(char);
	std::string generateCommandString();
protected:
private:
	std::map<char, std::string> rules;
	int iterations;
	std::string axiom;
};


#endif