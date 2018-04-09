#include "audio/irrKlang.h"
#include "window.h"
#include "Particles.cpp"
#include "Terrain.cpp"
#include "Fern.cpp"
#include "Tree1.cpp"
#include "Tree2.cpp"
#include "Flower.cpp"
#include "ProceduralPlant.cpp"
#include "LSystem.cpp"

using namespace irrklang;

const char* window_title = "Where the Red Fern Grows";
Cube * skybox;
Terrain * terrain;
Particles * particles;

std::vector<ProceduralPlant *> plants;

GLint shaderProgram;
GLint skyboxShader;
GLint particleShader;
GLint treeShader;

ISoundEngine * SoundEngine;
ISound * bgm;
ISound * rock;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define SKYBOX_VERTEX_SHADER_PATH "../skyboxshader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skyboxshader.frag"
#define PARTICLE_VERTEX_SHADER_PATH "../particleshader.vert"
#define PARTICLE_FRAGMENT_SHADER_PATH "../particleshader.frag"
#define TREE_SHADER_VERTEX_PATH "../treeshader.vert"
#define TREE_SHADER_FRAGMENT_PATH "../treeshader.frag"

#define PLANT_CHANCE 70
#define PLANT_SCALE 0.8f

// Default camera parameters
glm::vec3 cam_pos(100.0f, 20.0f, 100.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, -1.0f);	// d  | This is the direction you want to look
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

float cam_speed = 0.2f;
float sensitivity = 0.2;
float fov = 45.0f;

bool keys[1024];

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

//keeps track of which mouse button held
bool left_held = 0;
bool right_held = 0;
bool first_press = true;
double lastX, lastY; //holds last cursor position
bool soundOn = true;
bool drawParticles = true;
float pitch = 0.0f;
float yaw = 90.0f;

void do_movement();

void plantTrees();
void deleteTrees();

void Window::initialize_objects()
{
	skybox = new Cube();
	terrain = new Terrain();
	particles = new Particles(particleShader);

	SoundEngine = createIrrKlangDevice();

	bgm = SoundEngine->play2D("../wind.mp3", GL_TRUE, GL_TRUE, GL_TRUE);
	bgm->setIsPaused(!soundOn);

	rock = SoundEngine->play2D("../rock2.mp3", GL_TRUE, GL_TRUE, GL_TRUE);
	rock->setVolume(0.3);

	//load shaders
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxShader = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	particleShader = LoadShaders(PARTICLE_VERTEX_SHADER_PATH, PARTICLE_FRAGMENT_SHADER_PATH);
	treeShader = LoadShaders(TREE_SHADER_VERTEX_PATH, TREE_SHADER_FRAGMENT_PATH);

	terrain->generateTerrain(10);
	particles->setup(10);
	plantTrees();
	
	cam_pos.y = terrain->height[100][100] + 20;
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(skybox);
	delete(terrain);
	delete(particles);

	bgm->stop();

	deleteTrees();
	glDeleteProgram(shaderProgram);
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
}

void Window::idle_callback()
{
	particles->update();
}

