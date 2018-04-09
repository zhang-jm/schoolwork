#ifndef _MATRIXTRANSFORM_H_
#define _MATRIXTRANSFORM_H_

#include "Node.h"
#include "Group.h"

class MatrixTransform : public Group {
public: 
	MatrixTransform(glm::mat4 M, bool animate, bool forward, glm::vec3 scale);
	~MatrixTransform();

	glm::mat4 M;
	glm::vec3 spherecenter;
	glm::vec3 scale;
	bool drawn = 1;
	bool culling = 0;

	bool animated;
	int time = 0;
	bool forward;

	void draw(glm::mat4 C);
	void update(glm::mat4 C);
};
#endif;