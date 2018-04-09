#include "window.h"
#include "Group.cpp"
#include "MatrixTransform.cpp"
#include "Sphere.cpp"
#include <chrono>

using namespace std::chrono;

const char* window_title = "GLFW Starter Project";
Cube * skybox;

Group * robot;
Group * robotArmy;
MatrixTransform * body;
MatrixTransform * leftArm;
MatrixTransform * rightArm;
MatrixTransform * leftLeg;
MatrixTransform * rightLeg;
MatrixTransform * head;
MatrixTransform * bound;
Cube * cube1;
Cube * cube2;
glm::mat4 robotmodel = glm::mat4(1.0f);
Sphere * sphere;
bool culling = 0;

GLint shaderProgram1;
GLint skyboxShader;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH1 "../shader.vert"
#define FRAGMENT_SHADER_PATH1 "../shader.frag"
#define SKY_VERTEX_SHADER_PATH "../skyshader.vert"
#define SKY_FRAGMENT_SHADER_PATH "../skyshader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 200.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

float hnear, wnear, hfar, wfar;
glm::vec3 d, up, rightvec;
glm::vec3 fc, nc, tp, bp, rp, lp;
glm::vec3 rnorm, lnorm, tnorm, bnorm, fnorm, nnorm;

glm::mat4 Window::P;
glm::mat4 Window::V;

bool left_held;
bool right_held;
glm::vec3 lastPoint;

milliseconds lasttime = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

void Window::initialize_objects()
{
	// Load the shader program. Make sure you have the correct filepath up top
    skyboxShader = LoadShaders(SKY_VERTEX_SHADER_PATH, SKY_FRAGMENT_SHADER_PATH);
	shaderProgram1 = LoadShaders(VERTEX_SHADER_PATH1, FRAGMENT_SHADER_PATH1);

	skybox = new Cube(skyboxShader, 1, 0.0f);
	cube1 = new Cube(shaderProgram1, 0, 1.0f);
	cube2 = new Cube(shaderProgram1, 0, 2.0f);
	sphere = new Sphere(shaderProgram1, 1.0f);
	robot = new Group();
	robotArmy = new Group();
	createRobot();
	createRobotArmy();
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(skybox);
	delete(robot);
	delete(robotArmy);
	glDeleteProgram(shaderProgram1);
	glDeleteProgram(skyboxShader);
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

	glewExperimental = GL_TRUE;
	glewInit();

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
		Window::setUpCulling();
	}
}

void Window::idle_callback()
{
}

void Window::display_callback(GLFWwindow* window)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(skyboxShader);
	skybox->drawSkybox(skyboxShader);

	glUseProgram(shaderProgram1);
	//robot->draw(robotmodel);
	setUpCulling();
	robotArmy->draw(robotmodel);

	robotmodel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.005f)) * robotmodel;

	milliseconds now = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
	cout << (now - lasttime).count() << endl;
	lasttime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	//sphere->draw(glm::mat4(1.0f));

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

		else if (key == GLFW_KEY_A)
		{
			robotmodel = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)) * robotmodel;
		}

		else if (key == GLFW_KEY_S)
		{
			robotmodel = glm::scale(glm::mat4(1.0f), glm::vec3(1.3f, 1.3f, 1.3f)) * robotmodel;
		}

		else if (key == GLFW_KEY_D)
		{	
			sphere->drawn = !sphere->drawn;
		}

		else if (key == GLFW_KEY_C) {
			culling = !culling;
			createRobotArmy();
		}
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		std::cout << "L Mouse held" << std::endl;
		left_held = 1;
	}

	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		std::cout << "L Mouse release" << std::endl;
		left_held = 0;
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		std::cout << "L Mouse held" << std::endl;
		right_held = 1;
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		std::cout << "L Mouse release" << std::endl;
		right_held = 0;
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (left_held == 1) {

		glm::vec3 currPoint = trackBallMapping(xpos, ypos);
		glm::vec3 direction = currPoint - lastPoint;
		float velocity = glm::length(direction);

		if (velocity > 0.0001) {

			glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
			float rot_angle = velocity * 2;

			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rot_angle, glm::vec3(rotAxis.x, rotAxis.y, rotAxis.z));
			glm::vec4 rotated = rot * glm::vec4(cam_pos, 0.0f);
			cam_pos.x = rotated.x;
			cam_pos.y = rotated.y;
			cam_pos.z = rotated.z;

			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

	}

	else if (right_held == 1) {
		glm::vec3 currPoint = trackBallMapping(xpos, ypos);
		glm::vec3 direction = currPoint - lastPoint;
		float velocity = glm::length(direction);

		if (velocity > 0.0001) {

			glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
			float rot_angle = velocity * 2;

			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rot_angle, glm::vec3(rotAxis.x, rotAxis.y, rotAxis.z));
			robotmodel = rot * robotmodel;
		}
	}

	lastPoint = trackBallMapping(xpos, ypos);
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

