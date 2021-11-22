#pragma once
#include <gl/glew.h>
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define BOXSIZE 1.0
const double PI = 3.141592;

GLfloat OrbitDots[360][3];
float shapevertices[288];
void InitVertices();
void InitDots();

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