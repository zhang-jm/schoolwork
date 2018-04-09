#include "Factory.h"
#include "Window.h"

Factory::Factory()
{
	toWorld = glm::mat4(1.0f);
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
	model = new Model("../assets/factory3/factory3.obj");
}

Factory::~Factory()
{
	delete(model);
}

void Factory::draw(GLuint shaderProgram)
{
	model->Draw(toWorld, shaderProgram);
}