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
void SpecialKeyBoard(int key, int x, int y);
void SpecialKeyBoardUp(int key, int x, int y);
void TimerFunc(int value);
void DrawRight();
void DrawLeft();

GLfloat RightMoveX = 0.0, RightMoveY = 0.0, RightMoveZ = 0.0;
GLfloat LeftMoveX = 0.0, LeftMoveY = 0.0, LeftMoveZ = 0.0;
GLfloat MoveAllX = 0.0, MoveAllY = 0.0;
GLfloat LeftScale = 1.0, RightScale = 1.0;
GLfloat LeftZeroScale = 1.0, RightZeroScale = 1.0;

bool RightMovePlusX = false, RightMovePlusY = false, RightMovePlusZ = false;
bool LeftMovePlusX = false, LeftMovePlusY = false, LeftMovePlusZ = false;
bool RightMoveMinusX = false, RightMoveMinusY = false, RightMoveMinusZ = false;
bool LeftMoveMinusX = false, LeftMoveMinusY = false, LeftMoveMinusZ = false;

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
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 1.0, 0.0 },
	{ 1.0, 0.0, 1.0 },
	{ 0.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 0.0, 0.0, 0.0 },
	{ 0.95, 0.82, 0.71 },
};

unsigned int index[]{
	0, 1, 2,
	0, 2, 3,
	3, 2, 6,
	3, 6, 7,
	4, 0, 3,
	4, 3, 7,
	4, 6, 5,
	4, 7, 6,
	4, 5, 1,
	4, 1, 0,
	5, 1, 2,
	5, 2, 6,


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
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 transLeft = glm::mat4(1.0f);
	glm::mat4 transRight = glm::mat4(1.0f);
	glm::mat4 LeftScaleMat = glm::mat4(1.0f);
	glm::mat4 RightScaleMat = glm::mat4(1.0f);
	glm::mat4 MoveAll = glm::mat4(1.0f);
	LeftScaleMat = glm::scale(LeftScaleMat, glm::vec3(LeftZeroScale, LeftZeroScale, LeftZeroScale));
	RightScaleMat = glm::scale(RightScaleMat, glm::vec3(RightZeroScale, RightZeroScale, RightZeroScale));
	MoveAll = glm::translate(MoveAll, glm::vec3(MoveAllX, MoveAllY, 0.0f));
	

	glUseProgram(s_program[0]);
	glBindVertexArray(vao[0]);
	glm::mat4 transformMatrix0 = glm::mat4(1.0f);
	transformMatrix0 = glm::rotate(transformMatrix0, (GLfloat)glm::radians(30.0), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix0 = glm::rotate(transformMatrix0, (GLfloat)glm::radians(330.0), glm::vec3(0.0, 1.0, 0.0));

	unsigned int modelLocation0 = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation0, 1, GL_FALSE, glm::value_ptr(transformMatrix0));
	glBindVertexArray(vao[0]);

	glDrawArrays(GL_LINES, 0, 6);

	transLeft = glm::translate(transformMatrix0, glm::vec3(-0.5 + LeftMoveX, LeftMoveY, LeftMoveZ));
	transRight = glm::translate(transformMatrix0, glm::vec3(0.5 + RightMoveX, RightMoveY, RightMoveZ));

	glUseProgram(s_program[1]);
	glm::mat4 transformMatrix1 = glm::mat4(1.0f);
	transformMatrix1 = glm::scale(transformMatrix1, glm::vec3(LeftScale, LeftScale, LeftScale));
	transformMatrix1 = LeftScaleMat * MoveAll * transLeft * transformMatrix1;

	unsigned int modelLocation1 = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation1, 1, GL_FALSE, glm::value_ptr(transformMatrix1));
	glBindVertexArray(vao[1]);
	DrawLeft();

	glUseProgram(s_program[2]);
	
	glm::mat4 transformMatrix2 = glm::mat4(1.0f);
	transformMatrix2 = glm::scale(transformMatrix2, glm::vec3(RightScale, RightScale, RightScale));
	transformMatrix2 = RightScaleMat * MoveAll * transRight * transformMatrix2;
	
	unsigned int modelLocation2 = glGetUniformLocation(s_program[2], "modelTransform");
	glUniformMatrix4fv(modelLocation2, 1, GL_FALSE, glm::value_ptr(transformMatrix2));
	glBindVertexArray(vao[2]);
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
	glutSpecialFunc(SpecialKeyBoard);
	glutSpecialUpFunc(SpecialKeyBoardUp);
	glutTimerFunc(25, TimerFunc, 1);
	glutMainLoop();
}

