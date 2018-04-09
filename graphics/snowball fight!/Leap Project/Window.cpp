#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define OVR_OS_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#define OVR_OS_MAC
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#define OVR_OS_LINUX
#endif

#include "window.h"
#include "./LibOVR/Include/OVR_CAPI.h" //include oculus sdk
#include "./OVRAvatarSDK/Include/OVR_Avatar.h" //include oculus avatar sdk
#include "SnowballSystem.cpp"
#include "Snowball.cpp"
#include "Screen.cpp"
#include "SOIL.h"
#include "LeapSDK/include/Leap.h"
#include "Line.h"
#include "Player.cpp"
//#include "ClientGame.h"

//ClientGame * client;

glm::vec3 handPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 handAngle = glm::vec3(0.0f);
Line * handPointLine;

class LeapEventListener : public Leap::Listener {
public:
	virtual void onConnect(const Leap::Controller&);
	virtual void onDisconnect(const Leap::Controller&);
	virtual void onFrame(const Leap::Controller&);
};

void LeapEventListener::onConnect(const Leap::Controller& controller) {
	//std::cout << "Connected" << std::endl;
	// Enable gestures, set Config values:
	controller.enableGesture(Leap::Gesture::TYPE_SWIPE);
	controller.config().setFloat("Gesture.Swipe.MinLength", 200.0);
	controller.config().save();
}

//Not dispatched when running in a debugger
void LeapEventListener::onDisconnect(const Leap::Controller& controller) {
	std::cout << "Disconnected" << std::endl;
}

void LeapEventListener::onFrame(const Leap::Controller& controller) {
	//std::cout << "New frame available" << std::endl;
	Leap::Frame frame = controller.frame();
	// Process this frame's data...
	//std::cout << "Finger position: " << frame.fingers()[0].tipPosition() << std::endl;

	Leap::Vector leapFingerPos = frame.fingers()[0].tipPosition();
	//fingerPos = glm::vec3(leapFingerPos.x, leapFingerPos.y, leapFingerPos.z);

	Leap::Vector leapHandPos = frame.hands()[0].palmPosition();
	Leap::Vector leapHandAngle = frame.hands()[0].direction();
	handPos = glm::vec3(leapHandPos.x, leapHandPos.y, leapHandPos.z);
	handAngle = glm::vec3(leapHandAngle.x, leapHandAngle.y, leapHandAngle.z);
	//std::cout << "Hand angle: " << frame.hands()[0].direction() << std::endl;

}

const char* window_title = "Snowball Fight!";
GLint shaderProgram, skyboxshader, floorshader, lineShader;
GLuint quad_programID;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define SKYBOX_VERTEX_SHADER_PATH "../skyboxshader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skyboxshader.frag"
#define FLOOR_VERTEX_SHADER_PATH "../floorshader.vert"
#define FLOOR_FRAGMENT_SHADER_PATH "../floorshader.frag"
#define LINE_VERTEX_SHADER_PATH "../lineshader.vert"
#define LINE_FRAGMENT_SHADER_PATH "../lineshader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

glm::vec3 cam_pos2;
glm::vec3 cam_look_at2;

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

bool spacePressed = false;

//SnowballSystem * snowballs;
Player * thisPlayer;
Player * enemyPlayer;
Player * ditto;
Skybox * skybox;
Screen * floor2;

GLuint leftEye = 0;
GLuint rightEye = 0;
GLuint leftTexture;
GLuint rightTexture;

float spawnRate = 0.025f;
float trackSpawn = 5.0f;
bool gameOver = false;

GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

float mConvergence = 10;
float mEyeSeparation = 0.05f;
float mAspectRatio = 1;
float mFOV = 45;
float mNearClippingDistance = 1;
float mFarClippingDistance = 300;

bool enemyHit = false;
bool hit = false;
float timer = 0.0f;

static const GLfloat g_quad_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,
};


GLuint quad_vertexbuffer;
GLuint depthrenderbuffer;
GLuint depthrenderbuffer2;

