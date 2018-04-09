#ifndef FLOWER_H_
#define FLOWER_H_

#include "ProceduralPlant.h"

class Flower : public ProceduralPlant {
public:
	Flower();
	Flower(unsigned int seed);
	~Flower();
protected:
};


#endif