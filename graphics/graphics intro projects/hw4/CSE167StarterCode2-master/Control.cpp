#include "Control.h"
#include "Window.h"

Control::Control(GLuint shaderProgram)
{
	toWorld = glm::mat4(1.0f);
	this->shaderProgram = shaderProgram;

	addPoints();
	setUpCurves();

	cout << curve1.size() << endl;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, curve1.size() * sizeof(glm::vec3), curve1.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Control::~Control()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Control::draw()
{ 
	glm::mat4 view = Window::V * toWorld;

	uProjection = glGetUniformLocation(this->shaderProgram, "projection");
	uModelview = glGetUniformLocation(this->shaderProgram, "modelview");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &view[0][0]);
	GLuint uColor = glGetUniformLocation(this->shaderProgram, "color");
	glUniform1i(uColor, 1);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, curve1.size() * sizeof(glm::vec3), curve1.data(), GL_STATIC_DRAW);

	glDrawArrays(GL_LINES, 0, curve1.size() - pointsformatrix.size());

	glUniform1i(uColor, 4);

	glDrawArrays(GL_LINES, curve1.size() - pointsformatrix.size(), pointsformatrix.size());

	glBindVertexArray(0);

	for (int i = 0; i < pointstodraw.size(); i++) {
		pointstodraw[i]->draw(this->shaderProgram);
	}

	//cout << pointsformatrix[0]->location.at(0).x << endl;
}

void Control::update(glm::mat4 C){

}

void Control::addPoints() {
	Point * c20 = new Point(-5.5f, 0.0f, 2.8f, 0, 3);
	Point * end1 = new Point(-5.0f, -1.0f, 3.0f, 1, 2);
	Point * c1 = new Point(-4.5f, -2.0f, 3.2f, 2, 3);
	
	Point * c2 = new Point(-4.0f, -2.5f, 2.8f, 3, 3);
	Point * end2 = new Point(-3.5f, -2.0f, 3.0f, 4, 2);
	Point * c3 = new Point(-3.0f, -1.5f, 3.2f, 5, 3);
	
	Point * c4 = new Point(-2.5f, 1.0f, 2.0f, 6, 3);
	Point * end3 = new Point(-2.0f, 3.0f, 1.0f, 7, 2);
	Point * c5 = new Point(-1.5f, 5.0f, 0.0f, 8, 3);
	
	Point * c6 = new Point(-1.0f, 2.0f, 2.0f, 9, 3);
	Point * end4 = new Point(-0.5f, 3.0f, 1.0f, 10, 2);
	Point * c7 = new Point(0.0f, 4.0f, 0.0f, 11, 3);
	
	Point * c8 = new Point(1.0f, 1.0f, 1.0f, 12, 3);
	Point * end5 = new Point(2.0f, -1.0f, 0.0f, 13, 2);
	Point * c9 = new Point(3.0f, -3.0f, -1.0f, 14, 3);

	Point * c10 = new Point(-1.0f, 2.0f, -2.0f, 15, 3);
	Point * end6 = new Point(-1.5f, 3.0f, -2.5f, 16, 2);
	Point * c11 = new Point(-2.0f, 4.0f, -3.0f, 17, 3);

	Point * c12 = new Point(-2.5f, 3.0f, -2.5f, 18, 3);
	Point * end7 = new Point(-3.0f, 2.0f, -2.0f, 19, 2);
	Point * c13 = new Point(-3.5f, 1.0f, -1.5f, 20, 3);

	Point * c14 = new Point(-4.0f, 0.5f, -1.2f, 21, 3);
	Point * end8 = new Point(-4.5f, -0.5f, -1.0f, 22, 2);
	Point * c15 = new Point(-5.0f, -1.5f, -0.8f, 23, 3);

	Point * c16 = new Point(-6.0f, -2.0f, -0.5f, 24, 3);
	Point * end9 = new Point(-6.5f, -3.5f, 0.0f, 25, 2);
	Point * c17 = new Point(-7.0f, -5.0f, 0.5f, 26, 3);

	Point * c18 = new Point(-6.6f, -3.0f, 1.0f, 27, 3);
	Point * end10 = new Point(-6.3f, -2.0f, 1.5f, 28, 2);
	Point * c19 = new Point(-6.0f, -1.0f, 2.0f, 29, 3);


	pointsformatrix.push_back(end1);
	pointsformatrix.push_back(c1);
	pointsformatrix.push_back(c2);
	pointsformatrix.push_back(end2);
	pointsformatrix.push_back(end2);
	pointsformatrix.push_back(c3);
	pointsformatrix.push_back(c4);
	pointsformatrix.push_back(end3);
	pointsformatrix.push_back(end3);
	pointsformatrix.push_back(c5);
	pointsformatrix.push_back(c6);
	pointsformatrix.push_back(end4);
	pointsformatrix.push_back(end4);
	pointsformatrix.push_back(c7);
	pointsformatrix.push_back(c8);
	pointsformatrix.push_back(end5);
	pointsformatrix.push_back(end5);
	pointsformatrix.push_back(c9);
	pointsformatrix.push_back(c10);
	pointsformatrix.push_back(end6);
	pointsformatrix.push_back(end6);
	pointsformatrix.push_back(c11);
	pointsformatrix.push_back(c12);
	pointsformatrix.push_back(end7);
	pointsformatrix.push_back(end7);
	pointsformatrix.push_back(c13);
	pointsformatrix.push_back(c14);
	pointsformatrix.push_back(end8);
	pointsformatrix.push_back(end8);
	pointsformatrix.push_back(c15);
	pointsformatrix.push_back(c16);
	pointsformatrix.push_back(end9);
	pointsformatrix.push_back(end9);
	pointsformatrix.push_back(c17);
	pointsformatrix.push_back(c18);
	pointsformatrix.push_back(end10);
	pointsformatrix.push_back(end10);
	pointsformatrix.push_back(c19);
	pointsformatrix.push_back(c20);
	pointsformatrix.push_back(end1);

	pointstodraw.push_back(c20);
	pointstodraw.push_back(end1);
	pointstodraw.push_back(c1);
	pointstodraw.push_back(c2);
	pointstodraw.push_back(end2);
	pointstodraw.push_back(c3);
	pointstodraw.push_back(c4);
	pointstodraw.push_back(end3);
	pointstodraw.push_back(c5);
	pointstodraw.push_back(c6);
	pointstodraw.push_back(end4);
	pointstodraw.push_back(c7);
	pointstodraw.push_back(c8);
	pointstodraw.push_back(end5);
	pointstodraw.push_back(c9);
	pointstodraw.push_back(c10);
	pointstodraw.push_back(end6);
	pointstodraw.push_back(c11);
	pointstodraw.push_back(c12);
	pointstodraw.push_back(end7);
	pointstodraw.push_back(c13);
	pointstodraw.push_back(c14);
	pointstodraw.push_back(end8);
	pointstodraw.push_back(c15);
	pointstodraw.push_back(c16);
	pointstodraw.push_back(end9);
	pointstodraw.push_back(c17);
	pointstodraw.push_back(c18);
	pointstodraw.push_back(end10);
	pointstodraw.push_back(c19);
}