void Window::initialize_objects()
{
	// initialize the client 
	//client = new ClientGame();

	//snowballs = new SnowballSystem();
	thisPlayer = new Player(glm::vec3(0.0f, 0.0f, 40.0f), false);
	thisPlayer->isOpponent = false;
	enemyPlayer = new Player(glm::vec3(0.0f, 0.0f, -40.0f), true);
	enemyPlayer->isOpponent = true;
	enemyPlayer->setModel();
	ditto = new Player(glm::vec3(-75.0f, -20.0f, -150.0f), true);
	ditto->isOpponent = true;
	ditto->model = new Model("../assets/Ditto/ditto.obj");
	skybox = new Skybox();
	floor2 = new Screen();
	handPointLine = new Line();

	glGenFramebuffers(1, &leftEye);
	glGenFramebuffers(1, &rightEye);
	glGenBuffers(1, &quad_vertexbuffer);
	glGenTextures(1, &leftTexture);
	glGenTextures(1, &rightTexture);
	glGenRenderbuffers(1, &depthrenderbuffer);
	glGenRenderbuffers(1, &depthrenderbuffer2);

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxshader = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	floorshader = LoadShaders(FLOOR_VERTEX_SHADER_PATH, FLOOR_FRAGMENT_SHADER_PATH);
	lineShader = LoadShaders(LINE_VERTEX_SHADER_PATH, LINE_FRAGMENT_SHADER_PATH);

	quad_programID = LoadShaders("../anaglyph.vert", "../anaglyph.frag");
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(skybox);

	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyboxshader);
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

	if (width > 0 && height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	}
}

void Window::idle_callback()
{

}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Initialize Leap Motion controller
	Leap::Controller controller;
	LeapEventListener listener;
	controller.addListener(listener);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		thisPlayer->move(thisPlayer->headPosition + glm::vec3(0.0f, 0.0f, -1.0f) / 1.5f);
		//std::cout << "FORWARD" << std::endl;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		thisPlayer->move(thisPlayer->headPosition - glm::vec3(0.0f, 0.0f, -1.0f) / 1.5f);
		//std::cout << "BACK" << std::endl;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		thisPlayer->move(thisPlayer->headPosition + glm::vec3(1.0f, 0.0f, 0.0f) / 1.5f);
		//std::cout << "RIGHT" << std::endl;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		thisPlayer->move(thisPlayer->headPosition - glm::vec3(1.0f, 0.0f, 0.0f) / 1.5f);
		//std::cout << "LEFT" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		//std::cout << "SPACE" << std::endl;
		if (!spacePressed)spacePressed = true;
		else spacePressed = false;
	}
	else {
		spacePressed = false;
	}

	glEnable(GL_DEPTH_TEST);

	//Intialize this player's values
	//thisPlayer->headPosition = cam_pos;
	thisPlayer->handPosition = handPos;
	thisPlayer->handAngle = handAngle;

	//std::cout << cam_pos.x << " " << cam_pos.y << " " << cam_pos.z << std::endl;
	cam_pos = thisPlayer->headPosition - (glm::vec3(0.05f, 0.0f, 0.0f));
	cam_look_at = cam_pos + glm::vec3(0.05f, 0.0f, -20.0f);
	ApplyLeftFrustum();
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	glBindFramebuffer(GL_FRAMEBUFFER, leftEye);

	glBindTexture(GL_TEXTURE_2D, leftTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::width, Window::height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Window::width, Window::height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, leftTexture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, leftTexture);
	glViewport(0, 0, Window::width, Window::height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the list of draw buffers.
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	glUseProgram(skyboxshader);
	skybox->draw(skyboxshader);

	glUseProgram(floorshader);
	floor2->draw(floorshader);

	glUseProgram(shaderProgram);
	ditto->draw(shaderProgram, 0.05f, false, false);
	enemyPlayer->draw(shaderProgram, 0.05f, false, true);
	thisPlayer->draw(shaderProgram, 0.05f, spacePressed, true);
	

	glBindFramebuffer(GL_FRAMEBUFFER, rightEye);

	glBindTexture(GL_TEXTURE_2D, rightTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::width, Window::height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer2);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Window::width, Window::height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer2);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rightTexture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, rightTexture);
	glViewport(0, 0, Window::width, Window::height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam_pos2 = thisPlayer->headPosition + (glm::vec3(0.05f, 0.0f, 0.0f));
	cam_look_at2 = cam_pos2 + glm::vec3(-0.05f, 0.0f, -20.0f);
	ApplyRightFrustum();
	V = glm::lookAt(cam_pos2, cam_look_at2, cam_up);

	glUseProgram(skyboxshader);
	skybox->draw(skyboxshader);

	glUseProgram(floorshader);
	floor2->draw(floorshader);

	glUseProgram(shaderProgram);
	ditto->draw(shaderProgram, 0.05f, false, false);
	enemyPlayer->draw(shaderProgram, 0.05f, false, true);
	thisPlayer->draw(shaderProgram, 0.05f, spacePressed, true);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::width, Window::height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(quad_programID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, leftTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rightTexture);

	if (hit == true && timer < 2.0f) {
		timer += 0.05f;
		enemyHit = false;
		glUniform1i(glGetUniformLocation(quad_programID, "hit"), 1);
	}
	else if (enemyHit == true) {
		timer = 0.0f;
		hit = false;
		glUniform1i(glGetUniformLocation(quad_programID, "hit"), 2);
	}
	else {
		glUniform1i(glGetUniformLocation(quad_programID, "hit"), 0);
		timer = 0.0f;
		hit = false;
		enemyHit = false;
	}

	GLuint leftTexID = glGetUniformLocation(quad_programID, "lefteye");
	glUniform1i(leftTexID, 0);
	GLuint rightTexID = glGetUniformLocation(quad_programID, "righteye");
	glUniform1i(rightTexID, 1);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

	glDisableVertexAttribArray(0);
	glCullFace(GL_FRONT);

	glUseProgram(lineShader);
	handPointLine->setEndPoints(handPos, handAngle);
	handPointLine->draw(0.0f, lineShader);// , Window::P, Window::V);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);


	/*client->playerX = thisPlayer->headPosition.x;
	client->playerY = thisPlayer->headPosition.y;
	client->playerZ = thisPlayer->headPosition.z;
	client->update();*/
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
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			//std::cout << "GRAB" << std::endl;
			thisPlayer->grabSnowball();
			//snowballs->grabNextSnowball();
		}
	}
}

