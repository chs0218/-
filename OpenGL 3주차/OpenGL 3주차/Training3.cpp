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
void SetRectangle();
void RandomColor(int index);
bool IsCollide(int Seed);
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);
GLvoid DrawScene();
Direction dir[3];
Shape shape[4];

struct MyPoint {
	GLfloat Point_X;
	GLfloat Point_Y;
};

GLchar* vertexsource;
GLchar* fragmentsource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint s_program;
GLuint vao, vbo[2];
GLint width, height;

MyPoint myPoint[3] = {
	{0.4, -0.9},
	{0.0, 0.16},
	{0., -0.16}
};

GLfloat triShape[17][3];

GLfloat colors[12][3] = { //--- �ﰢ�� ������ ����
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.78, 0.41 },
	{ 0.95, 0.78, 0.41 },
	{ 0.95, 0.78, 0.41 },
	{ 0.91, 0.50, 0.21 },
	{ 0.91, 0.50, 0.21 },
	{ 0.91, 0.50, 0.21 }};

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

bool IsCollide(int Seed)
{
	switch (Seed)
	{
	case 0:
		if (CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[0][0], triShape[0][1]) *
			CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[1][0], triShape[0][1]) <= 0 &&
			CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[0][1], -BoxWidth, -BoxHeight) *
			CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[0][1], BoxWidth, -BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[0][0], triShape[0][1]) * CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[1][0], triShape[1][1]) == 0 &&
				CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[1][1], -BoxWidth, -BoxHeight) * CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[1][1], BoxWidth, -BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxWidth, BoxWidth, triShape[0][0], triShape[1][0]))
					return true;
			}
			else
				return true;
		}

		if (CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[1][0], triShape[1][1]) *
			CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[2][0], triShape[2][1]) <= 0 &&
			CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], -BoxWidth, -BoxHeight) *
			CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], BoxWidth, -BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[1][0], triShape[1][1]) * CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[2][0], triShape[2][1]) == 0 &&
				CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], -BoxWidth, -BoxHeight) * CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], BoxWidth, -BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxWidth, BoxWidth, triShape[1][0], triShape[2][0]))
					return true;
			}
			else
				return true;
		}

		if (CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[0][0], triShape[0][1]) *
			CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[2][0], triShape[2][1]) <= 0 &&
			CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], -BoxWidth, -BoxHeight) *
			CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], BoxWidth, -BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[0][0], triShape[0][1]) * CCW(-BoxWidth, -BoxHeight, BoxWidth, -BoxHeight, triShape[2][0], triShape[2][1]) == 0 &&
				CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], -BoxWidth, -BoxHeight) * CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], BoxWidth, -BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxWidth, BoxWidth, triShape[0][0], triShape[2][0]))
					return true;
			}
			else
				return true;
		}
		break;
	case 1:
		if (CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[0][0], triShape[0][1]) *
			CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[1][0], triShape[0][1]) <= 0 &&
			CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[0][1], BoxWidth, -BoxHeight) *
			CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[0][1], BoxWidth, BoxHeight) <= 0)
		{
			if (CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[0][0], triShape[0][1]) * CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[1][0], triShape[1][1]) == 0 &&
				CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[1][1], BoxWidth, -BoxHeight) * CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[1][1], BoxWidth, BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxHeight, BoxHeight, triShape[0][1], triShape[1][1]))
					return true;
			}
			else
				return true;
		}

		if (CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[1][0], triShape[1][1]) *
			CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[2][0], triShape[2][1]) <= 0 &&
			CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], BoxWidth, -BoxHeight) *
			CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], BoxWidth, BoxHeight) <= 0)
		{
			if (CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[1][0], triShape[1][1]) * CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[2][0], triShape[2][1]) == 0 &&
				CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], BoxWidth, -BoxHeight) * CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], BoxWidth, BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxHeight, BoxHeight, triShape[1][1], triShape[2][1]))
					return true;
			}
			else
				return true;
		}

		if (CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[0][0], triShape[0][1]) *
			CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[2][0], triShape[2][1]) <= 0 &&
			CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], BoxWidth, -BoxHeight) *
			CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], BoxWidth, BoxHeight) <= 0)
		{
			if (CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[0][0], triShape[0][1]) * CCW(BoxWidth, -BoxHeight, BoxWidth, BoxHeight, triShape[2][0], triShape[2][1]) == 0 &&
				CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], BoxWidth, -BoxHeight) * CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], BoxWidth, BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxHeight, BoxHeight, triShape[0][1], triShape[2][1]))
					return true;
			}
			else
				return true;
		}
		break;
	case 2:
		if (CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[0][0], triShape[0][1]) *
			CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[1][0], triShape[0][1]) <= 0 &&
			CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[0][1], -BoxWidth, BoxHeight) *
			CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[0][1], BoxWidth, BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[0][0], triShape[0][1]) * CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[1][0], triShape[1][1]) == 0 &&
				CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[1][1], -BoxWidth, BoxHeight) * CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[1][1], BoxWidth, BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxWidth, BoxWidth, triShape[0][0], triShape[1][0]))
					return true;
			}
			else
				return true;
		}

		if (CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[1][0], triShape[1][1]) *
			CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[2][0], triShape[2][1]) <= 0 &&
			CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], -BoxWidth, BoxHeight) *
			CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], BoxWidth, BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[1][0], triShape[1][1]) * CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[2][0], triShape[2][1]) == 0 &&
				CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], -BoxWidth, BoxHeight) * CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], BoxWidth, BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxWidth, BoxWidth, triShape[1][0], triShape[2][0]))
					return true;
			}
			else
				return true;
		}

		if (CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[0][0], triShape[0][1]) *
			CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[2][0], triShape[2][1]) <= 0 &&
			CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], -BoxWidth, BoxHeight) *
			CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], BoxWidth, BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[0][0], triShape[0][1]) * CCW(-BoxWidth, BoxHeight, BoxWidth, BoxHeight, triShape[2][0], triShape[2][1]) == 0 &&
				CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], -BoxWidth, BoxHeight) * CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], BoxWidth, BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxWidth, BoxWidth, triShape[0][0], triShape[2][0]))
					return true;
			}
			else
				return true;
		}
		break;
	case 3:
		if (CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[0][0], triShape[0][1]) *
			CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[1][0], triShape[0][1]) <= 0 &&
			CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[0][1], -BoxWidth, BoxHeight) *
			CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[0][1], -BoxWidth, -BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[0][0], triShape[0][1]) * CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[1][0], triShape[1][1]) == 0 &&
				CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[1][1], -BoxWidth, BoxHeight) * CCW(triShape[0][0], triShape[0][1], triShape[1][0], triShape[1][1], -BoxWidth, -BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxHeight, BoxHeight, triShape[0][1], triShape[1][1]))
					return true;
			}
			else
				return true;
		}

		if (CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[1][0], triShape[1][1]) *
			CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[2][0], triShape[2][1]) <= 0 &&
			CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], -BoxWidth, BoxHeight) *
			CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], -BoxWidth, -BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[1][0], triShape[1][1]) * CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[2][0], triShape[2][1]) == 0 &&
				CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], -BoxWidth, BoxHeight) * CCW(triShape[1][0], triShape[1][1], triShape[2][0], triShape[2][1], -BoxWidth, -BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxHeight, BoxHeight, triShape[1][1], triShape[2][1]))
					return true;
			}
			else
				return true;
		}

		if (CCW(-BoxWidth, BoxHeight, -BoxWidth, 0-.24f, triShape[0][0], triShape[0][1]) *
			CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[2][0], triShape[2][1]) <= 0 &&
			CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], -BoxWidth, BoxHeight) *
			CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], -BoxWidth, -BoxHeight) <= 0)
		{
			if (CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[0][0], triShape[0][1]) * CCW(-BoxWidth, BoxHeight, -BoxWidth, -BoxHeight, triShape[2][0], triShape[2][1]) == 0 &&
				CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], -BoxWidth, BoxHeight) * CCW(triShape[0][0], triShape[0][1], triShape[2][0], triShape[2][1], -BoxWidth, -BoxHeight) == 0)
			{
				if (ParallelCollide(-BoxHeight, BoxHeight, triShape[0][1], triShape[2][1]))
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
	for (int i = 0; i < 3; ++i)
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
	triShape[9][0] = -BoxWidth;
	triShape[9][1] = BoxHeight;
	triShape[9][2] = 0;
	triShape[10][0] = -BoxWidth;
	triShape[10][1] = -BoxHeight;
	triShape[10][2] = 0;
	triShape[11][0] = -BoxWidth;
	triShape[11][1] = -BoxHeight;
	triShape[11][2] = 0;
	triShape[12][0] = BoxWidth;
	triShape[12][1] = -BoxHeight;
	triShape[12][2] = 0;
	triShape[13][0] = BoxWidth;
	triShape[13][1] = -BoxHeight;
	triShape[13][2] = 0;
	triShape[14][0] = BoxWidth;
	triShape[14][1] = BoxHeight;
	triShape[14][2] = 0;
	triShape[15][0] = BoxWidth;
	triShape[15][1] = BoxHeight;
	triShape[15][2] = 0;
	triShape[16][0] = -BoxWidth;
	triShape[16][1] = BoxHeight;
	triShape[16][2] = 0;
}
void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, NULL);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
	}
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, NULL);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
	}
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(triShape), triShape, GL_DYNAMIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	//--- colors �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����

	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

