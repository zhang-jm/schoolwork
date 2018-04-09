#include "window.h"
#include "Sphere.cpp"
#include "Control.cpp"
#include "Point.cpp"

const char* window_title = "GLFW Starter Project";
Cube * skybox;
Cube * sph;
Sphere * sphere;
Control * control;

bool paused = 1;

GLint shaderProgram1;
GLint selShader;
GLint skyboxShader;
GLint mapShader;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH1 "../shader.vert"
#define FRAGMENT_SHADER_PATH1 "../shader.frag"
#define VERTEX_SHADER_PATH2 "../selshader.vert"
#define FRAGMENT_SHADER_PATH2 "../selshader.frag"
#define VERTEX_SHADER_PATH3 "../mapshader.vert"
#define FRAGMENT_SHADER_PATH3 "../mapshader.frag"
#define SKY_VERTEX_SHADER_PATH "../skyshader.vert"
#define SKY_FRAGMENT_SHADER_PATH "../skyshader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width = 640;
int Window::height = 480;

float hnear, wnear, hfar, wfar;
glm::vec3 d, up, rightvec;
glm::vec3 fc, nc, tp, bp, rp, lp;
glm::vec3 rnorm, lnorm, tnorm, bnorm, fnorm, nnorm;

glm::mat4 Window::P;
glm::mat4 Window::V;

bool left_held;
bool right_held;
glm::vec3 lastPoint;

double xpos1, ypos1;
double lastX, lastY;
Point * selected;
glm::vec3 currentsphereloc;

void Window::initialize_objects()
{
	// Load the shader program. Make sure you have the correct filepath up top
    skyboxShader = LoadShaders(SKY_VERTEX_SHADER_PATH, SKY_FRAGMENT_SHADER_PATH);
	shaderProgram1 = LoadShaders(VERTEX_SHADER_PATH1, FRAGMENT_SHADER_PATH1);
	selShader = LoadShaders(VERTEX_SHADER_PATH2, FRAGMENT_SHADER_PATH2);
	mapShader = LoadShaders(VERTEX_SHADER_PATH3, FRAGMENT_SHADER_PATH3);

	skybox = new Cube(skyboxShader, 1);
	control = new Control(shaderProgram1);
	//sphere = new Sphere(mapShader, 1);
	sph = new Cube(mapShader, 0);

	resetSphere();
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(skybox);
	delete(control);
	//delete(sphere);

	glDeleteProgram(shaderProgram1);
	glDeleteProgram(skyboxShader);
}

glm::vec3 Window::getCamPos() {
	return cam_pos;
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
	control->draw();

	glUseProgram(mapShader);
	sph->render();
	
	/*glUseProgram(shaderProgram1);

	for (int i = 0; i < control->pointstodraw.size(); i++) {
		control->pointstodraw.at(i)->selectionDraw(shaderProgram1);
	}*/

	if (!paused) {
		glm::vec3 nextpoint = control->calculateNextPoint();
		//cout << nextpoint.x << endl;
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), nextpoint - currentsphereloc);
		currentsphereloc = nextpoint;

		sph->toWorld = trans * sph->toWorld;
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

		else if (key == GLFW_KEY_R) {
			control->reset();
			resetSphere();
		}

		else if (key == GLFW_KEY_P) {
			paused = !paused;
		}

		else if (key == GLFW_KEY_N) {
			glm::vec3 nextpoint = control->calculateNextPoint();
			//cout << nextpoint.x << endl;
			glm::mat4 trans = glm::translate(glm::mat4(1.0f), nextpoint - currentsphereloc);
			currentsphereloc = nextpoint;

			sph->toWorld = trans * sph->toWorld;
		}
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		std::cout << "L Mouse held" << std::endl;
		lastX = xpos1;
		lastY = ypos1;
		left_held = 1;
	}

	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		std::cout << "L Mouse release" << std::endl;
		left_held = 0;
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		right_held = 1;
		
		glfwGetCursorPos(window, &xpos1, &ypos1);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram1);
		for (int i = 0; i < control->pointstodraw.size(); i++) {
			control->pointstodraw.at(i)->selectionDraw(shaderProgram1);
		}

		unsigned char pix[4];
		//cout << lastX << " " << height - lastY << endl;
		glReadPixels(xpos1, height - ypos1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pix);

		cout << (int)pix[0] << " " << (int)pix[1] << " " << (int)pix[2] << " " << (int)pix[3] << endl;
 		if ((int)pix[0] != 0 && (int)pix[0] <= control->pointstodraw.size()) {
			selected = control->pointstodraw[(int)pix[0] - 1];
		}
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		right_held = 0;
		selected = 0;
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

	else if (right_held == 1) {
		if (selected)
		{
			selected->update((xpos - xpos1) / 100, (ypos1 - ypos) / 100, 0.0f);

			if (selected->color == 2) { //endpoint
				control->pointstodraw.at(selected->ID - 1)->update((xpos - xpos1) / 100, (ypos1 - ypos) / 100, 0.0f);
				control->pointstodraw.at(selected->ID + 1)->update((xpos - xpos1) / 100, (ypos1 - ypos) / 100, 0.0f);
			}
			else { //control
				if (selected->ID == 0 || (selected->ID != 29 && control->pointstodraw.at(selected->ID + 1)->color == 2)) {
					control->pointstodraw.at(selected->ID + 2)->update(-(xpos - xpos1) / 100, -(ypos1 - ypos) / 100, 0.0f);
				}
				else {
					control->pointstodraw.at(selected->ID - 2)->update(-(xpos - xpos1) / 100, -(ypos1 - ypos) / 100, 0.0f);
				}
			}
			control->setUpCurves();
			glm::vec3 currPoint = trackBallMapping(xpos, ypos);
			glm::vec3 direction = currPoint - lastPoint;
			float velocity = glm::length(direction);

			if (velocity > 0.0001) {

				glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
				float rot_angle = velocity * 2;

				glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rot_angle, glm::vec3(rotAxis.x, rotAxis.y, rotAxis.z));
			}
		}
	}

	xpos1 = xpos;
	ypos1 = ypos;
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

void Window::resetSphere() {
	currentsphereloc = control->highest;
	glm::vec3 translation = control->highest - glm::vec3(0.0f);
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);

	sph->toWorld = trans * glm::mat4(1.0f);
}