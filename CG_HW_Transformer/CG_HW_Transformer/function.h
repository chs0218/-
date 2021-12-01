#ifndef _function
#define _function

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

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

GLfloat RandomX[2], RandomZ[2];
GLfloat cameraX = 0.0, cameraY = 3.0, cameraZ = 8.0, Fradius = 0.0f, armlegR = 0.0;
GLfloat transX = 0.0, transY = 2.0, transZ = 0.0;
GLfloat Speed = 0.05;

bool OpenF = false, CloseF = false, armlegPlus = true, modelMove = false, jumpUp = false, jumpDown = true, RotateY = false;
int dir = 0;

void InitBuffer();
void DrawMain();
void Display();
void Reshape(int w, int h);
void TimerFunc(int value);
void Keyboard(unsigned char key, int x, int y);
void RandomObjects();
void RotateCameraCenterY();
bool CheckCollision();

#endif