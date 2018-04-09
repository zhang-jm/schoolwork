#include "OBJObject.h"
#include "Window.h"
#include <iostream>
#include <algorithm>

OBJObject::OBJObject(const char *filepath) 
{
	toWorld = glm::mat4(1.0f);
	this->angle = 0.0f;
	std::cout << "constructor" << std::endl;
	parse(filepath);
	parse(filepath);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	//std::cout << "vbo" << std::endl;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &VBON);
	glBindBuffer(GL_ARRAY_BUFFER, VBON);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	//std::cout << "vbon" << std::endl;

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	//std::cout << "EBO" << std::endl;

	glBindVertexArray(0);
}

OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

std::vector<glm::vec3> OBJObject::getVertices()
{
	return this->vertices;
}

std::vector<glm::vec3> OBJObject::getNormals()
{
	return this->normals;
}

glm::mat4 OBJObject::getToWorld()
{
	return this->toWorld;
}

void OBJObject::setToWorld(glm::mat4 tw)
{
	this->toWorld = tw;
}

void OBJObject::parse(const char *filepath) 
{
	FILE * fp;
	char string[100];
	fpos_t pos;
	float x, y, z;
	float r, g, b;
	float minx = 0, maxx = 0, miny = 0, maxy = 0, minz = 0, maxz = 0;
	unsigned int i1, i2, i3;
	int c1, c2;
	int facecounter = 0;

	std::cout << firstparse << std::endl;

	fopen_s(&fp, filepath, "rb"); //open file
	if (fp == NULL) { std::cout << "error loading file" << std::endl; exit(-1); } //if error

	do {
		fgetpos(fp, &pos);
		fgets(string, 100, fp);

		if (feof(fp)) {
			break;
		}

		if ((string[0] == 'v'))
		{
			if (string[1] == ' ') {
				sscanf_s(string, "v %f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
				
				if (firstparse == 0) {
					if (firstvertice = 0) {
						minx = x;
						miny = y;
						minz = z;
						maxx = x;
						maxy = y;
						maxz = z;

						firstvertice = 1;
					}
					else {
						if (x < minx) { minx = x; }
						if (x > maxx) { maxx = x; }

						if (y < miny) { miny = y; }
						if (y > maxy) { maxy = y; }

						if (z < minz) { minz = z; }
						if (z > maxz) { maxz = z; }
					}
				}

				else {
					//std::cout << r << " " << g << " " << b << std::endl;
					//std::cout << x << std::endl;

					vertices.push_back(glm::vec3((x - avgx) / axis, (y - avgy) / axis, (z - avgz) / axis));
				}

			}

			else if (string[1] == 'n') {
				sscanf_s(string, "vn %f %f %f", &x, &y, &z);
				//std::cout << x << " " << y << " " << z << std::endl;

				if (firstparse == 1) {
					normals.push_back(glm::vec3(x, y, z));
				}
			}
		}

		else if ((string[0] == 'f'))
		{
			if (string[1] == ' ') {
				facecounter++;
				sscanf_s(string, "f %i//%i %i//%i %i//%i", &i1, &i1, &i2, &i2, &i3, &i3);

				if (firstparse == 1) {
					indices.push_back(i1 - 1);
					indices.push_back(i2 - 1);
					indices.push_back(i3 - 1);
				}
			}
		}
	} while (!feof(fp));

	/*std::cout << minx << std::endl;
	std::cout << maxx << std::endl;
	std::cout << miny << std::endl;
	std::cout << maxy << std::endl;
	std::cout << minz << std::endl;
	std::cout << maxz << std::endl;*/

	avgx = (minx + maxx) / 2;
	avgy = (miny + maxy) / 2;
	avgz = (minz + maxz) / 2;

	/*std::cout << avgx << std::endl;
	std::cout << avgy << std::endl;
	std::cout << avgz << std::endl;*/

	axis = std::max(maxx - minx, maxy - miny);
	axis = std::max(axis, maxz - minz);
	//std::cout << axis << std::endl;

	firstparse = 1;

	/*std::cout << vertices.size() << std::endl;
	std::cout << normals.size() << std::endl;
	std::cout << indices.size() << std::endl;
	std::cout << facecounter << std::endl;*/

	fclose(fp);
}

void OBJObject::draw(GLuint shaderProgram, int color, int light) 
{
	sp = shaderProgram;

	modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, 0.0f, 0.0f, 20.0f);

	GLint quadLoc = glGetUniformLocation(shaderProgram, "quad");
	glUniform1f(quadLoc, 0.03f);

	setUpMaterials(shaderProgram, color);

	if (firstdraw == 0) {
		initializeLights(shaderProgram);
		firstdraw = 1;
	}

	setUpLights(shaderProgram, light);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	//std::cout << "drawn" << std::endl;

	glBindVertexArray(0);
}