void DrawScene() //--- glutDisplayFunc()�Լ��� ����� �׸��� �ݹ� �Լ�
{
	InitTriangle();
	InitBuffer();
	//--- ����� ���� ����
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(s_program);
	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao);
	//--- �ﰢ�� �׸���
	glDrawArrays(GL_TRIANGLES, 0, 9);
	glDrawArrays(GL_LINES, 9, 8);
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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
	for (int i = 0; i < 3; ++i)
	{
		switch (dir[i])
		{
		case Direction::LeftUp:
			myPoint[i].Point_X -= 0.05f;
			myPoint[i].Point_Y += 0.05f;
			
			if (IsCollide(0))
			{
				myPoint[i].Point_Y -= 0.1f;
				dir[i] = Direction::LeftDown;
				--shape[i];
			}

			if (IsCollide(1))
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

			if (IsCollide(1))
			{
				myPoint[i].Point_X += 0.1f;
				dir[i] = Direction::RightDown;
				--shape[i];
			}

			if (IsCollide(2))
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

			if (IsCollide(0))
			{
				myPoint[i].Point_Y -= 0.1f;
				dir[i] = Direction::RightDown;
				++shape[i];
			}

			if (IsCollide(3))
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

			if (IsCollide(2))
			{
				myPoint[i].Point_Y += 0.1f;
				dir[i] = Direction::RightUp;
				--shape[i];
			}

			if (IsCollide(3))
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
	glutTimerFunc(50, TimerFunction, 1); // Ÿ�̸��Լ� �� ����
}

void main(int argc, char** argv)	//---������ ����ϰ� �ݹ��Լ� ����
{
	width = 900;
	height = 900;

	dir[0] = Direction::LeftUp;
	shape[0] = Shape::ToTop;
	dir[1] = Direction::Left;
	shape[1] = Shape::ToBottom;
	dir[2] = Direction::Right;
	shape[2] = Shape::ToTop;
	SetRectangle();
	InitTriangle();

	//---������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training3");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(50, TimerFunction, 1);
	glutMainLoop();
}