void Window::display_callback(GLFWwindow* window)
{
	do_movement();
	P = glm::perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);
	V = glm::lookAt(cam_pos, cam_pos + cam_look_at, cam_up);

	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw skybox
	glUseProgram(skyboxShader);
	skybox->draw(skyboxShader);

	//draw terrain
	glUseProgram(shaderProgram);
	terrain->draw(shaderProgram);

	//draw trees
	glUseProgram(treeShader);
	for (auto it = plants.begin(); it != plants.end(); ++it)
		(*it)->draw(treeShader);

	if (drawParticles) {
		//draw particles
		glUseProgram(particleShader);
		particles->draw(particleShader);
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
		keys[key] = true;

		switch (key) {
		case(GLFW_KEY_ESCAPE):
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case(GLFW_KEY_T):
			terrain->generateTerrain(10);
			cam_pos.y = terrain->height[(int)cam_pos.x][(int)cam_pos.z] + 20;
			deleteTrees();
			plantTrees();
			break;
		case(GLFW_KEY_P):
			for (auto it = plants.begin(); it != plants.end(); ++it)
				(*it)->regenerate();
			break;
		case(GLFW_KEY_R):
			cam_pos = glm::vec3(10.0f, 1.0f, 20.0f);
			cam_look_at = glm::vec3(0.0f, 0.0f, -1.0f);
			cam_up = glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		case(GLFW_KEY_Y):
			bgm->setIsPaused(soundOn);
			soundOn = !soundOn;
			break;
		case(GLFW_KEY_O):
			drawParticles = !drawParticles;
			break;
		case(GLFW_KEY_W):
			if (soundOn) {
				rock->setIsPaused(false);
			}
			break;
		case(GLFW_KEY_A):
			if (soundOn) {
				rock->setIsPaused(false);
			}
			break;
		case(GLFW_KEY_S):
			if (soundOn) {
				rock->setIsPaused(false);
			}
			break;
		case(GLFW_KEY_D):
			if (soundOn) {
				rock->setIsPaused(false);
			}
			break;
		}
	}

	else if (action == GLFW_RELEASE) {
		keys[key] = false;
		if (key = GLFW_KEY_W) {
			if (soundOn) {
				rock->setIsPaused(true);
			}
		}
		else if (key = GLFW_KEY_A) {
			if (soundOn) {
				rock->setIsPaused(true);
			}
		}
		else if (key = GLFW_KEY_S) {
			if (soundOn) {
				rock->setIsPaused(true);
			}
		}
		else if (key = GLFW_KEY_D) {
			if (soundOn) {
				rock->setIsPaused(true);
			}
		}
	}
}


void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	//left mouse pressed
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		left_held = 1;
	}

	//left mouse released
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		left_held = 0;
	}

	//right mouse pressed
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		right_held = 1;
	}

	//right mouse released
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		right_held = 0;
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (left_held == 1) {
		float xoffset = (xpos - lastX) * sensitivity;
		float yoffset = (lastY - ypos) * sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		else if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
		front.y = glm::sin(glm::radians(pitch));
		front.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
		cam_look_at = glm::normalize(front);
	}

	lastX = xpos;
	lastY = ypos;
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Zoom
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset * sensitivity;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void do_movement() {
	if (keys[GLFW_KEY_W]) {
		cam_pos += cam_look_at * cam_speed;
		cam_pos.y = terrain->height[(int)cam_pos.x][(int)cam_pos.z] + 20;

		if (cam_pos.x > 1000) {
			cam_pos.x = 1000;
		}

		else if (cam_pos.x < 100) {
			cam_pos.x = 100;
		}

		if (cam_pos.z > 1000) {
			cam_pos.z = 1000;
		}

		else if (cam_pos.z < 100) {
			cam_pos.z = 100;
		}
	}
	if (keys[GLFW_KEY_S])
		cam_pos -= cam_look_at * cam_speed;
	if (keys[GLFW_KEY_D])
		cam_pos += glm::normalize(glm::cross(cam_look_at, cam_up)) * cam_speed;
	if (keys[GLFW_KEY_A])
		cam_pos -= glm::normalize(glm::cross(cam_look_at, cam_up)) * cam_speed;
}

void plantTrees() {
	srand(time(NULL));
	for (int i = 0; i < terrain->size; i += 200 + (rand() % 20)) {
		for (int j = 0; j < terrain->size; j += 200 + (rand() % 20)) {
			if (i > terrain->size || j > terrain->size) {
				break;
			}

			if (terrain->height[i][j] < -5.0)
				continue;
			if (rand() % 100 < PLANT_CHANCE) {
				// Plant a tree
				ProceduralPlant * plant;
				switch (rand() % 4) {
				case(0):
					plant = new Tree1();
					break;
				case(1):
					plant = new Tree2();
					break;
				case(2):
					plant = new Fern();
					break;
				case(3):
					plant = new Flower();
					break;
				default:
					plant = new Flower();
					break;
				}
				glm::vec3 position = glm::vec3((float)i, terrain->height[i][j], (float)j);
				plant->setToWorld(glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(PLANT_SCALE)));
				plants.emplace_back(plant);
			}
		}
	}
}

void deleteTrees() {
	for (auto it = plants.begin(); it != plants.end(); ++it) delete(*it);
	plants.clear();
}