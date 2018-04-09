#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include "Keyframe.h"

using namespace std;

class Channel
{
public:
	Channel();
	~Channel();

	std::string extrapIn, extrapOut;
	std::vector<Keyframe *> keyframes;

	void addKeyframe(float time, float value, std::string tangentIn, std::string tangentOut);
	float evaluate(float time);
	float computeValue(float time);
};

#endif