void OBJObject::setUpMaterials(GLuint shaderProgram, int color) {
	if (color == 0) {
		GLint normalflagLoc = glGetUniformLocation(shaderProgram, "normal");
		glUniform1i(normalflagLoc, 0);


		GLint matAmbientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
		GLint matDiffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
		GLint matSpecularLoc = glGetUniformLocation(shaderProgram, "material.specular");
		GLint matShineLoc = glGetUniformLocation(shaderProgram, "material.shininess");

		glUniform3f(matAmbientLoc, 0.25f, 0.20725f, 0.20725f);
		glUniform3f(matDiffuseLoc, 1.0f, 0.829f, 0.829f);
		glUniform3f(matSpecularLoc, 0.296648f, 0.296648f, 0.296648f);
		glUniform1f(matShineLoc, 8.0f);
	}

	else if (color == 1) {
		GLint normalflagLoc = glGetUniformLocation(shaderProgram, "normal");
		glUniform1i(normalflagLoc, 0);

		GLint matAmbientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
		GLint matDiffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
		GLint matSpecularLoc = glGetUniformLocation(shaderProgram, "material.specular");
		GLint matShineLoc = glGetUniformLocation(shaderProgram, "material.shininess");

		glUniform3f(matAmbientLoc, 0.19225f, 0.19225f, 0.19225f);
		glUniform3f(matDiffuseLoc, 0.50754f, 0.50754f, 0.50754f);
		glUniform3f(matSpecularLoc, 0.508273f, 0.508273f, 0.508273f);
		glUniform1f(matShineLoc, 80.0f);
	}

	else if (color == 2) {
		GLint normalflagLoc = glGetUniformLocation(shaderProgram, "normal");
		glUniform1i(normalflagLoc, 0);

		GLint matAmbientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
		GLint matDiffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
		GLint matSpecularLoc = glGetUniformLocation(shaderProgram, "material.specular");
		GLint matShineLoc = glGetUniformLocation(shaderProgram, "material.shininess");

		glUniform3f(matAmbientLoc, 0.135f, 0.2225f, 0.1575f);
		glUniform3f(matDiffuseLoc, 0.54f, 0.89f, 0.63f);
		glUniform3f(matSpecularLoc, 0.316228f, 0.316228f, 0.316228f);
		glUniform1f(matShineLoc, 20.0f);
	}

	else {
		GLint normalflagLoc = glGetUniformLocation(shaderProgram, "normal");
		glUniform1i(normalflagLoc, 1);
	}
}

void OBJObject::initializeLights(GLuint shaderProgram) {
	xdir = -0.2; ydir = -1.0;
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 1.0f, 1.0f, 1.0f);

	xp = 1; yp = 4; zp = 3;
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.position"), 1.0f, 4.0f, 3.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.ambient"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.specular"), 1.0f, 1.0f, 1.0f);

	xs = 0.5; ys = 0.5; zs = 1;
	xsd = 0.0; ysd = 0.0; zsd = -0.1;
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.position"), 0.5f, 0.5f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.direction"), 0.0f, 0.0f, -0.5f);

	cutoff = 12.5f; exp = 9.5;
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.cutoff"), glm::cos(glm::radians(cutoff)));
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.exponent"), glm::cos(glm::radians(exp)));

	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
}

