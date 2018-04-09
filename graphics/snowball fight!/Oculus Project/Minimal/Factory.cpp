#include "Factory.h"
#include "Window.h"

Factory::Factory()
{
	toWorld = glm::mat4(1.0f);
	toWorld = toWorld * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	
	model = new Model("./assets/factory3/factory3.obj");
}

Factory::~Factory()
{
	delete(model);
}

void Factory::draw(GLuint shaderProgram, const glm::mat4 & projection, const glm::mat4 & modelview)
{
	model->Draw(toWorld, shaderProgram, projection, modelview, false);
	
}