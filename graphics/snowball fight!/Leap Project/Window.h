#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Skybox.h"
#include "shader.h"

class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ApplyLeftFrustum();
	static void ApplyRightFrustum();
	static glm::vec3 getPlayerPos();
	static void setEnemyPlayerPos(float x, float y, float z);
	static bool getPlayerSnowballInHand();
	static void setEnemySnowballPosition(glm::vec3 pos);
	static glm::vec3 getPlayerSnowballPos();
	static void setEnemySnowballinHand(bool inHand);
	static bool getHitByEnemy();
	static void setEnemyHit(bool enemyHit);
};

#endif
