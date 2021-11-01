#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <random>

#define BOXSIZE 0.5

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader[2], fragmentShader[3];
GLuint s_program[3];
GLuint vao[2], vbo[4], EBO[4];
GLint width, height;


GLfloat cameraX = 0.0, cameraY = 5.0, cameraZ = 5.0;
glm::vec3 cameraPos = glm::vec3(cameraX, cameraY, cameraZ); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0, 0.0, 0.0); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

void DrawMain();
void RandomObjects();
void KeyBoard(unsigned char key, int x, int y);
void RotateCameraCenterY();
void TimerFunc(int value);
void CheckCollision();
void ShrinkObject();
void ResetObject();

GLfloat ObjectsX[20], ObjectsZ[20];
GLfloat ScaleX[20], ScaleY[20], ScaleZ[20];
GLfloat eraserX, eraserZ;
bool ObjectCollision[20];
bool RotateR = false, RotateReverseR = false;

GLfloat LineDots[][3] = {
	{5.0, 0.0, 0.0},
	{-5.0, 0.0, 0.0},
	{0.0, 5.0, 0.0},
	{0.0, -5.0, 0.0},
	{0.0, 0.0, 5.0},
	{0.0, 0.0, -5.0},
};

GLfloat LineColors[][3] = {
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
};

unsigned int Lineindex[] = {
	0, 1,
	2, 3,
	4, 5
};

GLfloat Dots[][3] = {
	// 육면체
	{-BOXSIZE, BOXSIZE, BOXSIZE},
	{-BOXSIZE, -BOXSIZE, BOXSIZE},
	{BOXSIZE, -BOXSIZE, BOXSIZE},
	{BOXSIZE, BOXSIZE, BOXSIZE},
	{-BOXSIZE, BOXSIZE, -BOXSIZE},
	{-BOXSIZE, -BOXSIZE, -BOXSIZE},
	{BOXSIZE, -BOXSIZE, -BOXSIZE},
	{BOXSIZE, BOXSIZE, -BOXSIZE}
};

GLfloat Colors[][3] = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
	{0.0, 1.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 1.0, 1.0},
	{0.0, 0.0, 0.0}
};

unsigned int Shapeindex[] = {
	// 육면체
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

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	vertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[0], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[0]);

	vertexsource = filetobuf("vertex_eraser.glsl");
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

	fragmentsource = filetobuf("fragment_red.glsl");
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[1]);

	fragmentsource = filetobuf("fragment_blue.glsl");
	fragmentShader[2] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[2], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[2]);
}

