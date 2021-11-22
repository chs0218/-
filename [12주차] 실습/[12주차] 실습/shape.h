#pragma once
#include <gl/glew.h>
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

#define BOXSIZE 1.0

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(-1000, 1000);
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

int SnowNum = 40;
Snow snows[40];

const double PI = 3.141592;
GLuint SierpinskiVAO;
GLuint SierpinskiVBO;

GLfloat OrbitDots[360][3];
float shapevertices[288];
void InitVertices();
void InitDots();
void Sierpinski(GLfloat V1[], GLfloat V2[], GLfloat V3[], GLfloat Normal[], int n);
void DrawTriangle();

GLfloat Dots[][3] = {
	// ¿∞∏È√º
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
	// ¿∞∏È√º
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
	// ¿∞∏È√º
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


