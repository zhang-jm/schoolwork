#include "window.h"
#include "OBJObject.cpp"

const char* window_title = "GLFW Starter Project";
Cube * cube;

OBJObject * current; 

OBJObject * bunny;
OBJObject * bear; 
OBJObject * dragon; 

GLint shaderProgram;

int shift_pressed = 0;
int left_held = 0;
int right_held = 0;
int firstpress = 0;
bool light_control = 0;

double oldx = 0;
double oldy = 0;
glm::vec3 lastPoint;
int color = 0;
int light = 0;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "shader.vert"
#define FRAGMENT_SHADER_PATH "shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	//cube = new Cube();
	std::cout << "initializing" << std::endl;
	bunny = new OBJObject("bunny.obj");
	bear = new OBJObject("bear.obj");
	dragon = new OBJObject("dragon.obj");

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	//delete(cube);
	delete(bunny);
	delete(bear);
	delete(dragon);
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
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// Render the cube
	//cube->draw(shaderProgram);
	//bunny->draw(shaderProgram, color, light);
	if (current != NULL)
	{
		current->draw(shaderProgram, color, light);
	}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << light_control << std::endl;
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		else if (key == GLFW_KEY_F1)
		{
			current = bunny;
			current->setToWorld(glm::mat4(1.0f));
			current->lighton = light_control;
			color = 0;
		}

		else if (key == GLFW_KEY_F2)
		{
			current = bear;
			current->setToWorld(glm::mat4(1.0f));
			current->lighton = light_control;
			color = 1;
		}

		else if (key == GLFW_KEY_F3)
		{
			current = dragon;
			current->setToWorld(glm::mat4(1.0f));
			current->lighton = light_control;
			color = 2;
		}

		else if (key == GLFW_KEY_S)
		{
			if (current != NULL)
			{
				if (shift_pressed == 1)
				{
					current->scale(shaderProgram, 1);
				}

				else
				{
					current->scale(shaderProgram, 2);
				}
			}

			shift_pressed = 0;
		}

		else if (key == GLFW_KEY_E)
		{
			if (current != NULL)
			{
				if (light_control == 1) 
				{
					if (shift_pressed == 1)
					{
						current->exp = current->exp + 0.5;
					}

					else
					{
						current->exp = current->exp - 0.5;
						if (current->exp < 0) { current->exp = 0; }
					}
					std::cout << current->exp << std::endl;
				}

			}

			shift_pressed = 0;
		}

		else if (key == (GLFW_KEY_LEFT_SHIFT))
		{
			shift_pressed = 1;
		}

		else if (key == GLFW_KEY_R)
		{
			current->reset();
		}

		else if (key == GLFW_KEY_0)
		{
			light_control = 0;
			current->lighton = 0;
		}

		else if (key == GLFW_KEY_1)
		{
			light_control = 1;
			current->lighton = 1;
			light = 0;
		}

		else if (key == GLFW_KEY_2)
		{
			light_control = 1;
			current->lighton = 1;
			light = 1;
		}

		else if (key == GLFW_KEY_3)
		{
			light_control = 1;
			current->lighton = 1;
			light = 2;
		}

		else if (key == GLFW_KEY_N)
		{
			if (current == bunny && color == 3) {
				color = 0;
			}
			else if (current == bear && color == 3) {
				color = 1;
			}
			else if (current == dragon && color == 3) {
				color = 2;
			}
			else {
				color = 3;
			}
		}
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		//std::cout << "Mouse held" << std::endl;
		right_held = 1;
	}
	
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		//std::cout << "Mouse release" << std::endl;
		oldx = 0;
		oldy = 0;
		right_held = 0;
	}

	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		std::cout << "L Mouse held" << std::endl;
		left_held = 1;
	}

	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		std::cout << "L Mouse release" << std::endl;
		left_held = 0;
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	float x = (float)(xpos - oldx) * 0.05;
	float y = (float)(oldy - ypos) * 0.05;

	//std::cout << "cursor" << std::endl;
	if (right_held == 1)
	{
		if (light_control == 0) {
			//std::cout << x << " " << y << std::endl;
			current->move(shaderProgram, x, y, 0.0f);
		}
		else {
			if (light == 2 && y > 0)
			{
				if (current->cutoff < 59) {
					current->cutoff = current->cutoff + 0.2f;
				}
				else {
					current->cutoff = 59;
				}
			}

			else if(light == 2 && y < 0){
				if (current->cutoff > 0) {
					current->cutoff = current->cutoff - 0.2f;
				}
				else {
					current->cutoff = 0;
				}
				//::cout << current->cutoff << std::endl;
			}
		}
	}

	if (left_held == 1) {

			glm::vec3 currPoint = trackBallMapping(xpos, ypos);
			glm::vec3 direction = currPoint - lastPoint;
			float velocity = glm::length(direction);

			if (velocity > 0.0001) {

				glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
				float rot_angle = velocity * 2;

				if (light_control == 0) {
					current->orbit(shaderProgram, rot_angle, rotAxis.x, rotAxis.y, rotAxis.z);
				}
				else {
					if (light == 0)
					{
						current->move(shaderProgram, x, y, 0);
					}
					else if (light == 1) {
						current->orbit(shaderProgram, rot_angle, rotAxis.x, rotAxis.y, rotAxis.z);
					}
					else if (light == 2)
					{
						current->orbit(shaderProgram, rot_angle, rotAxis.x, rotAxis.y, rotAxis.z);
					}
				}
			}

	}

	oldx = xpos;
	oldy = ypos;

	lastPoint = trackBallMapping(xpos, ypos);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//std::cout << yoffset << std::endl;
	if (yoffset < 0)
	{
		if (light_control == 0) {
			current->move(shaderProgram, 0, 0, 0.5f);
		}
		else {
			current->scale(shaderProgram, 1);
		}
	}
	else 
	{ 
		if (light_control == 0) {
			current->move(shaderProgram, 0, 0, -0.5f);
		}
		else {
			current->scale(shaderProgram, 0);
		} 
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