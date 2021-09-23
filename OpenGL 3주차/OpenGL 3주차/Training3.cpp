#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#include <fstream>
#include <random>

#define BoxWidth 0.59999
#define BoxHeight 0.25999

enum class Direction: int{
	LeftUp = 0,
	RightUp,
	RightDown,
	LeftDown,
	Left,
	Right
};

enum class Shape: int{
	ToLeft = 0,
	ToTop,
	ToRight,
	ToBottom
};

Shape operator++(Shape& shape) {
	shape = static_cast<Shape>((static_cast<int>(shape) + 1) % 4);
	return shape;
}

Shape operator--(Shape& shape) {
	shape = static_cast<Shape>((static_cast<int>(shape) + 3) % 4);
	return shape;
}

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
void InitTriangle();
void TimerFunction(int value);
void Mouse(int button, int state, int x, int y);
void SetRectangle();
void RandomColor(int index);
bool IsCollide(int Seed, int Index);
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);
GLvoid DrawScene();
Direction dir[6];
Shape shape[6];

struct MyPoint {
	GLfloat Point_X;
	GLfloat Point_Y;
};

GLchar* vertexsource;
GLchar* fragmentsource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint s_program;
GLuint vao, vbo[2];
GLint width, height;

MyPoint myPoint[6] = {
	{0.4, -0.9},
	{0.9, 0},
	{0.6, 0.9},
	{-0.9, 0},
	{0.0, 0.16},
	{0., -0.16}
};

GLfloat triShape[26][3];

GLfloat colors[18][3] = { //--- 삼각형 꼭지점 색상
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.78, 0.41 },
	{ 0.95, 0.78, 0.41 },
	{ 0.95, 0.78, 0.41 },
	{ 0.91, 0.50, 0.21 },
	{ 0.91, 0.50, 0.21 },
	{ 0.91, 0.50, 0.21 },
	{ 0.53, 0.51, 0.64 },
	{ 0.53, 0.51, 0.64 },
	{ 0.53, 0.51, 0.64 },
	{ 0.47, 0.60, 0.50 },
	{ 0.47, 0.60, 0.50 },
	{ 0.47, 0.60, 0.50 },
	{ 0.33, 0.78, 0.82 },
	{ 0.33, 0.78, 0.82 },
	{ 0.33, 0.78, 0.82 } };

int iIndex = 0;

int CCW(const GLfloat ax, const GLfloat ay, const GLfloat bx, const GLfloat by, const GLfloat cx, const GLfloat cy)
{
	GLfloat iAns = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
	if (iAns < 0) return 1;
	else if (iAns > 0) return -1;
	else return 0;
}

bool ParallelCollide(GLfloat p1, GLfloat p2, GLfloat p3, GLfloat p4)
{
	GLfloat tmp;

	if (p1 > p2)
	{
		tmp = p1;
		p1 = p2;
		p2 = tmp;
	}

	if (p3 > p4)
	{
		tmp = p3;
		p3 = p4;
		p4 = tmp;
	}

	if (p2 >= p3 && p1 <= p4)
		return true;
	else
		return false;
}

