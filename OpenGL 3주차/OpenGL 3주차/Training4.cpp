#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#include <fstream>

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
void InitTriangle();
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);
GLvoid DrawScene();

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader, fragmentShader;
GLuint s_program;
GLuint vao, vbo[2];
GLint width, height;

void Moving(int i);
GLint iTriangle = 0;
int iSpeed = 100;
bool bMove = false;
bool bHorizonVec[6];
bool bVerticalVec[6];
bool bCommandA = false;

struct MyPoint {
	GLfloat Point_X;
	GLfloat Point_Y;
};

MyPoint myPoint[6] = {
	{-0.8, 0.8},
	{-0.5, 0.8},
	{-0.2, 0.8},
	{0.1, 0.8},
	{0.4, 0.8},
	{0.7, 0.8}
};

GLfloat triShape[18][3];

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


void main(int argc, char** argv)	//---윈도우 출력하고 콜백함수 설정
{
	width = 900;
	height = 900;

	InitTriangle();

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training4");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);
	glutTimerFunc(50, TimerFunction, 1);
	glutMainLoop();
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

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader);

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
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader);

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
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triShape), triShape, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShaders();
	make_fragmentShaders();

	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(s_program);
}

void InitTriangle()
{
	for (int i = 0; i < iTriangle; ++i)
	{
		triShape[3 * i][0] = myPoint[i].Point_X;
		triShape[3 * i][1] = myPoint[i].Point_Y + 0.1f;
		triShape[3 * i + 1][0] = myPoint[i].Point_X - 0.1f;
		triShape[3 * i + 1][1] = myPoint[i].Point_Y - 0.1f;
		triShape[3 * i + 2][0] = myPoint[i].Point_X + 0.1f;
		triShape[3 * i + 2][1] = myPoint[i].Point_Y - 0.1f;
	}
}

void DrawScene()
{
	InitTriangle();
	InitBuffer();
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !bMove)
	{
		if (iTriangle < 5)
		{
			bHorizonVec[iTriangle] = false;
			bVerticalVec[iTriangle] = false;
			++iTriangle;
			bHorizonVec[iTriangle] = false;
			bVerticalVec[iTriangle] = false;
			++iTriangle;
		}
	}
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'M':
	case 'm':
		bMove = true;
		break;
	case 'N':
	case 'n':
		for (int i = 0; i < iTriangle; ++i)
		{
			if (bHorizonVec[i])
				bHorizonVec[i] = false;
			else
				bHorizonVec[i] = true;

			if (bVerticalVec[i])
				bVerticalVec[i] = false;
			else
				bVerticalVec[i] = true;
		}
		break;
	case '+':
		if(iSpeed > 20)
			iSpeed -= 10;
		break;
	case '-':
		if (iSpeed < 200)
			iSpeed += 10;
		break;
	case 'A':
	case 'a':
		if(bMove)
			bCommandA = true;
		break;
	case 'B':
	case 'b':
		if (bCommandA)
		{
			for (int i = 0; i < iTriangle; ++i)
			{
				if (i % 2 == 0)
				{
					Moving(i);
					Moving(i);
				}
			}
			bCommandA = false;
		}
		break;
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void Moving(int i)
{
	if (bHorizonVec[i])
	{
		myPoint[i].Point_X -= 0.1f;
		if (myPoint[i].Point_X <= -1.0f)
		{
			bHorizonVec[i] = false;
			myPoint[i].Point_X += 0.1f;
			if (bVerticalVec[i])
			{
				myPoint[i].Point_Y += 0.1f;
				if (myPoint[i].Point_Y >= 1.0f)
				{
					bVerticalVec[i] = false;
					myPoint[i].Point_Y -= 0.1f;
				}
			}
			else
			{
				myPoint[i].Point_Y -= 0.1f;
				if (myPoint[i].Point_Y <= -1.0f)
				{
					bVerticalVec[i] = true;
					myPoint[i].Point_Y += 0.1f;
				}
			}

		}
	}
	else
	{
		myPoint[i].Point_X += 0.1f;
		if (myPoint[i].Point_X >= 1.0f)
		{
			bHorizonVec[i] = true;
			myPoint[i].Point_X -= 0.1f;
			if (bVerticalVec[i])
			{
				myPoint[i].Point_Y += 0.1f;
				if (myPoint[i].Point_Y >= 1.0f)
				{
					bVerticalVec[i] = false;
					myPoint[i].Point_Y -= 0.1f;
				}
			}
			else
			{
				myPoint[i].Point_Y -= 0.1f;
				if (myPoint[i].Point_Y <= -1.0f)
				{
					bVerticalVec[i] = true;
					myPoint[i].Point_Y += 0.1f;
				}
			}

		}
	}
}

void TimerFunction(int value)
{
	if (bMove)
	{
		for (int i = iTriangle - 1; i >= 0; --i)
		{
			if (bCommandA)
			{
				if (i % 2 == 1)
				{
					myPoint[i].Point_X = myPoint[i - 1].Point_X;
					myPoint[i].Point_Y = myPoint[i - 1].Point_Y;
					bVerticalVec[i] = bVerticalVec[i - 1];
					bHorizonVec[i] = bHorizonVec[i - 1];
				}

				else
					Moving(i);
			}
			else
				Moving(i);
		}
	}
	glutPostRedisplay();
	glutTimerFunc(iSpeed, TimerFunction, 1); // 타이머함수 재 설정
}