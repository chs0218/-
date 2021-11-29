#pragma once
#include <gl/glew.h>
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#define BOXSIZE 1.0

#include "stb_image.h"


float shapevertices[254];
void InitVertices();
void InitTexture();
GLuint vao[2], vbo[2];
GLuint s_program[2];
unsigned int texture[6];

GLfloat Dots[][3] = {
	// ����ü
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

GLfloat textureST[][2] = {
	{0.0, 0.0},
	{1.0, 0.0},
	{0.0, 1.0},
	{1.0, 1.0},
	{0.5, 1.0}
};

unsigned int Shapeindex[] = {
	// ����ü
	0, 1, 2,
	0, 2, 3,
	7, 6, 5,
	7, 5, 4,
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

unsigned int textureindex[] = {
	// ����ü
	2, 0, 1,
	2, 1, 3
};

float Lines[] = {
	1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
	0.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0,
	0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
	0.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0
};