void Control::setUpCurves() {
	curve1.clear();

	for (int i = 0; i < 10; i++) {
		glm::mat4x3 cpoints = glm::mat4x3(pointsformatrix[4 * i]->location.at(0),
			pointsformatrix[4 * i + 1]->location.at(0),
			pointsformatrix[4 * i + 2]->location.at(0),
			pointsformatrix[4 * i + 3]->location.at(0));

		glm::mat4x3 mult = cpoints * B;

		glm::vec3 result = mult * glm::vec4(pow(0, 3), pow(0, 2), 0, 1.0f);
		curve1.push_back(result);
		if (i == 0) {
			highest = result;
			highestcurve = i;
			highestt = 0;
		}
		else {
			if (result.y > highest.y) {
				highest = result;
				highestcurve = i;
				highestt = 0;
			}
		}

		for (int j = 1; j <= 150; j++) {
			float t = j / 150.0f;

			result = mult * glm::vec4(pow(t, 3), pow(t, 2), t, 1.0f);
			//cout << result.x << " " << result.y << endl;
			curve1.push_back(result);
			if (j != 150) {
				curve1.push_back(result);
			}

			if (result.y > highest.y) {
				highest = result;
				highestcurve = i;
				highestt = t;
			}
		}
	}

	curve1.push_back(pointsformatrix.at(pointsformatrix.size() - 2)->location.at(0));
	curve1.push_back(pointsformatrix.at(pointsformatrix.size() - 1)->location.at(0));

	for (int k = 0; k < pointsformatrix.size() - 2; k++) {
		curve1.push_back(pointsformatrix.at(k)->location.at(0));
	}

	currPoint = highest;
	cout << highest.x << " " << highest.y << " " << highest.z << endl;
}

glm::vec3 Control::calculateNextPoint() {
	float heightdiff = highest.y - currPoint.y;
	cout << heightdiff << endl;
	if (heightdiff < 0) {
		heightdiff = 0.001;
	}
	float velocity = sqrtf(-2 * accel * heightdiff) + 0.01f;
	velocity = velocity * 0.01f;

	if (heightdiff == 0) {
		currcurve = highestcurve;
		velt = highestt;
	}
	cout << "velocity: " << velocity << endl;
	velt = velt + velocity;
	cout << "v + t: " << velt << endl;
	if (velt >= 1) {
		int offset = (int)velt;
		velt = velt - offset;

		currcurve = (currcurve + offset) % 10;
	}
	cout << "t: " << velt << endl;
	cout << "number curve we're on: " << currcurve << endl;
	glm::mat4x3 cpoints = glm::mat4x3(pointsformatrix[4 * currcurve]->location.at(0),
		pointsformatrix[4 * currcurve + 1]->location.at(0),
		pointsformatrix[4 * currcurve + 2]->location.at(0),
		pointsformatrix[4 * currcurve + 3]->location.at(0));

	glm::mat4x3 mult = cpoints * B;

	glm::vec3 nextpoint = mult * glm::vec4(pow(velt, 3), pow(velt, 2), velt, 1.0f);
	currPoint = nextpoint;
	return nextpoint;
}

void Control::reset() {
	currPoint = highest;
}