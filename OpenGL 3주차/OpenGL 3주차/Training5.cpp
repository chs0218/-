#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <random>

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);
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


void InitDots(int x, int y);

GLfloat Dots[218][3];
GLfloat colors[218][3];
GLfloat BK_RED = 1.0f, BK_GREEN = 1.0f, BK_BLUE = 1.0f;
bool Draw = false;
int Animation = 0;


void main(int argc, char** argv)	//---윈도우 출력하고 콜백함수 설정
{
	width = 900;
	height = 900;

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_DYNAMIC_DRAW);
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

void DrawScene()
{
	glClearColor(BK_RED, BK_GREEN, BK_BLUE, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);
	glBindVertexArray(vao);
	glPointSize(3);
	if (Draw)
		glDrawArrays(GL_POINTS, 0, Animation);
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	if (!Draw)
		Draw = true;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 99);
		BK_RED = dis(gen) * 0.01f;
		BK_GREEN = dis(gen) * 0.01f;
		BK_BLUE = dis(gen) * 0.01f;
		
		InitDots(x, y);
		InitBuffer();
	}
	glutPostRedisplay();
}
void TimerFunction(int value)
{
	if (Animation < 218)
		++Animation;
	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction, 1); // 타이머함수 재 설정
}

void InitDots(int x, int y)
{
	GLfloat floatX = ((GLfloat)x / (GLfloat)width - 0.5f) * 2.0f;
	GLfloat floatY = ((GLfloat)y / (GLfloat)height - 0.5f) * -2.0f;
	
	int iDegree = 0;
	const double PI = 3.141592;
	GLfloat fRadius = 0;
	Animation = 0;

	for (int i = 0; i < 109; ++i)
	{
		Dots[i][0] = floatX + fRadius * (GLfloat)cos(iDegree *  PI / 180.0f);
		Dots[i][1] = floatY + fRadius * (GLfloat)sin(iDegree * PI / 180.0f);
		colors[i][0] = 1.0f;
		colors[i][1] = 1.0f;
		colors[i][2] = 1.0f;
		fRadius += 0.002f;
		iDegree += 10;
	}

	floatX += 0.436f;
	iDegree -= 90;

	for (int i = 109; i < 218; ++i)
	{
		Dots[i][0] = floatX + fRadius * (GLfloat)sin(iDegree * PI / 180.0f);
		Dots[i][1] = floatY + fRadius * (GLfloat)cos(iDegree * PI / 180.0f);
		colors[i][0] = 1.0f;
		colors[i][1] = 1.0f;
		colors[i][2] = 1.0f;
		fRadius -= 0.002f;
		iDegree += 10;
	}

}