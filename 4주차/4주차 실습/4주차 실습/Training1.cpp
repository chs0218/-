#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#include <fstream>

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader, fragmentShader;
GLuint s_program;
GLuint vao, vbo[2], EBO[2];
GLint width, height;

GLfloat mx, my;
int iIndex = -1;
bool ButtonDown = false;
bool RectangleClick = false;
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

bool IsInside(GLfloat x, GLfloat y);

GLfloat Dots[4][3] = {
	{-0.5, -0.5, 0},
	{0.5, -0.5, 0},
	{0.5, 0.5, 0},
	{-0.5, 0.5, 0}
};

GLfloat tmpDots[4][3] = {
	{-0.5, -0.5, 0},
	{0.5, -0.5, 0},
	{0.5, 0.5, 0},
	{-0.5, 0.5, 0}
};

GLfloat colors[4][3] = { //--- 삼각형 꼭지점 색상
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.78, 0.41 },
	{ 0.91, 0.50, 0.21 },
	{ 0.53, 0.51, 0.64 }
};

unsigned int index[]{
	0, 1,
	1, 2,
	2, 3,
	3, 0
};

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
	glGenBuffers(2, EBO);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
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

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	InitBuffer();
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);
	glBindVertexArray(vao);
	glLineWidth(5);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
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

void main(int argc, char** argv)	//---윈도우 출력하고 콜백함수 설정
{
	width = 900;
	height = 900;

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training1");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void Mouse(int button, int state, int x, int y)
{
	GLfloat float_X = ((GLfloat)x / (GLfloat)width - 0.5f) * 2.0f;
	GLfloat float_Y = ((GLfloat)y / (GLfloat)height - 0.5f) * -2.0f;

	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		iIndex = -1;
		ButtonDown = true;
		mx = float_X;
		my = float_Y;
		for (int i = 0; i < 4; ++i)
		{
			if (float_X >= Dots[i][0] - 0.01f && float_X <= Dots[i][0] + 0.01f && float_Y <= Dots[i][1] + 0.01f && float_Y >= Dots[i][1] - 0.01f)
				iIndex = i;
		}

		if (iIndex == -1)
		{
			if (IsInside(float_X, float_Y))
			{
				RectangleClick = true;
				for (int i = 0; i < 4; ++i)
				{
					tmpDots[i][0] = Dots[i][0];
					tmpDots[i][1] = Dots[i][1];
				}
			}
		}
	}

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		ButtonDown = false;
		RectangleClick = false;
	}
}

void Motion(int x, int y)
{
	if (ButtonDown)
	{
		GLfloat float_X = ((GLfloat)x / (GLfloat)width - 0.5f) * 2.0f;
		GLfloat float_Y = ((GLfloat)y / (GLfloat)height - 0.5f) * -2.0f;

		if (iIndex == -1)
		{
			if (RectangleClick)
			{
				for (int i = 0; i < 4; ++i)
				{
					Dots[i][0] = tmpDots[i][0] +(float_X - mx);
					Dots[i][1] = tmpDots[i][1] + (float_Y - my);
				}
			}
		}

		else
		{
			Dots[iIndex][0] = float_X;
			Dots[iIndex][1] = float_Y;
		}
	}

	glutPostRedisplay();
}

bool IsInside(GLfloat x, GLfloat y)
{
	bool isInnerPoint = false;

	for (int i = 0; i < 4; ++i)
	{
		if ((Dots[i % 4][1] < y && y <= Dots[(i + 1) % 4][1]) || (Dots[(i + 1) % 4][1] < y && y <= Dots[i % 4][1]))
		{
			if (Dots[i % 4][0] + (y - Dots[i % 4][1]) / (Dots[(i + 1) % 4][1] - Dots[i % 4][1]) * (Dots[(i + 1) % 4][0] - Dots[i % 4][0]) < x) {
				isInnerPoint = !isInnerPoint;
			}
		}
	}

	return isInnerPoint;
}
