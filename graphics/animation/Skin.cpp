#include "Skin.h"

Skin::Skin(Skeleton * s) {
	skel = s;
	toWorld = glm::mat4(1.0f);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBON);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBON);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Skin::~Skin() {
}

void Skin::draw(GLuint shaderProgram) {
	glm::mat4 modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, 0.0f, 0.0f, 20.0f);

	GLint quadLoc = glGetUniformLocation(shaderProgram, "quad");
	glUniform1f(quadLoc, 0.03f);

	GLint matAmbientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
	GLint matDiffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
	GLint matSpecularLoc = glGetUniformLocation(shaderProgram, "material.specular");
	GLint matShineLoc = glGetUniformLocation(shaderProgram, "material.shininess");

	glUniform3f(matAmbientLoc, 0.25f, 0.20725f, 0.20725f);
	glUniform3f(matDiffuseLoc, 1.0f, 0.829f, 0.829f);
	glUniform3f(matSpecularLoc, 0.296648f, 0.296648f, 0.296648f);
	glUniform1f(matShineLoc, 8.0f);

	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.position"), -6.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 1.0f, 1.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 1.0f, 1.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 1.0f, 1.0f, 0.0f);
	
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight2.position"), 6.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight2.ambient"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight2.diffuse"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight2.specular"), 0.0f, 0.0f, 1.0f);

	glEnable(GL_CULL_FACE);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, triangleIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Skin::update() {
	bindpos.clear();
	bindnorms.clear();

	int skinweightindex = 0;
	for (int j = 0; j < positions.size(); j++) {
		int numWeights = skinweightnums.at(j);

		glm::vec4 vprime = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		glm::vec4 nprime = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		for (int k = 0; k < numWeights; k++) {
			glm::vec2 skinweight = skinweights.at(skinweightindex);
			int jointnum = skinweight.x;

			vprime = vprime + skinweight.y * skel->getWorldMatrix(jointnum) * bindings.at(jointnum) * glm::vec4(positions.at(j), 1.0f);
			nprime = nprime + skinweight.y * skel->getWorldMatrix(jointnum) * bindings.at(jointnum) * glm::vec4(normals.at(j), 0.0f);
			skinweightindex++;
		}
		bindpos.push_back(glm::vec3(vprime.x, vprime.y, vprime.z));
		glm::vec3 normalize = glm::normalize(glm::vec3(nprime.x, nprime.y, nprime.z));
		bindnorms.push_back(normalize);
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bindpos.size() * sizeof(glm::vec3), bindpos.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBON);
	glBufferData(GL_ARRAY_BUFFER, bindnorms.size() * sizeof(glm::vec3), bindnorms.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleIndices.size() * sizeof(int), triangleIndices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Skin::parse(char * filename) {
	FILE* fp;
	char string[100];
	fpos_t pos;

	fopen_s(&fp, filename, "rb");
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }

	do {
		fscanf_s(fp, "%s", string);
		std::cout << string << std::endl;

		if (feof(fp)) {
			break;
		}

		if ((string[0] == 'p') && (string[1] == 'o')) //positions
		{
			std::cout << "positions" << std::endl;

			float numPositions;

			fscanf_s(fp, "%f", &numPositions);
			cout << numPositions << endl;

			fscanf_s(fp, "%s", string);
			cout << string << endl;
				
			for (int i = 0; i < numPositions; i++) {
				float x, y, z;

				fscanf_s(fp, "%f %f %f", &x, &y, &z);
				positions.push_back(glm::vec3(x, y, z));
			}

			cout << positions.size() << endl;

			fscanf_s(fp, "%s", string);
			cout << string << endl;
		}
		else if ((string[0] == 'n') && (string[1] == 'o')) //normals
		{
			std::cout << "norms" << std::endl;

			float numPositions;

			fscanf_s(fp, "%f", &numPositions);
			cout << numPositions << endl;

			fscanf_s(fp, "%s", string);
			cout << string << endl;

			for (int i = 0; i < numPositions; i++) {
				float x, y, z;

				fscanf_s(fp, "%f %f %f", &x, &y, &z);
				normals.push_back(glm::vec3(x, y, z));
			}

			cout << normals.size() << endl;

			fscanf_s(fp, "%s", string);
			cout << string << endl;
		}
		else if ((string[0] == 's') && (string[1] == 'k')) //skinweights
		{
			std::cout << "weights" << std::endl;

			float numPositions;

			fscanf_s(fp, "%f", &numPositions);
			cout << numPositions << endl;

			fscanf_s(fp, "%s", string);
			cout << string << endl;

			for (int i = 0; i < numPositions; i++) {
				float numAttachments;
				fscanf_s(fp, "%f", &numAttachments);
				skinweightnums.push_back(numAttachments);

				for (int j = 0; j < numAttachments; j++) {
					float x, y;

					fscanf_s(fp, "%f %f", &x, &y);
					skinweights.push_back(glm::vec2(x, y));
				}
			}

			cout << skinweights.size() << endl;

			fscanf_s(fp, "%s", string);
			cout << string << endl;
		}
		else if ((string[0] == 't') && (string[1] == 'r')) //triangles
		{
			std::cout << "triangles" << std::endl;

			float numPositions;

			fscanf_s(fp, "%f", &numPositions);
			cout << numPositions << endl;

			fscanf_s(fp, "%s", string);
			cout << string << endl;

			for (int i = 0; i < numPositions; i++) {
				float x, y, z;

				fscanf_s(fp, "%f %f %f", &x, &y, &z);
				triangles.push_back(glm::vec3(x, y, z));
				triangleIndices.push_back(x);
				triangleIndices.push_back(y);
				triangleIndices.push_back(z);
			}

			cout << triangles.size() << endl;

			fscanf_s(fp, "%s", string);
			cout << string << endl;
		}
		else if ((string[0] == 'm') && (string[1] == 'a')) //matrix
		{
			std::cout << "matrix" << std::endl;

			fscanf_s(fp, "%s", string);
			cout << string << endl;

			float ax, bx, cx, dx, ay, by, cy, dy, az, bz, cz, dz;
			fscanf_s(fp, "%f %f %f %f %f %f %f %f %f %f %f %f", 
				&ax, &ay, &az, &bx, &by, &bz, &cx, &cy, &cz, &dx, &dy, &dz);

			/*glm::vec4 col1 = glm::vec4(ax, bx, cx, dx);
			glm::vec4 col2 = glm::vec4(ay, by, cy, dy);
			glm::vec4 col3 = glm::vec4(az, bz, cz, dz);
			glm::mat4x4 B = glm::mat4x4(col1, col2, col3, glm::vec4(0, 0, 0, 1));*/
			glm::mat4x4 B = glm::mat4x4(
				ax, ay, az, 0,
				bx, by, bz, 0,
				cx, cy, cz, 0,
				dx, dy, dz, 1
			);
			const float *pSource = (const float*)glm::value_ptr(B);
			for (int i = 0; i < 16; ++i) {
				cout << pSource[i] << " ";
			}
			cout << "" << endl;

			glm::mat4x4 Binv = glm::inverse(B);
			
			bindings.push_back(Binv);

			fscanf_s(fp, "%s", string);
			cout << string << endl;
		}
	} while (!feof(fp));
	
	cout << bindings.size() << endl;

	fclose(fp);
}