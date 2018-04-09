#ifndef _KEYFRAME_H_
#define _KEYFRAME_H_

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

class Keyframe
{
public:
	Keyframe(float time, float value, std::string tangentIn, std::string tangentOut);
	~Keyframe();

	float time;
	float value;
	std::string tangentIn, tangentOut;
};

#endif