void KeyBoard(unsigned char key, int x, int y)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 4);
	switch (key) {
	case 'w':
		if (MoveAllY < 0.5f)
			MoveAllY += 0.01f;
		break;
	case 'a':
		if (MoveAllX > -0.5f)
			MoveAllX -= 0.01f;
		break;
	case 's':
		if (MoveAllY > -0.5f)
			MoveAllY -= 0.01f;
		break;
	case 'd':
		if (MoveAllX < 0.5f)
			MoveAllX += 0.01f;
		break;
	case 'C':
	case 'c':

		break;
	case 'L':
	case 'l':
		LeftMovePlusX = true;
		break;
	case 'J':
	case 'j':
		LeftMoveMinusX = true;
		break;
	case 'I':
	case 'i':
		LeftMovePlusY = true;
		break;
	case 'K':
	case 'k':
		LeftMoveMinusY = true;
		break;
	case 'O':
	case 'o':
		LeftMovePlusZ = true;
		break;
	case 'P':
	case 'p':
		LeftMoveMinusZ = true;
		break;
	case 'm':
		if (LeftScale < 2.0f)
			LeftScale += 0.2f;
		break;
	case 'n':
		if (LeftScale > 0.5f)
			LeftScale -= 0.2f;
		break;
	case 'M':
		if (RightScale < 2.0f)
			RightScale += 0.2f;
		break;
	case 'N':
		if (RightScale > 0.5f)
			RightScale -= 0.2f;
		break;
	case ',':
		if (LeftZeroScale > 0.5f)
			LeftZeroScale -= 0.2f;
		break;
	case '.':
		if (LeftZeroScale < 2.0f)
			LeftZeroScale += 0.2f;
		break;
	case '<':
		if (RightZeroScale > 0.5f)
			RightZeroScale -= 0.2f;
		break;
	case '>':
		if (RightZeroScale < 2.0f)
			RightZeroScale += 0.2f;
		break;
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	}
}

void KeyBoardUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 'L':
	case 'l':
		LeftMovePlusX = false;
		break;
	case 'J':
	case 'j':
		LeftMoveMinusX = false;
		break;
	case 'I':
	case 'i':
		LeftMovePlusY = false;
		break;
	case 'K':
	case 'k':
		LeftMoveMinusY = false;
		break;
	case 'O':
	case 'o':
		LeftMovePlusZ = false;
		break;
	case 'P':
	case 'p':
		LeftMoveMinusZ = false;
		break;
	}
}

void TimerFunc(int value)
{
	if (RightMovePlusX)
	{
		if (RightMoveX < 0.49f)
			RightMoveX += 0.01f;
	}
	if (RightMoveMinusX)
	{
		if (RightMoveX > -0.49f)
			RightMoveX -= 0.01f;
	}
	if (RightMovePlusY)
	{
		if (RightMoveY < 0.8f)
			RightMoveY += 0.01f;
	}
	if (RightMoveMinusY)
	{
		if (RightMoveY > -0.8f)
			RightMoveY -= 0.01f;
	}
	if (RightMovePlusZ)
	{
		if (RightMoveZ < 0.8f)
			RightMoveZ += 0.01f;
	}
	if (RightMoveMinusZ)
	{
		if (RightMoveZ > -0.8f)
			RightMoveZ -= 0.01f;
	}


	if (LeftMovePlusX)
	{
		if (LeftMoveX < 0.49)
			LeftMoveX += 0.01f;
	}
	if (LeftMoveMinusX)
	{
		if (LeftMoveX > -0.49f)
			LeftMoveX -= 0.01f;
	}
	if (LeftMovePlusY)
	{
		if (LeftMoveY < 0.8f)
			LeftMoveY += 0.01f;
	}
	if (LeftMoveMinusY)
	{
		if (LeftMoveY > -0.8f)
			LeftMoveY -= 0.01f;
	}
	if (LeftMovePlusZ)
	{
		if (LeftMoveZ < 0.8f)
			LeftMoveZ += 0.01f;
	}
	if (LeftMoveMinusZ)
	{
		if (LeftMoveZ > -0.8f)
			LeftMoveZ -= 0.01f;
	}

	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void DrawLeft()
{
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricOrientation(qobj, GLU_OUTSIDE);
	gluSphere(qobj, 0.1, 10, 10);
}

void DrawRight()
{
	glPolygonMode(GL_FRONT, GL_FILL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void SpecialKeyBoard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		RightMovePlusX = true;
		break;
	case GLUT_KEY_LEFT:
		RightMoveMinusX = true; 
		break;
	case GLUT_KEY_UP:
		RightMovePlusY = true;
		break;
	case GLUT_KEY_DOWN:
		RightMoveMinusY = true; 
		break;
	case GLUT_KEY_PAGE_UP:
		RightMovePlusZ = true;
		break;
	case GLUT_KEY_PAGE_DOWN:
		RightMoveMinusZ = true; 
		break;
	}
}

void SpecialKeyBoardUp(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		RightMovePlusX = false;
		break;
	case GLUT_KEY_LEFT:
		RightMoveMinusX = false;
		break;
	case GLUT_KEY_UP:
		RightMovePlusY = false;
		break;
	case GLUT_KEY_DOWN:
		RightMoveMinusY = false;
		break;
	case GLUT_KEY_PAGE_UP:
		RightMovePlusZ = false;
		break;
	case GLUT_KEY_PAGE_DOWN:
		RightMoveMinusZ = false;
		break;
	}
}