bool IsCollide(int Seed, int Index)
{
	switch (Seed)
	{
	case 0:
		if (CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) *
			CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index][1]) <= 0 &&
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index][1], -BoxWidth, -BoxHeight) *
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index][1], BoxWidth, -BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) * CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index + 1][1]) == 0 &&
				CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], -BoxWidth, -BoxHeight) * CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], BoxWidth, -BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxWidth, BoxWidth, triShape[3 * Index][0], triShape[3 * Index + 1][0]))
					return true;
			}
			else
				return true;
		}

		if (CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index + 1][1]) *
			CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) <= 0 &&
			CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, -BoxHeight) *
			CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, -BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index + 1][1]) * CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) == 0 &&
				CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, -BoxHeight) * CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, -BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxWidth, BoxWidth, triShape[3 * Index + 1][0], triShape[3 * Index + 2][0]))
					return true;
			}
			else
				return true;
		}

		if (CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) *
			CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) <= 0 &&
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, -BoxHeight) *
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, -BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) * CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) == 0 &&
				CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, -BoxHeight) * CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, -BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxWidth, BoxWidth, triShape[3 * Index][0], triShape[3 * Index + 2][0]))
					return true;
			}
			else
				return true;
		}
		break;
	case 1:
		if (CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) *
			CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index][1]) <= 0 &&
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index][1], BoxWidth, -BoxHeight) *
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index][1], BoxWidth, BoxHeight) <= 0)
		{
			if (CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) * CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index + 1][1]) == 0 &&
				CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], BoxWidth, -BoxHeight) * CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], BoxWidth, BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxHeight, BoxHeight, triShape[3 * Index][1], triShape[3 * Index + 1][1]))
					return true;
			}
			else
				return true;
		}

		if (CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index + 1][1]) *
			CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) <= 0 &&
			CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, -BoxHeight) *
			CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, BoxHeight) <= 0)
		{
			if (CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index + 1][1]) * CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) == 0 &&
				CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, -BoxHeight) * CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxHeight, BoxHeight, triShape[3 * Index + 1][1], triShape[3 * Index + 2][1]))
					return true;
			}
			else
				return true;
		}

		if (CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) *
			CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) <= 0 &&
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, -BoxHeight) *
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, BoxHeight) <= 0)
		{
			if (CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) * CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) == 0 &&
				CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, -BoxHeight) * CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxHeight, BoxHeight, triShape[3 * Index][1], triShape[3 * Index + 2][1]))
					return true;
			}
			else
				return true;
		}
		break;
	case 2:
		if (CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) *
			CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index][1]) <= 0 &&
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index][1], -BoxWidth, BoxHeight) *
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index][1], BoxWidth, BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) * CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index + 1][1]) == 0 &&
				CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], -BoxWidth, BoxHeight) * CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], BoxWidth, BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxWidth, BoxWidth, triShape[3 * Index][0], triShape[3 * Index + 1][0]))
					return true;
			}
			else
				return true;
		}

		if (CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index + 1][1]) *
			CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) <= 0 &&
			CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, BoxHeight) *
			CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index + 1][1]) * CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) == 0 &&
				CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, BoxHeight) * CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxWidth, BoxWidth, triShape[3 * Index + 1][0], triShape[3 * Index + 2][0]))
					return true;
			}
			else
				return true;
		}

		if (CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) *
			CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) <= 0 &&
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, BoxHeight) *
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) * CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) == 0 &&
				CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, BoxHeight) * CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], BoxWidth, BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxWidth, BoxWidth, triShape[3 * Index][0], triShape[3 * Index + 2][0]))
					return true;
			}
			else
				return true;
		}
		break;
	case 3:
		if (CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) *
			CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index][1]) <= 0 &&
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index][1], -BoxWidth, BoxHeight) *
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index][1], -BoxWidth, -BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) * CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index + 1][1]) == 0 &&
				CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], -BoxWidth, BoxHeight) * CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], -BoxWidth, -BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxHeight, BoxHeight, triShape[3 * Index][1], triShape[3 * Index + 1][1]))
					return true;
			}
			else
				return true;
		}

		if (CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index + 1][1]) *
			CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) <= 0 &&
			CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, BoxHeight) *
			CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, -BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[3 * Index + 1][0], triShape[3 * Index + 1][1]) * CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) == 0 &&
				CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, BoxHeight) * CCW(triShape[3 * Index + 1][0], triShape[3 * Index + 1][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, -BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxHeight, BoxHeight, triShape[3 * Index + 1][1], triShape[3 * Index + 2][1]))
					return true;
			}
			else
				return true;
		}

		if (CCW(-BoxWidth, BoxHeight, -BoxWidth, 0-.24f, triShape[3 * Index][0], triShape[3 * Index][1]) *
			CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) <= 0 &&
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, BoxHeight) *
			CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, -BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[3 * Index][0], triShape[3 * Index][1]) * CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[3 * Index + 2][0], triShape[3 * Index + 2][1]) == 0 &&
				CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, BoxHeight) * CCW(triShape[3 * Index][0], triShape[3 * Index][1], triShape[3 * Index + 2][0], triShape[3 * Index + 2][1], -BoxWidth, -BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxHeight, BoxHeight, triShape[3 * Index][1], triShape[3 * Index + 2][1]))
					return true;
			}
			else
				return true;
		}
		break;
	}
	// CCW(A,B,C)*CCW(A,B,D) <=0 && CCW(C,D,A)*CCW(C,D,B) <=0
	

	return false;
}

