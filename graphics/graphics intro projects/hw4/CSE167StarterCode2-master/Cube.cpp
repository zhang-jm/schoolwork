#include "Cube.h"
#include "Window.h"
#include "texture.cpp"

Cube::Cube(GLuint shaderProgram, bool skybox)
{
	toWorld = glm::mat4(1.0f);
	this->shaderProgram = shaderProgram;
	this->skybox = skybox;
	setUpSkybox();

	if (skybox) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	else {
		cout << "sphere" << endl;
 		setUpSphere(0.5f, 20, 20);
		//cout << sphereVertices.size() << endl;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &VBON);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(glm::vec3), sphereVertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, VBON);
		glBufferData(GL_ARRAY_BUFFER, sphereNormals.size() * sizeof(glm::vec3), sphereNormals.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

Cube::~Cube()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Cube::drawSkybox(GLuint shaderProgram)
{ 
	glDepthMask(GL_FALSE);
	glm::mat4 view = Window::V;
	glm::mat4 newmv = glm::mat4(glm::mat3(view));

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &newmv[0][0]);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

void Cube::setUpSkybox() {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	if (skybox) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	unsigned char * right = loadPPM("../cottoncandy_rt.ppm", Window::width, Window::height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, Window::width, Window::height, 0, GL_RGB, GL_UNSIGNED_BYTE, right);

	unsigned char * left = loadPPM("../cottoncandy_lf.ppm", Window::width, Window::height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, Window::width, Window::height, 0, GL_RGB, GL_UNSIGNED_BYTE, left);

	unsigned char * top = loadPPM("../cottoncandy_up.ppm", Window::width, Window::height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, Window::width, Window::height, 0, GL_RGB, GL_UNSIGNED_BYTE, top);

	unsigned char * bottom = loadPPM("../cottoncandy_dn.ppm", Window::width, Window::height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, Window::width, Window::height, 0, GL_RGB, GL_UNSIGNED_BYTE, bottom);

	unsigned char * back = loadPPM("../cottoncandy_bk.ppm", Window::width, Window::height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, Window::width, Window::height, 0, GL_RGB, GL_UNSIGNED_BYTE, back);

	unsigned char * front = loadPPM("../cottoncandy_ft.ppm", Window::width, Window::height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, Window::width, Window::height, 0, GL_RGB, GL_UNSIGNED_BYTE, front);

	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Select GL_MODULATE to mix texture with polygon color for shading:
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Use bilinear interpolation:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Use clamp to edge to hide skybox edges:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cube::setUpSphere(float radius, int stacks, int slices) {
	if (prevStacks != stacks || prevSlices != slices) {
		prevStacks = stacks; prevSlices = slices;

		sphereVertices.clear();
		sphereNormals.clear();

		float fstacks = (float)stacks;
		float fslices = (float)slices;

		for (int i = 0; i < slices; i++) {
			for (int j = 0; j < stacks; j++) {
				// Top left
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
					radius * -cos(pi * (j + 1.0f) / fslices),
					radius * sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices)));

				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
					-cos(pi * (j + 1.0f) / fslices),
					sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices))));

				// Top right
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices),
					radius * -cos(pi * (j + 1.0) / fslices),
					radius * sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices),
					-cos(pi * (j + 1.0) / fslices),
					sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices))));

				// Bottom right
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
					radius * -cos(pi * j / fslices),
					radius * sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
					-cos(pi * j / fslices),
					sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices))));

				// Need to repeat 2 of the vertices since we can only draw triangles. Eliminates the confusion
				// of array indices.
				// Top left
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
					radius * -cos(pi * (j + 1.0f) / fslices),
					radius * sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
					-cos(pi * (j + 1.0f) / fslices),
					sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices))));

				// Bottom right
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
					radius * -cos(pi * j / fslices),
					radius * sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
					-cos(pi * j / fslices),
					sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices))));

				// Bottom left
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * pi * i / fstacks) * sin(pi * j / fslices),
					radius * -cos(pi * j / fslices),
					radius * sin(2.0f * pi * i / fstacks) * sin(pi * j / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * pi * i / fstacks) * sin(pi * j / fslices),
					-cos(pi * j / fslices),
					sin(2.0f * pi * i / fstacks) * sin(pi * j / fslices))));
			}
		}
	}
}

void Cube::draw(glm::mat4 C) {
	toWorld = C;

	Cube::render();
}

void Cube::update(glm::mat4 C){

}

void Cube::render() {
	glm::mat4 modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(this->shaderProgram, "projection");
	uModelview = glGetUniformLocation(this->shaderProgram, "modelview");
	GLuint uModel = glGetUniformLocation(this->shaderProgram, "model");
	GLuint uCamPos = glGetUniformLocation(this->shaderProgram, "cameraPos");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);

	glm::vec3 pos = Window::getCamPos();
	glUniform3f(uCamPos, pos.x, pos.y, pos.z);

	glBindVertexArray(VAO);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	//glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);
	glDrawArrays(GL_TRIANGLES, 0, sphereVertices.size());
	glBindVertexArray(0);
}

