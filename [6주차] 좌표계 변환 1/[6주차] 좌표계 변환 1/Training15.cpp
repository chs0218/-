#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <iostream>
#include <fstream>

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);
GLvoid DrawScene();
void InitTriangle();

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader[3], fragmentShader[3];
GLuint s_program[4];
GLuint vao[3], vbo[6], EBO[4];
GLint width, height;

void KeyBoard(unsigned char key, int x, int y);
void KeyBoardUp(unsigned char key, int x, int y);
void TimerFunc(int value);
void DrawRight();
void DrawLeft();

GLfloat Xradius = 30.0, Rradius = 330.0, Yradius = 330.0;
GLfloat RightXradius = 0.0, RightYradius = 0.0;
GLfloat LeftXradius = 0.0, LeftYradius = 0.0;
bool RotateRightX = false, RotateRightY = false;
bool RotateLeftX = false, RotateLeftY = false;
bool RotateR = false;
GLint LeftSeed = 1, RightSeed = 0;

GLfloat LineData[][3] = {
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},

	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0},

	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0}
};

GLfloat LineColor[][3] = {
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0}
};

GLfloat Dots[][3] = {
	{-0.15, 0.15, 0.15},
	{-0.15, -0.15, 0.15},
	{0.15, -0.15, 0.15},
	{0.15, 0.15, 0.15},
	{-0.15, 0.15, -0.15},
	{-0.15, -0.15, -0.15},
	{0.15, -0.15, -0.15},
	{0.15, 0.15, -0.15},
	{0.0, 0.15, 0.0}
};

GLfloat colors[][3] = {
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.78, 0.41 },
	{ 0.91, 0.50, 0.21 },
	{ 0.53, 0.51, 0.64 },
	{ 0.47, 0.60, 0.50 },
	{ 0.33, 0.78, 0.82 },

	{ 0.12, 0.71, 0.65 },
	{ 0.71, 0.46, 0.13 },
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.78, 0.41 }
};

unsigned int index[]{
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
	8, 5, 1,
	8, 6, 5,
	1, 5, 2,
	2, 5, 6
};

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	vertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[0], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[0]);
	vertexShader[1] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[1], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[1]);
	vertexShader[2] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[2], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[2]);
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment.glsl");
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[0], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[0]);

	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[1]);

	fragmentShader[2] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[2], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[2]);
}

void InitBuffer()
{
	glGenVertexArrays(3, vao);
	glGenBuffers(6, vbo);
	glGenBuffers(4, EBO);

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineData), LineData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineColor), LineColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[1]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShaders();
	make_fragmentShaders();

	s_program[0] = glCreateProgram();
	glAttachShader(s_program[0], vertexShader[0]);
	glAttachShader(s_program[0], fragmentShader[0]);
	glLinkProgram(s_program[0]);

	glDeleteShader(vertexShader[0]);
	glDeleteShader(fragmentShader[0]);

	s_program[1] = glCreateProgram();

	glAttachShader(s_program[1], vertexShader[1]);
	glAttachShader(s_program[1], fragmentShader[1]);
	glLinkProgram(s_program[1]);

	glDeleteShader(vertexShader[1]);
	glDeleteShader(fragmentShader[1]);

	s_program[2] = glCreateProgram();

	glAttachShader(s_program[2], vertexShader[2]);
	glAttachShader(s_program[2], fragmentShader[2]);
	glLinkProgram(s_program[2]);

	glDeleteShader(vertexShader[2]);
	glDeleteShader(fragmentShader[2]);
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 tmp1 = glm::mat4(1.0f);
	glm::mat4 tmp2 = glm::mat4(1.0f);

	glUseProgram(s_program[0]);
	glBindVertexArray(vao[0]);
	glm::mat4 transformMatrix0 = glm::mat4(1.0f);
	transformMatrix0 = glm::rotate(transformMatrix0, (GLfloat)glm::radians(30.0), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix0 = glm::rotate(transformMatrix0, (GLfloat)glm::radians(Rradius), glm::vec3(0.0, 1.0, 0.0));
	unsigned int modelLocation0 = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation0, 1, GL_FALSE, glm::value_ptr(transformMatrix0));
	glBindVertexArray(vao[0]);

	glDrawArrays(GL_LINES, 0, 6);

	tmp1 = glm::translate(transformMatrix0, glm::vec3(-0.5, 0.0, 0.0));
	tmp2 = glm::translate(transformMatrix0, glm::vec3(0.5, 0.0, 0.0));

	glUseProgram(s_program[1]);
	glm::mat4 transformMatrix1 = glm::mat4(1.0f);
	transformMatrix1 = glm::rotate(transformMatrix1, (GLfloat)glm::radians(RightXradius), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix1 = glm::rotate(transformMatrix1, (GLfloat)glm::radians(RightYradius), glm::vec3(0.0, 1.0, 0.0));
	transformMatrix1 = tmp1 * transformMatrix1;

	unsigned int modelLocation1 = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation1, 1, GL_FALSE, glm::value_ptr(transformMatrix1));
	glBindVertexArray(vao[1]);
	glEnable(GL_DEPTH_TEST);
	DrawLeft();

	glUseProgram(s_program[2]);
	glm::mat4 transformMatrix2 = glm::mat4(1.0f);
	transformMatrix2 = glm::rotate(transformMatrix2, (GLfloat)glm::radians(LeftXradius), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix2 = glm::rotate(transformMatrix2, (GLfloat)glm::radians(LeftYradius), glm::vec3(0.0, 1.0, 0.0));
	transformMatrix2 = tmp2 * transformMatrix2;

	unsigned int modelLocation2 = glGetUniformLocation(s_program[2], "modelTransform");
	glUniformMatrix4fv(modelLocation2, 1, GL_FALSE, glm::value_ptr(transformMatrix2));
	glBindVertexArray(vao[2]);
	glEnable(GL_DEPTH_TEST);

	DrawRight();
	
	glutSwapBuffers();
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training13");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyBoard);
	glutKeyboardUpFunc(KeyBoardUp);
	glutTimerFunc(25, TimerFunc, 1);
	glutMainLoop();
}

