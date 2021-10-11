#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
GLuint vertexShader[2], fragmentShader[2];
GLuint s_program[2];
GLuint vao[2], vbo[4], EBO[2];
GLint width, height;

void KeyBoard(unsigned char key, int x, int y);
void SpecialKeyBoard(int key, int x, int y);
void KeyBoardUp(unsigned char key, int x, int y);
void SpecialKeyBoardUp(int key, int x, int y);
void TimerFunc(int value);
void DrawMain();
bool ShowPyramid = false;
bool HideBehind = true;
bool RotateX = false, RotateY = false;
bool MovePlusX = false, MovePlusY = false, MoveMinusX = false, MoveMinusY = false;
bool Wire = false;
GLfloat Xradius = 30.0, Yradius = 330.0;
GLfloat TransX = 0.0, TransY = 0.0;

GLfloat LineData[][3] = {
	{1.0, 0.0, -0.99},
	{-1.0, 0.0, -0.99},

	{0.0, -1.0, -0.99},
	{0.0, 1.0, -0.99}
};

GLfloat LineColor[][3] = {
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},

	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0}
};

GLfloat Dots[][3] = {
	{-0.5, 0.5, 0.5},
	{-0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5},
	{0.5, 0.5, 0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, -0.5, -0.5},
	{0.5, -0.5, -0.5},
	{0.5, 0.5, -0.5},
	{0.0, 0.5, 0.0}
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
	vertexsource = filetobuf("vertex_line.glsl");
	vertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[0], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[0]);

	vertexsource = filetobuf("vertex.glsl");
	vertexShader[1] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[1], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[1]);
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
}

void InitBuffer()
{
	glGenVertexArrays(2, vao);
	glGenBuffers(4, vbo);
	glGenBuffers(2, EBO);

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
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program[0]);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 4);

	glUseProgram(s_program[1]);
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::translate(transformMatrix, glm::vec3(TransX, TransY, 0.0));
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(Xradius), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(Yradius), glm::vec3(0.0, 1.0, 0.0));
	

	unsigned int modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glBindVertexArray(vao[1]);

	DrawMain();
	
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
	switch (key) {
	case 'C':
	case 'c':
		ShowPyramid = false;
		break;
	case 'P':
	case 'p':
		ShowPyramid = true;
		break;
	case 'H':
	case 'h':
		HideBehind = !HideBehind;
		break;
	case 'X':
	case 'x':
		RotateX = true;
		break;
	case 'Y':
	case 'y':
		RotateY = true;
		break;
	case 'W':
	case 'w':
		Wire = !Wire;
		break;
	case 'S':
	case 's':
		Wire = false;
		Xradius = 30.0, Yradius = 330.0;
		TransX = 0.0, TransY = 0.0;
		ShowPyramid = false;
		HideBehind = true;
		RotateX = false, RotateY = false;
		MovePlusX = false, MovePlusY = false, MoveMinusX = false, MoveMinusY = false;
		break;
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void SpecialKeyBoard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		MovePlusX = true;
		break;
	case GLUT_KEY_LEFT:
		MoveMinusX = true;
		break;
	case GLUT_KEY_UP:
		MovePlusY = true;
		break;
	case GLUT_KEY_DOWN:
		MoveMinusY = true;
		break;
	}
}

void SpecialKeyBoardUp(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		MovePlusX = false;
		break;
	case GLUT_KEY_LEFT:
		MoveMinusX = false;
		break;
	case GLUT_KEY_UP:
		MovePlusY = false;
		break;
	case GLUT_KEY_DOWN:
		MoveMinusY = false;
		break;
	}
}

void KeyBoardUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 'X':
	case 'x':
		RotateX = false;
		break;
	case 'Y':
	case 'y':
		RotateY = false;
		break;
	}
	glutPostRedisplay();
}

void TimerFunc(int value)
{
	if(RotateX)
		Xradius = (int)(Xradius + 5.0) % 360;
	if(RotateY)
		Yradius = (int)(Yradius + 5.0) % 360;
	if (MovePlusX)
		if(TransX < 0.95f)
			TransX += 0.05f;
	if (MovePlusY)
		if (TransY < 0.95f)
			TransY += 0.05f;
	if (MoveMinusX)
		if (TransX > -0.95f)
			TransX -= 0.05f;
	if (MoveMinusY)
		if (TransY > -0.95f)
			TransY -= 0.05f;

	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void DrawMain()
{
	if(Wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (HideBehind)
	{
		glEnable(GL_DEPTH_TEST);
		if (ShowPyramid)
			glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void*)(36 * sizeof(GLuint)));
		else
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glDisable(GL_DEPTH_TEST);
	}

	else
	{
		if (ShowPyramid)
			glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void*)(36 * sizeof(GLuint)));
		else
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
}