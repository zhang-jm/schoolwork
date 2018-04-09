#ifndef TEST_PLANT_H_
#define TEST_PLANT_H_

#include "ProceduralPlant.h"

#include <time.h>

class TestPlant : public ProceduralPlant {
public:
	TestPlant() : TestPlant(time(NULL)) {

	}

	TestPlant(unsigned int seed) {
		generateGeometry(1.0f, 25.0f, 4.0f, "F");
	}
	~TestPlant() {
		ProceduralPlant::~ProceduralPlant();
	}
protected:
};


#endif