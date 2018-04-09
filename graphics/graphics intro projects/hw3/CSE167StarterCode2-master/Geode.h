#ifndef _GEODE_H_
#define _GEODE_H_

#include "Node.h"

class Geode : public Node {
public:
	virtual void draw(glm::mat4 C)=0;
	virtual void update(glm::mat4 C)=0;
	virtual void render() = 0;
};
#endif
