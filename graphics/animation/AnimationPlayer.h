#ifndef _ANIMATIONPLAYER_H_
#define _ANIMATIONPLAYER_H_

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
#include "Animation.h"

class AnimationPlayer
{
public:
	AnimationPlayer();
	~AnimationPlayer();

	float time;
	Animation * anim;

	void setClip(Animation * anim);
	void Update();
};
#endif
