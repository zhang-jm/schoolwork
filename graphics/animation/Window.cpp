#include "window.h"
#include "Skeleton.cpp"
#include "Joint.cpp"
#include "Skin.cpp"
#include "AnimationPlayer.cpp"
#include "Animation.cpp"
#include "Channel.cpp"
#include "Keyframe.cpp"

const char* window_title = "CSE 169 HW 2";
Skeleton * wasp;
Skin * waspSkin;
Animation * waspAnim;
AnimationPlayer * animPlayer;
GLint shaderProgram;
GLint skinShaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define SKIN_VERTEX_SHADER_PATH "../skinshader.vert"
#define SKIN_FRAGMENT_SHADER_PATH "../skinshader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 5.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

int left_held;
float lastX, lastY;
bool waspskel, waspskin;

void Window::initialize_objects()
{
	wasp = new Skeleton();
	waspSkin = new Skin(wasp);
	waspAnim = new Animation(wasp);
	animPlayer = new AnimationPlayer();

	wasp->parse("../wasp.skel");
	wasp->update();

	waspSkin->parse("../wasp.skin");
	waspSkin->update();

	waspAnim->parse("../wasp_walk.anim");

	animPlayer->setClip(waspAnim);

	for (int i = 0; i < 10; i++) {
		//animPlayer->Update();
		//wasp->update();
		//waspSkin->update();
	}

	waspskel = true;
	waspskin = true;

	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skinShaderProgram = LoadShaders(SKIN_VERTEX_SHADER_PATH, SKIN_FRAGMENT_SHADER_PATH);
}

void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
	glDeleteProgram(skinShaderProgram);

	delete(wasp);
	delete(waspSkin);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	animPlayer->Update();
	wasp->update();
	waspSkin->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);
	if (waspskel) {
		//wasp->draw(shaderProgram);
	}

	glUseProgram(skinShaderProgram);
	if (waspskin) {
		waspSkin->draw(skinShaderProgram);
	}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		else if (key == GLFW_KEY_N) {
			wasp->nextJoint();
		}
		else if (key == GLFW_KEY_M) {
			wasp->changeDirection();
		}
		else if (key == GLFW_KEY_I) {
			wasp->increaseDOF();
			waspSkin->update();
		}
		else if (key == GLFW_KEY_O) {
			wasp->decreaseDOF();
			waspSkin->update();
		}

		else if (key == GLFW_KEY_1) {
			waspskel = !waspskel;
		}
		else if (key == GLFW_KEY_2) {
			waspskin = !waspskin;
		}
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//std::cout << "L Mouse held" << std::endl;
		left_held = 1;
	}

	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		//std::cout << "L Mouse release" << std::endl;
		left_held = 0;
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (left_held == 1) {

		float angle;
		// Perform horizontal (y-axis) rotation
		angle = (float)(lastX - xpos) / 100.0f;
		cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cam_pos, 1.0f));
		cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cam_up, 1.0f));

		//Now rotate vertically based on current orientation
		angle = (float)(ypos - lastY) / 100.0f;
		glm::vec3 axis = glm::cross((cam_pos - cam_look_at), cam_up);
		cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(cam_pos, 1.0f));
		cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(cam_up, 1.0f));
		// Now update the camera
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		lastX = xpos;
		lastY = ypos;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset < 0)
	{
		cam_pos.z = cam_pos.z - 0.5f;
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
	else
	{
		cam_pos.z = cam_pos.z + 0.5f;
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

glm::vec3 Window::trackBallMapping(float x, float y)
{
	glm::vec3 v;
	float d;
	v.x = (2.0 * x - width) / width;
	v.y = (height - 2.0 * y) / height;
	v.z = 0.0;
	d = glm::length(v);
	d = (d<1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d*d);
	v = glm::normalize(v); // Still need to normalize, since we only capped d, not v.
	return v;
}