void InitTriangle()
{
	for (int i = 0; i < 6; ++i)
	{
		switch (shape[i])
		{
		case Shape::ToLeft:
			triShape[3 * i][0] = myPoint[i].Point_X - 0.1f;
			triShape[3 * i][1] = myPoint[i].Point_Y;
			triShape[3 * i + 1][0] = myPoint[i].Point_X + 0.1f;
			triShape[3 * i + 1][1] = myPoint[i].Point_Y - 0.1f;
			triShape[3 * i + 2][0] = myPoint[i].Point_X + 0.1f;
			triShape[3 * i + 2][1] = myPoint[i].Point_Y + 0.1f;
			break;
		case Shape::ToRight:
			triShape[3 * i][0] = myPoint[i].Point_X + 0.1f;
			triShape[3 * i][1] = myPoint[i].Point_Y;
			triShape[3 * i + 1][0] = myPoint[i].Point_X - 0.1f;
			triShape[3 * i + 1][1] = myPoint[i].Point_Y + 0.1f;
			triShape[3 * i + 2][0] = myPoint[i].Point_X - 0.1f;
			triShape[3 * i + 2][1] = myPoint[i].Point_Y - 0.1f;
			break;
		case Shape::ToTop:
			triShape[3 * i][0] = myPoint[i].Point_X - 0.1f;
			triShape[3 * i][1] = myPoint[i].Point_Y - 0.1f;
			triShape[3 * i + 1][0] = myPoint[i].Point_X + 0.1f;
			triShape[3 * i + 1][1] = myPoint[i].Point_Y - 0.1f;
			triShape[3 * i + 2][0] = myPoint[i].Point_X;
			triShape[3 * i + 2][1] = myPoint[i].Point_Y + 0.1f;
			break;
		case Shape::ToBottom:
			triShape[3 * i][0] = myPoint[i].Point_X;
			triShape[3 * i][1] = myPoint[i].Point_Y - 0.1f;
			triShape[3 * i + 1][0] = myPoint[i].Point_X + 0.1f;
			triShape[3 * i + 1][1] = myPoint[i].Point_Y + 0.1f;
			triShape[3 * i + 2][0] = myPoint[i].Point_X - 0.1f;
			triShape[3 * i + 2][1] = myPoint[i].Point_Y + 0.1f;
			break;
		default:

			break;
		}
	}
}

void SetRectangle()
{
	triShape[18][0] = -BoxWidth;
	triShape[18][1] = BoxHeight;
	triShape[18][2] = 0;
	triShape[19][0] = -BoxWidth;
	triShape[19][1] = -BoxHeight;
	triShape[19][2] = 0;
	triShape[20][0] = -BoxWidth;
	triShape[20][1] = -BoxHeight;
	triShape[20][2] = 0;
	triShape[21][0] = BoxWidth;
	triShape[21][1] = -BoxHeight;
	triShape[21][2] = 0;
	triShape[22][0] = BoxWidth;
	triShape[22][1] = -BoxHeight;
	triShape[22][2] = 0;
	triShape[23][0] = BoxWidth;
	triShape[23][1] = BoxHeight;
	triShape[23][2] = 0;
	triShape[24][0] = BoxWidth;
	triShape[24][1] = BoxHeight;
	triShape[24][2] = 0;
	triShape[25][0] = -BoxWidth;
	triShape[25][1] = BoxHeight;
	triShape[25][2] = 0;
}
void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, NULL);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
	}
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, NULL);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
	}
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(triShape), triShape, GL_DYNAMIC_DRAW);
	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//--- colors 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기

	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(s_program);
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	InitTriangle();
	InitBuffer();
	//--- 변경된 배경색 설정
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(s_program);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);
	//--- 삼각형 그리기
	glDrawArrays(GL_TRIANGLES, 0, 18);
	glDrawArrays(GL_LINES, 18, 8);
	glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

char* filetobuf(const char* file)
{
	std::ifstream TextFile;
	int size;
	char* str = NULL;
	TextFile.open(file);

	if (TextFile.is_open())
	{
		TextFile.seekg(0, std::ios::end);
		size = TextFile.tellg();
		str = new char[size];
		for (int i = 0; i < size; ++i)
			str[i] = '\0';
		TextFile.seekg(0, std::ios::beg);
		TextFile.read(str, size);
		TextFile.close();
		return str;
	}
	else
		return NULL;
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (x >= 180 && x <=720 && y >=315 && y <= 585)
		{
			
		}

		else
		{
			myPoint[iIndex].Point_X = ((GLfloat)x / (GLfloat)width - 0.5f) * 2.0f;
			myPoint[iIndex].Point_Y = ((GLfloat)y / (GLfloat)height - 0.5f) * -2.0f;
			iIndex += 1;
			if (iIndex == 4)
				iIndex = 0;
		}
	}
	glutPostRedisplay();
}

