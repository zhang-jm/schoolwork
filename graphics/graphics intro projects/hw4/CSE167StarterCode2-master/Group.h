#ifndef _GROUP_H_
#define _GROUP_H_

#include "Node.h"
#include <vector>

class Group : public Node {
public:
	Group();
	~Group();

	std::vector<Node*> * children;

	void draw(glm::mat4 C);
	void update(glm::mat4 C);
	void addChild(Node * child);
	void removeChild();
};
#endif