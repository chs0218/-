#ifndef _function
#define _function

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>
#include "Robot.h"
#include "Robot.cpp"
#include "RT_Obstacle.h"

#define BOXSIZE 1.0

float shapevertices[216];

GLfloat Dots[][3] = {
	// ¿∞∏È√º
	{-BOXSIZE, BOXSIZE, BOXSIZE},
	{-BOXSIZE, -BOXSIZE, BOXSIZE},
	{BOXSIZE, -BOXSIZE, BOXSIZE},
	{BOXSIZE, BOXSIZE, BOXSIZE},
	{-BOXSIZE, BOXSIZE, -BOXSIZE},
	{-BOXSIZE, -BOXSIZE, -BOXSIZE},
	{BOXSIZE, -BOXSIZE, -BOXSIZE},
	{BOXSIZE, BOXSIZE, -BOXSIZE}
};


GLfloat normal[][3] = {
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0},
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0}
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
	1, 6, 2
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
	5, 5, 5
};

GLuint g_window_w = 1400;
GLuint g_window_h = 900;

GLuint ShapeVAO;
GLuint ShapeVBO;

GLfloat cameraX = 0.0, cameraY = 3.0, cameraZ = 8.0;

bool RotateY = false, RotateMinus = false;
bool AutoMove = false, CommandT = true, CommandN = false;

void InitBuffer();
void Display();
void Reshape(int w, int h);
void TimerFunc(int value);
void Keyboard(unsigned char key, int x, int y);
void RotateCameraCenterY();
#endif