void RandomColor(int index)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 99);

	for (int i = 0; i < 3; ++i)
	{
		colors[index * 3][0] = colors[index * 3 + 1][0] = colors[index * 3 + 2][0] = GLfloat(dis(gen)) * 0.01f;
		colors[index * 3][1] = colors[index * 3 + 1][1] = colors[index * 3 + 2][1] = GLfloat(dis(gen)) * 0.01f;
		colors[index * 3][2] = colors[index * 3 + 1][2] = colors[index * 3 + 2][2] = GLfloat(dis(gen)) * 0.01f;
	}
}
void TimerFunction(int value)
{
	int tmp = -1;
	for (int i = 0; i < 6; ++i)
	{
		switch (dir[i])
		{
		case Direction::LeftUp:
			myPoint[i].Point_X -= 0.05f;
			myPoint[i].Point_Y += 0.05f;
			
			if (IsCollide(0, i))
			{
				myPoint[i].Point_Y -= 0.1f;
				dir[i] = Direction::LeftDown;
				--shape[i];
			}

			if (IsCollide(1, i))
			{
				myPoint[i].Point_X += 0.1f;
				dir[i] = Direction::RightUp;
				++shape[i];
			}

			if (myPoint[i].Point_X - 0.1f < -1.0f)
			{
				dir[i] = Direction::RightUp;
				++shape[i];
			}
				
			if (myPoint[i].Point_Y + 0.1f > 1.0f)
			{
				dir[i] = Direction::LeftDown;
				--shape[i];
			}
			break;
		case Direction::LeftDown:
			myPoint[i].Point_X -= 0.05f;
			myPoint[i].Point_Y -= 0.05f;

			if (IsCollide(1, i))
			{
				myPoint[i].Point_X += 0.1f;
				dir[i] = Direction::RightDown;
				--shape[i];
			}

			if (IsCollide(2, i))
			{
				myPoint[i].Point_Y += 0.1f;
				dir[i] = Direction::LeftUp;
				++shape[i];
			}

			if (myPoint[i].Point_X - 0.1f < -1.0f)
			{
				dir[i] = Direction::RightDown;
				--shape[i];
			}
				
			if (myPoint[i].Point_Y - 0.1f < -1.0f)
			{
				dir[i] = Direction::LeftUp;
				++shape[i];
			}
			break;
		case Direction::RightUp:
			myPoint[i].Point_X += 0.05f;
			myPoint[i].Point_Y += 0.05f;

			if (IsCollide(0, i))
			{
				myPoint[i].Point_Y -= 0.1f;
				dir[i] = Direction::RightDown;
				++shape[i];
			}

			if (IsCollide(3, i))
			{
				myPoint[i].Point_X -= 0.1f;
				dir[i] = Direction::LeftUp;
				--shape[i];
			}

			if (myPoint[i].Point_X + 0.1f > 1.0f)
			{
				dir[i] = Direction::LeftUp;
				--shape[i];
			}
			if (myPoint[i].Point_Y + 0.1f > 1.0f)
			{
				dir[i] = Direction::RightDown;
				++shape[i];
			}
			break;
		case Direction::RightDown:
			myPoint[i].Point_X += 0.05f;
			myPoint[i].Point_Y -= 0.05f;

			if (IsCollide(2, i))
			{
				myPoint[i].Point_Y += 0.1f;
				dir[i] = Direction::RightUp;
				--shape[i];
			}

			if (IsCollide(3, i))
			{
				myPoint[i].Point_X -= 0.1f;
				dir[i] = Direction::LeftDown;
				++shape[i];
			}

			if (myPoint[i].Point_X + 0.1f > 1.0f)
			{
				dir[i] = Direction::LeftDown;
				++shape[i];
			}

			if (myPoint[i].Point_Y - 0.1f < -1.0f)
			{
				dir[i] = Direction::RightUp;
				--shape[i];
			}
			break;
		case Direction::Left:
			myPoint[i].Point_X -= 0.05f;
			if (myPoint[i].Point_X - 0.1f < -0.5999f)
			{
				dir[i] = Direction::Right;
				RandomColor(i);
			}
			break;
		case Direction::Right:
			myPoint[i].Point_X += 0.05f;
			if (myPoint[i].Point_X + 0.1f > 0.5999f)
			{
				dir[i] = Direction::Left;
				RandomColor(i);
			}
			break;
		default:
			break;
		}

	}
	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction, 1); // 타이머함수 재 설정
}

void main(int argc, char** argv)	//---윈도우 출력하고 콜백함수 설정
{
	width = 900;
	height = 900;

	dir[0] = Direction::LeftUp;
	shape[0] = Shape::ToTop;
	dir[1] = Direction::RightUp;
	shape[1] = Shape::ToLeft;
	dir[2] = Direction::LeftDown;
	shape[2] = Shape::ToBottom;
	dir[3] = Direction::RightDown;
	shape[3] = Shape::ToRight;
	dir[4] = Direction::Left;
	shape[4] = Shape::ToBottom;
	dir[5] = Direction::Right;
	shape[5] = Shape::ToTop;
	SetRectangle();
	InitTriangle();

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training3");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutMouseFunc(Mouse);
	glutReshapeFunc(Reshape);
	glutTimerFunc(50, TimerFunction, 1);
	glutMainLoop();
}











