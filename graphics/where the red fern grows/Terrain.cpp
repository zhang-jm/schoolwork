#include "Terrain.h"
#include "Window.h"

Terrain::Terrain()
{
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	srand(time(NULL));
}

Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBON);
	glDeleteBuffers(1, &EBO);
}

void Terrain::draw(GLuint shaderProgram)
{
	glm::mat4 view = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &view[0][0]);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, points.size());
	glBindVertexArray(0);
}

void Terrain::generateTerrain(int n) {
	size = pow(2, n) + 1; //size = 2^n + 1
	range = ogrange;
	points.clear();
	normals.clear();

	cout << "Generating terrain with size " << size << endl;

	height = new double *[size];
	for (int i = 0; i < size; i++)
	{
		height[i] = new double[size];
		for (int j = 0; j < size; j++)
			height[i][j] = 0.0;
	}

	//initialize corner points to random values
	height[0][0] = 1;
	height[0][size - 1] = 1;
	height[size - 1][0] = 1;
	height[size - 1][size - 1] = 1;

	//run diamond square algorithm
	for (int sidelength = size - 1; sidelength >= 2; sidelength /= 2) {
		int half = sidelength / 2;

		square(sidelength, half);
		diamond(sidelength, half);

		range /= 2; //decrease magnitude of random value
		cout << range << endl;
	}

	//create a grid that isn't a perfect square
	for (int x = 0; x < size; x++) {
		for (int z = 0; z < size; z++) {
			grid.push_back(glm::vec2(x + dRand(-0.3, 0.3), z + dRand(-0.3, 0.3)));
		}
	}

	//draw each square in grid as 2 triangles
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - 1; j++) {
			//get the points for a square in the grid
			glm::vec2 topleft = grid.at((i * size) + j);
			glm::vec2 bottomleft = grid.at((i * size) + j + 1);
			glm::vec2 topright = grid.at(((i + 1) * size) + j);
			glm::vec2 bottomright = grid.at(((i + 1) * size) + j + 1);

			//convert the points to 3D with the randomly generated heights
			glm::vec3 tl = glm::vec3(topleft.x, height[i][j], topleft.y);
			glm::vec3 bl = glm::vec3(bottomleft.x, height[i][j + 1], bottomleft.y);
			glm::vec3 tr = glm::vec3(topright.x, height[i + 1][j], topright.y);
			glm::vec3 br = glm::vec3(bottomright.x, height[i + 1][j + 1], bottomright.y);

			//first triangle of square
			points.push_back(tl); //A
			points.push_back(bl); //B
			points.push_back(tr); //C

			//get normal of first triangle
			glm::vec3 BC = tr - bl; //BC = C - B
			glm::vec3 BA = tl - bl; //BA = A - B
			glm::vec3 norm = glm::normalize(glm::cross(BC, BA)); //normal = BC x BA
			normals.push_back(norm);
			normals.push_back(norm);
			normals.push_back(norm);

			//2nd triangle of square
			points.push_back(tr); //A
			points.push_back(bl); //B
			points.push_back(br); //C

			//get normal of second triangle
			BC = br - bl; //BC = C - B
			BA = tr - bl; //BA = A - B
			norm = glm::normalize(glm::cross(BC, BA)); //normal = BC x BA
			normals.push_back(norm);
			normals.push_back(norm);
			normals.push_back(norm);
		}
	}

	//update the buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBON);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

//diamond step
void Terrain::diamond(int sidelength, int half) {
	for (int x = 0; x < size - 1; x += half) {
		for (int y = (x + half) % sidelength; y < size - 1; y += sidelength) {
			
			double avg = height[(x - half + size - 1) % (size - 1)][y] +
				height[(x + half) % (size - 1)][y] +
				height[x][(y + half) % (size - 1)] +
				height[x][(y - half + size - 1) % (size - 1)];
			avg /= 4.0;
			
			height[x][y] = avg + dRand(-range, range);

			if (x == 0) { 
				height[size - 1][y] = avg; 
			}

			if (y == 0) { 
				height[x][size - 1] = avg; 
			}
		}
	}
}

//square step
void Terrain::square(int sidelength, int half) {
	//perform for each square
	for (int x = 0; x < size - 1; x += sidelength) {
		for (int y = 0; y < size - 1; y += sidelength) {
			
			//get the avg of the square's corner values
			double avg = height[x][y] + height[x + sidelength][y] + height[x][y + sidelength] + height[x + sidelength][y + sidelength];
			avg /= 4.0;

			//middle of square = avg + random val
			height[x + half][y + half] = avg + dRand(-range, range);
		}
	}
}

//taken from https://github.com/ROUKIEN/DiamondSquare/blob/master/src/DiamondSquare.cpp
//generates random double within the range
double Terrain::dRand(double dMin, double dMax)
{
	double d = (double)rand() / RAND_MAX;
	//cout << "D: " << d << "Dmin " << dMin << "dMax " << dMax << endl;
	//cout << "Total: " << dMin + (d * (dMax - dMin)) << endl;
	return dMin + (d * (dMax - dMin));
}