void InitBuffer()
{
	glGenVertexArrays(2, vao);
	glGenBuffers(4, vbo);
	glGenBuffers(4, EBO);

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineDots), LineDots, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Lineindex), Lineindex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineColors), LineColors, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Lineindex), Lineindex, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[1]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Shapeindex), Shapeindex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Shapeindex), Shapeindex, GL_STATIC_DRAW);
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

	s_program[1] = glCreateProgram();

	glAttachShader(s_program[1], vertexShader[1]);
	glAttachShader(s_program[1], fragmentShader[1]);
	glLinkProgram(s_program[1]);

	s_program[2] = glCreateProgram();

	glAttachShader(s_program[2], vertexShader[1]);
	glAttachShader(s_program[2], fragmentShader[2]);
	glLinkProgram(s_program[2]);

	glDeleteShader(vertexShader[0]);
	glDeleteShader(vertexShader[1]);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);
	glDeleteShader(fragmentShader[2]);
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	DrawMain();
	glDisable(GL_DEPTH_TEST);
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
	RandomObjects();

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training21");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutKeyboardFunc(KeyBoard);
	glutTimerFunc(25, TimerFunc, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void KeyBoard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'W':
	case 'w':
		if (eraserZ > -9.0)
			eraserZ -= 0.1f;
		break;
	case 'S':
	case 's':
		if (eraserZ < 9.0)
			eraserZ += 0.1f;
		break;
	case 'A':
	case 'a':
		if (eraserX > -9.0)
			eraserX -= 0.1f;
		break;
	case 'D':
	case 'd':
		if (eraserX < 9.0)
			eraserX += 0.1f;
		break;
	case 'R':
		RotateR = false;
		RotateReverseR = !RotateReverseR;
		break;
	case 'r':
		RotateReverseR = false;
		RotateR = !RotateR;
		break;
	case 'c':
		ResetObject();
		break;
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void TimerFunc(int value)
{
	if (RotateR || RotateReverseR)
		RotateCameraCenterY();

	CheckCollision();
	ShrinkObject();
	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void RotateCameraCenterY()
{
	GLfloat tmpX = 0.0, tmpZ = 0.0;

	if (RotateR)
	{
		tmpX = cameraZ * glm::sin(glm::radians(1.0)) + cameraX * glm::cos(glm::radians(1.0));
		tmpZ = cameraZ * glm::cos(glm::radians(1.0)) - cameraX * glm::sin(glm::radians(1.0));
	}

	else
	{
		tmpX = cameraZ * glm::sin(glm::radians(-1.0)) + cameraX * glm::cos(glm::radians(-1.0));
		tmpZ = cameraZ * glm::cos(glm::radians(-1.0)) - cameraX * glm::sin(glm::radians(-1.0));
	}
	

	cameraX = tmpX;
	cameraZ = tmpZ;
}

void DrawMain()
{
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);


	projection = glm::perspective(glm::radians(130.0f), (float)width / (float)height, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -0.5));

	cameraPos = glm::vec3(cameraX, cameraY, cameraZ);

	// 좌표축
	glUseProgram(s_program[0]);
	glBindVertexArray(vao[0]);

	viewLocation = glGetUniformLocation(s_program[0], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);


	// 바닥
	transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0, -0.05, 0.0));
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(-90.0), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(20.0, 20.0, 1.0));
	transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0, 0.0, -BOXSIZE));


	// 바닥깔기
	glUseProgram(s_program[0]);
	glBindVertexArray(vao[1]);

	viewLocation = glGetUniformLocation(s_program[0], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Object 생성
	for (int i = 0; i < 20; ++i)
	{
		transformMatrix = glm::mat4(1.0f);
		transformMatrix = glm::translate(transformMatrix, glm::vec3(ObjectsX[i], BOXSIZE * ScaleY[i], ObjectsZ[i]));
		transformMatrix = glm::scale(transformMatrix, glm::vec3(ScaleX[i], ScaleY[i], ScaleZ[i]));

		modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	// 지우개 생성

	glUseProgram(s_program[1]);
	glBindVertexArray(vao[1]);

	transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::translate(transformMatrix, glm::vec3(eraserX, BOXSIZE, eraserZ));

	viewLocation = glGetUniformLocation(s_program[1], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[1], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glUseProgram(s_program[2]);
	glBindVertexArray(vao[1]);

	transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::translate(transformMatrix, glm::vec3(eraserX, BOXSIZE * 3, eraserZ));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(0.5, 1, 0.5));

	viewLocation = glGetUniformLocation(s_program[1], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[1], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void CheckCollision()
{
	for (int i = 0; i < 20; ++i)
	{
		if (!ObjectCollision[i])
		{
			if (ObjectsX[i] - BOXSIZE * ScaleX[i] < eraserX + BOXSIZE &&
				ObjectsX[i] + BOXSIZE * ScaleX[i] > eraserX - BOXSIZE &&
				ObjectsZ[i] - BOXSIZE * ScaleZ[i] < eraserZ + BOXSIZE &&
				ObjectsZ[i] + BOXSIZE * ScaleZ[i] > eraserZ - BOXSIZE)
				ObjectCollision[i] = true;
		}
	}
}

void ShrinkObject()
{
	for (int i = 0; i < 20; ++i)
	{
		if (ObjectCollision[i])
		{
			if (ScaleX[i] > 0)
				ScaleX[i] -= 0.1f;
			else if (ScaleX[i] < 0)
				ScaleX[i] = 0.0;

			if (ScaleY[i] > 0)
				ScaleY[i] -= 0.1f;
			else if (ScaleY[i] < 0)
				ScaleY[i] = 0.0;

			if (ScaleZ[i] > 0)
				ScaleZ[i] -= 0.1f;
			else if (ScaleZ[i] < 0)
				ScaleZ[i] = 0.0;
		}
	}
}

void RandomObjects()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 160);
	GLfloat tmp;

	for (int i = 0; i < 20; ++i)
	{
		tmp = (GLfloat)dis(gen);
		tmp = (tmp - 80.0) * 0.1;
		ObjectsX[i] = tmp;

		tmp = (GLfloat)dis(gen);
		tmp = (tmp - 80.0) * 0.1;
		ObjectsZ[i] = tmp;

		tmp = (GLfloat)dis(gen);
		tmp = (tmp + 25.0) * 0.01;
		ScaleX[i] = tmp;

		tmp = (GLfloat)dis(gen);
		tmp = (tmp + 25.0) * 0.01;
		ScaleY[i] = tmp;

		tmp = (GLfloat)dis(gen);
		tmp = (tmp + 25.0) * 0.01;
		ScaleZ[i] = tmp;
	}

	tmp = (GLfloat)dis(gen);
	tmp = (tmp - 80.0) * 0.1;
	eraserX = tmp;
	
	tmp = (GLfloat)dis(gen);
	tmp = (tmp - 80.0) * 0.1;
	eraserZ = tmp;
}

void ResetObject()
{
	cameraX = 0.0, cameraY = 5.0, cameraZ = 5.0;
	RandomObjects();
	for (int i = 0; i < 20; ++i)
	{
		ObjectCollision[i] = false;
	}
	RotateR = false, RotateReverseR = false;
}