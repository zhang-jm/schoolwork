#ifndef _ANIMATION_H_
#define _ANIMATION_H_

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
#include "Channel.h"
#include "Skeleton.h"

class Animation
{
public:
	Animation(Skeleton * skel);
	~Animation();

	float startTime, endTime;
	float numChannels;

	Skeleton * skel;
	std::vector<Channel *> channels;

	void parse(char * filename);
	void evaluate(float time);
};
#endif