void Window::createRobot() {
	body = new MatrixTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 2.0f, 1.5f)), 0, 0, glm::vec3(1.0f));
	body -> addChild(cube1);

	head = new MatrixTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.7f, 0.7f, 0.7f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 9.0f, 0.0f)), 0, 0, glm::vec3(0.7f, 0.7f, 0.7f));
	head->addChild(cube2);

	rightArm = new MatrixTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 2.0f, 0.8f)) * glm::translate(glm::mat4(1.0f), glm::vec3(9.2f, 0.0f, 0.0f)), 1, 0, glm::vec3(0.5f, 2.0f, 0.8f));
	rightArm->addChild(cube2);

	leftArm = new MatrixTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 2.0f, 0.8f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-9.2f, 0.0f, 0.0f)), 1, 1, glm::vec3(0.5f, 2.0f, 0.8f));
	leftArm->addChild(cube2);

	rightLeg = new MatrixTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 2.0f, 0.8f)) * glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -4.5, 0.0f)), 1, 0, glm::vec3(0.5f, 2.0f, 0.8f));
	rightLeg->addChild(cube2);

	leftLeg = new MatrixTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 2.0f, 0.8f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -4.5f, 0.0f)), 1, 1, glm::vec3(0.5f, 2.0f, 0.8f));
	leftLeg->addChild(cube2);

	bound = new MatrixTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.85f, 0.0f)), 0, 0, glm::vec3(0.0f));
	bound->addChild(sphere);

	robot -> addChild(body);
	robot -> addChild(head);
	robot->addChild(rightArm);
	robot->addChild(leftArm);
	robot->addChild(leftLeg);
	robot->addChild(rightLeg);
	robot->addChild(bound);
}

Group * Window::createRobotArmy() {
	int drawn = 0;
	robotArmy = new Group();
	/*cout << rightvec.x << " " << rightvec.y << " " << rightvec.z << " " << endl;
	cout << d.x << " " << d.y << " " << d.z << " " << endl;
	cout << up.x << " " << up.y << " " << up.z << " " << endl;*/
	//cout << rnorm.x << " " << rnorm.y << " " << rnorm.z << " " << endl;

	//cout << lnorm.x << " " << lnorm.y << " " << lnorm.z << " " << endl;

	/*cout << tnorm.x << " " << tnorm.y << " " << tnorm.z << " " << endl;

	cout << bnorm.x << " " << bnorm.y << " " << bnorm.z << " " << endl;*/
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			MatrixTransform * m = new MatrixTransform(glm::translate(glm::mat4(1.0f), glm::vec3((float)i * 20.0f, 0.0f, (float)j * 20.0f)), 0, 0, glm::vec3(1.0f));
			m->addChild(robot);
			drawn++;

			if (culling) {
				int result = testCulling(glm::vec3((float)i * 20.0f, 0.0f, (float)j * 20.0f), 30.0f);
				//cout << result << endl;
				if (result == 0) {
					m->drawn = 0;
					drawn--;
				}
				else {
					m->drawn = 1;
				}
			}
			robotArmy->addChild(m);
		}
	}

	//cout << drawn << endl;
	return robotArmy;
}

void Window::setUpCulling() {
	//cout << "set up culling" << endl;
	//help & info retrieved from http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-implementation/
	hnear = tan((45.0f * (3.14159 / 180.0))/2) * 0.1f;
	wnear = hnear * (width / height);

	hfar = tan((45.0f * (3.14159/180.0)) / 2) * 1000.0f;
	wfar = hfar * (width / height);

	d = glm::normalize(cam_pos - cam_look_at);
	rightvec = glm::normalize(glm::cross(cam_up, d));
	up = glm::normalize(glm::cross(d, rightvec));

	fc = cam_pos - d * 1000.0f;
	fnorm = d;
	
	nc = cam_pos - d * 0.1f;
	nnorm = -d;

	glm::vec3 a;
	tp = (nc + up * hnear); //top
	a = glm::normalize(tp - cam_pos);
	tnorm = glm::normalize(glm::cross(a, rightvec));

	bp = (nc - up * hnear); //bottom
	a = glm::normalize(bp - cam_pos);
	bnorm = glm::normalize(glm::cross(rightvec, a));

	rp = (nc + rightvec * wnear); //right
	a = glm::normalize(rp - cam_pos);
	rnorm = glm::normalize(glm::cross(up, a));

	lp = (nc - rightvec * wnear); //left
	a = glm::normalize(lp - cam_pos);
	lnorm = glm::normalize(glm::cross(a, up));

}

int Window::testCulling(glm::vec3 spherecenter, float radius) {
	int result = 1; //inside
	float distance;

	glm::vec3 normals[] = { fnorm, nnorm, rnorm, lnorm, tnorm, bnorm };
	glm::vec3 points[] = { fc, nc, rp, lp, tp, bp };
	
	for (int i = 0; i < 6; i++) {
		glm::vec3 pnorm = normals[i];
		glm::vec3 ppoint = points[i];

		distance = glm::dot(spherecenter - ppoint, pnorm);
		/*cout << spherecenter.x - ppoint.x << " " << spherecenter.y - ppoint.y << " " << spherecenter.z - ppoint.z << endl;
		cout << pnorm.x << " " << pnorm.y << " " << pnorm.z << endl;
		cout << distance << endl;*/

		if (distance < -radius) {
			//cout << i << endl;
			return 0;
		}
		else if (distance < radius)
		{
			result = 2;
		}
	}

	return result;
}