void KeyBoard(unsigned char key, int x, int y)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 4);
	switch (key) {
	case 'X':
	case 'x':
		RotateRightX = true;
		break;
	case 'Y':
	case 'y':
		RotateRightY = true;
		break;
	case 'A':
	case 'a':
		RotateLeftX = true;
		break;
	case 'B':
	case 'b':
		RotateLeftY = true;
		break;
	case 'R':
	case 'r':
		RotateR = true;
		break;
	case 'S':
	case 's':
		Xradius = 30.0, Rradius = 330.0, Yradius = 330.0;
		RightXradius = 0.0, RightYradius = 0.0;
		LeftXradius = 0.0, LeftYradius = 0.0;
		RightSeed = 0, LeftSeed = 1;
		break;
	case 'C':
	case 'c':
		LeftSeed = dis(gen);
		RightSeed = dis(gen);
		break;
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void KeyBoardUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 'A':
	case 'a':
		RotateLeftX = false;
		break;
	case 'B':
	case 'b':
		RotateLeftY = false;
		break;
	case 'R':
	case 'r':
		RotateR = false;
		break;
	case 'X':
	case 'x':
		RotateRightX = false;
		break;
	case 'Y':
	case 'y':
		RotateRightY = false;
		break;
	}
	glutPostRedisplay();
}

void TimerFunc(int value)
{
	if (RotateRightX)
		RightXradius = (int)(RightXradius + 5.0) % 360;
	if (RotateRightY)
		RightYradius = (int)(RightYradius + 5.0) % 360;
	if (RotateLeftX)
		LeftXradius = (int)(LeftXradius + 5.0) % 360;
	if (RotateLeftY)
		LeftYradius = (int)(LeftYradius + 5.0) % 360;
	if (RotateR)
		Rradius = (int)(Rradius + 5.0) % 360;

	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void DrawLeft()
{
	GLUquadricObj* qobj;
	switch (LeftSeed)
	{
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void*)(36 * sizeof(GLuint)));
		break;
	case 2:
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluSphere(qobj, 0.1, 10, 10);
		break;
	case 3:
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluSphere(qobj, 0.1, 3, 8);
		break;
	case 4:
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluDisk(qobj, 0.05, 0.2, 10, 3);
		break;
	}
	
}

void DrawRight()
{
	GLUquadricObj* qobj;
	switch (RightSeed)
	{
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void*)(36 * sizeof(GLuint)));
		break;
	case 2:
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluSphere(qobj, 0.1, 10, 10);
		break;
	case 3:
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluSphere(qobj, 0.1, 3, 8);
		break;
	case 4:
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluDisk(qobj, 0.05, 0.2, 10, 3);
		break;
	}
}