void Window::ApplyLeftFrustum()
{
	float top, bottom, left, right;

	top = mNearClippingDistance * tan(mFOV / 2);
	bottom = -top;

	float a = mAspectRatio * tan(mFOV / 2) * mConvergence;

	float b = a - mEyeSeparation / 2;
	float c = a + mEyeSeparation / 2;

	left = -b * mNearClippingDistance / mConvergence;
	right = c * mNearClippingDistance / mConvergence;

	// Set the Projection Matrix
	Window::P = glm::frustum(left, right, bottom, top, mNearClippingDistance, mFarClippingDistance);
}

void Window::ApplyRightFrustum()
{
	float top, bottom, left, right;

	top = mNearClippingDistance * tan(mFOV / 2);
	bottom = -top;

	float a = mAspectRatio * tan(mFOV / 2) * mConvergence;

	float b = a - mEyeSeparation / 2;
	float c = a + mEyeSeparation / 2;

	left = -c * mNearClippingDistance / mConvergence;
	right = b * mNearClippingDistance / mConvergence;

	Window::P = glm::frustum(left, right, bottom, top, mNearClippingDistance, mFarClippingDistance);
}

glm::vec3 Window::getPlayerPos() {
	return thisPlayer->headPosition;
}

void Window::setEnemyPlayerPos(float x, float y, float z) {
	enemyPlayer->move(glm::vec3(x, y, z));
}

bool Window::getPlayerSnowballInHand() {
	return thisPlayer->mySnowballs->holdingSnowball;
}

void Window::setEnemySnowballPosition(glm::vec3 pos) {
	glm::vec3 esb;
	if (enemyPlayer->mySnowballs->holdingSnowball) {
		esb = enemyPlayer->mySnowballs->snowballInHand->position;
		//cout << esb.x << " " << esb.y << " " << esb.z << endl;
		if ((abs(esb.x - thisPlayer->headPosition.x) < 150.0f || abs(esb.y - thisPlayer->headPosition.y) < 150.0f) &&
			(abs(esb.z - thisPlayer->headPosition.z) < 3.0f)) {
			cout << "hit" << endl;
			hit = true;
			enemyPlayer->mySnowballs->snowballInHand->updatePosition(glm::vec3(0.0f, 0.0f, -40.0f));
			enemyPlayer->mySnowballs->holdingSnowball = false;
		}
		else {
			enemyPlayer->mySnowballs->snowballInHand->updatePosition(pos);
		}
	}
}

glm::vec3 Window::getPlayerSnowballPos() {
	if (thisPlayer->mySnowballs->holdingSnowball) return thisPlayer->mySnowballs->snowballInHand->position;
	else return glm::vec3(0.0f);
}

void Window::setEnemySnowballinHand(bool inHand) {
	if(inHand && !enemyPlayer->mySnowballs->holdingSnowball) enemyPlayer->grabSnowball();
}

bool Window::getHitByEnemy() {
	return hit;
}

void Window::setEnemyHit(bool enemyHit2) {
	enemyHit = enemyHit2;
}