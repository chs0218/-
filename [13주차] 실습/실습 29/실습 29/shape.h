#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <random>
#include <math.h>

#define BOXSIZE 1.0

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(-800, 800);
std::uniform_int_distribution<int> disY(0, 800);

class Snow {
	GLfloat x, y, z;

public:
	Snow() {
		x = (GLfloat)dis(gen) * 0.01;
		y = (GLfloat)disY(gen) * 0.01;
		z = (GLfloat)dis(gen) * 0.01;
	}

	void print() {
		std::cout << x << ' ' << y << ' ' << z << std::endl;
	}

	void falling() {
		this->y -= 0.05;
		if (this->y < 0.0)
			this->y = 10.0;
	}

	GLfloat getSnowX(){
		return x;
	}

	GLfloat getSnowY() {
		return y;
	}

	GLfloat getSnowZ() {
		return z;
	}
};

class Obstacle {
	GLfloat x, z, dist;
public:
	Obstacle() {
		x = 0.0, z = 0.0, dist = 0.0;
		while (x > -3.0 && x < 3.0)
			x = (GLfloat)dis(gen) * 0.01;
		while (z > -3.0 && z < 3.0)
			z = (GLfloat)dis(gen) * 0.01;
	}

	void reset() {
		x = (GLfloat)dis(gen) * 0.01;
		z = (GLfloat)dis(gen) * 0.01;
		while (x > -3.0 && x < 3.0)
			x = (GLfloat)dis(gen) * 0.01;
		while (z > -3.0 && z < 3.0)
			z = (GLfloat)dis(gen) * 0.01;
	}

	GLfloat getX() {
		return x;
	}

	GLfloat getZ() {
		return z;
	}

	GLfloat getdist() {
		return dist;
	}

	void makedist(glm::vec3 cameraPos) {
		dist = pow((x - cameraPos.x), 2) + pow((0 - cameraPos.y), 2) + pow((z - cameraPos.z), 2);
		std::cout << dist << std::endl;
	}

	bool operator <(Obstacle& obstacle) {
		return this->dist > obstacle.getdist();
	}
};
int SnowNum = 40;

Snow snows[40];
Obstacle obstacle[10];

const double PI = 3.141592;
GLuint SierpinskiVAO, AlphaVAO;
GLuint SierpinskiVBO, AlphaVBO;

GLfloat OrbitDots[360][3];

float shapevertices[288];
float Alphavertices[360];

GLfloat lightX = 0.0f, lightY = 4.0f, lightZ = 8.0f, lightColor[3] = { 1.0f, 1.0f, 1.0f };
GLfloat Lradius = 0.0f, OrbitM = 0.0f, OrbitR = 0.0f, OrbitS = 0.0f, ambientlight = 0.3f;

glm::vec3 cameraPos = glm::vec3(0.0f, 6.0f, 8.0f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

void InitAlpha();
void InitVertices();
void InitDots();
void InitDist();
void Sierpinski(GLfloat V1[], GLfloat V2[], GLfloat V3[], GLfloat Normal[], int n);
void DrawTriangle();

GLfloat Dots[][3] = {
	// 육면체
	{-BOXSIZE, BOXSIZE, BOXSIZE},
	{-BOXSIZE, -BOXSIZE, BOXSIZE},
	{BOXSIZE, -BOXSIZE, BOXSIZE},
	{BOXSIZE, BOXSIZE, BOXSIZE},
	{-BOXSIZE, BOXSIZE, -BOXSIZE},
	{-BOXSIZE, -BOXSIZE, -BOXSIZE},
	{BOXSIZE, -BOXSIZE, -BOXSIZE},
	{BOXSIZE, BOXSIZE, -BOXSIZE},
	{0.0, BOXSIZE, 0.0}
};


GLfloat normal[][3] = {
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0},
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0},
	{0.0, 0.44, 0.88},
	{0.88, 0.44, 0.0},
	{0.0, 0.44, -0.88},
	{-0.88, 0.44, 0.0},
};

unsigned int Shapeindex[] = {
	// 육면체
	0, 1, 2,
	0, 2, 3,
	4, 6, 5,
	4, 7, 6,
	3, 2, 6,
	3, 6, 7,
	4, 5, 1,
	4, 1, 0,
	4, 0, 3,
	4, 3, 7,
	1, 5, 6,
	1, 6, 2,

	8, 1, 2,
	8, 2, 6,
	8, 6, 5,
	8, 5, 1
};

unsigned int normalindex[] = {
	// 육면체
	0, 0, 0,
	0, 0, 0,
	1, 1, 1,
	1, 1, 1,
	2, 2, 2,
	2, 2, 2,
	3, 3, 3,
	3, 3, 3,
	4, 4, 4,
	4, 4, 4,
	5, 5, 5,
	5, 5, 5,
	6, 6, 6,
	7, 7, 7,
	8, 8, 8,
	9, 9, 9,
};