void OBJObject::setUpLights(GLuint shaderProgram, int light) {
	if (light == 0) {
		GLint lightLoc = glGetUniformLocation(shaderProgram, "light");
		glUniform1i(lightLoc, 0);
	}

	else if (light == 1) {
		GLint lightLoc = glGetUniformLocation(shaderProgram, "light");
		glUniform1i(lightLoc, 1);
	}

	else {
		GLint lightLoc = glGetUniformLocation(shaderProgram, "light");
		glUniform1i(lightLoc, 2);

		glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.cutoff"), glm::cos(glm::radians(cutoff)));
		glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.exponent"), glm::cos(glm::radians(exp)));
	}

	this->light = light;
}

void OBJObject::move(GLuint shaderProgram, float x, float y, float z)
{
	if (lighton && light == 0) {
		
		if (x > 0 && y > 0) {
			glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), xdir - 0.05, ydir - 0.05, -1.3f);
			xdir = xdir - 0.05;
			ydir = ydir - 0.05;
		}
		
		else if (x <= 0 && y > 0) {
			glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), xdir + 0.05, ydir - 0.05, -1.3f);
			xdir = xdir + 0.05;
			ydir = ydir - 0.05;
		}

		else if (x > 0 && y <= 0) {
			glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), xdir - 0.05, ydir + 0.05, -1.3f);
			xdir = xdir - 0.05;
			ydir = ydir + 0.05;
		}

		else if (x <= 0 && y <= 0) {
			glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), xdir + 0.05, ydir + 0.05, -1.3f);
			xdir = xdir + 0.05;
			ydir = ydir + 0.05;
		}
	}

	else {
		glm::mat4 t_mat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
		this->toWorld = t_mat * this->toWorld;
	}
}

void OBJObject::scale(GLuint shaderProgram, float direction)
{
	if(direction == 1)
	{
		if (lighton && light == 1) {
			glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.position"), xp, yp, zp + 0.5f);
			zp = zp + 0.5f;
		}
		else if (lighton && light == 2) {
			glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.position"), xs, ys, zs + 0.1f);
			zs = zs + 0.1f;
			std::cout << zs << std::endl;
		}
		else if (lighton && light == 0) {}
		else if (!lighton) {
			glm::mat4 s_mat = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
			this->toWorld = this->toWorld * s_mat;
		}

	}
	else
	{
		if (lighton && light == 1) {
			glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.position"), xp, yp, zp - 0.5f);
			zp = zp - 0.5f;
		}
		else if (lighton && light == 2) {
			glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.position"), xs, ys, zs - 0.1f);
			zs = zs - 0.1f;
		}
		else if (lighton && light == 0) {}
		else if (!lighton) {
			glm::mat4 s_mat = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
			this->toWorld = this->toWorld * s_mat;
		}
	}

}

void OBJObject::orbit(GLuint shaderProgram, float deg, float x, float y, float z)
{
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), deg, glm::vec3(x, y, z));

	if (lighton && light == 1) {
		glm::vec4 currentdir(xp, yp, zp, 1.0f);
		glm::vec4 newdir = rot * currentdir;

		//std::cout << "blah" << std::endl;
		glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.position"), newdir.x, newdir.y, zp);

		xp = newdir.x;
		yp = newdir.y;
	}

	else if (lighton && light == 2) {
		glm::vec4 currentdir(xs, ys, zs, 1.0f);
		glm::vec4 newdir = rot * currentdir;
		//std::cout << "blah" << std::endl;

		glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.position"), newdir.x, newdir.y, zs);
		//glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.direction"), -newdir.x, -newdir.y, -zs);
		xs = newdir.x;
		ys = newdir.y;

		xsd = -newdir.x;
		ysd = -newdir.y;
		zsd = -zs;
	}

	else {
		this->toWorld = rot * this->toWorld;

		modelview = Window::V * toWorld;
		glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	}

}

void OBJObject::reset()
{
	this->toWorld = glm::mat4(1.0f);
}