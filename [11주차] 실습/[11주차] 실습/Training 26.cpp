#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>

#define BOXSIZE 0.5

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader[2], fragmentShader[2];
GLuint s_program[2];
GLuint vao[2], vbo[2];
GLint width, height;

bool RotateA = false, RotateR = false, RotateY = false, MovePlusZ = false, MoveMinusZ = false, RotateCranePlus = false, RotateCraneMinus = false, RotateArm = false, ArmPlus = true;
GLfloat RotateCenterY = 0.0, cameraX = 0.0, cameraY = 3.0, cameraZ = 4.0, cameraR = 0.0, craneZ = 0.0, cranebodyR = 0.0, armR = 0.0, RotateCenter = 0.0;
GLfloat CameraDirX = 0.0, CameraDirY = 0.0, CameraDirZ = 0.0;

// light
GLfloat lightX = 0.0f, lightY = 8.0f, lightZ = 12.0f, Lradius = 0.0f;
GLfloat lightColor[3] = { 1.0f, 1.0f, 1.0f };
bool RotateL = false, TurnOff = false, RotateMinus = false;
glm::vec3 cameraPos = glm::vec3(cameraX, cameraY, cameraZ); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(CameraDirX, CameraDirY, CameraDirZ); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

void DrawMain();
void RotateCameraCenterY();
void RotateCameraCenter();
void RotateCamera();
void RotateCrane();
void MoveCraneZ();
void InitVertices();
void KeyBoard(unsigned char key, int x, int y);
void TimerFunc(int value);

float vertices[216];

float Lines[] = {
	3.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
	-3.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
	0.0, 3.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
	0.0, -3.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0,
	0.0, 0.0, 3.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
	0.0, 0.0, -3.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0
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

GLfloat normal[][3] = {
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0},
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0}
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

unsigned int normalindex[] = {
	// 육면체
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

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	vertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[0], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[0]);

	vertexsource = filetobuf("vertex_object.glsl");
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

	fragmentsource = filetobuf("fragment_object.glsl");
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[1]);
}

void InitBuffer()
{
	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Lines), Lines, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(vao[1]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
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

	glDeleteShader(vertexShader[0]);
	glDeleteShader(fragmentShader[0]);

	glDeleteShader(vertexShader[1]);
	glDeleteShader(fragmentShader[1]);
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
	InitVertices();

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training26");

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
	case 'B':
		if (MoveMinusZ)
			MoveMinusZ = false;
		else
		{
			MoveMinusZ = true;
			MovePlusZ = false;
		}
		break;
	case 'b':
		if (MovePlusZ)
			MovePlusZ = false;
		else
		{
			MovePlusZ = true;
			MoveMinusZ = false;
		}
		break;
	case 'M':
		if (RotateCraneMinus)
			RotateCraneMinus = false;
		else
		{
			RotateCraneMinus = true;
			RotateCranePlus = false;
		}
		break;
	case 'm':
		if (RotateCranePlus)
			RotateCranePlus = false;
		else
		{
			RotateCranePlus = true;
			RotateCraneMinus = false;
		}
		break;
	case 'T':
	case 't':
		RotateArm = !RotateArm;
		break;
	case 'X':
		CameraDirX -= 0.5f;
		cameraX -= 0.5f;
		break;
	case 'x':
		CameraDirX += 0.5f;
		cameraX += 0.5f;
		break;
	case 'y':
	case 'Y':
		RotateY = !RotateY;
		break;
	case 'A':
	case 'a':
		RotateA = !RotateA;
		break;
	case 'Z':
		CameraDirZ -= 0.5f;
		cameraZ -= 0.5f;
		break;
	case 'z':
		CameraDirZ += 0.5f;
		cameraZ += 0.5f;
		break;
	case 'R':
	case 'r':
		RotateR = !RotateR;
		break;
	case 'S':
	case 's':
		RotateA = false, RotateR = false, RotateY = false, MovePlusZ = false, MoveMinusZ = false, RotateCranePlus = false, RotateCraneMinus = false, RotateArm = false;
		break;
	case 'C':
	case 'c':
		CameraDirX = 0.0, CameraDirY = 0.0, CameraDirZ = 0.0;
		RotateCenterY = 0.0, cameraX = 0.0, cameraY = 3.0, cameraZ = 4.0, cameraR = 0.0, craneZ = 0.0, cranebodyR = 0.0, armR = 0.0, RotateCenter = 0.0;;
		RotateA = false, RotateR = false, RotateY = false, MovePlusZ = false, MoveMinusZ = false, RotateCranePlus = false, RotateCraneMinus = false, RotateArm = false, ArmPlus = true;
		break;
	case ',':
		TurnOff = !TurnOff;
		break;
	case '1':
		lightColor[0] = 1.0;
		lightColor[1] = 1.0;
		lightColor[2] = 1.0;
		break;
	case '2':
		lightColor[0] = 1.0;
		lightColor[1] = 0.0;
		lightColor[2] = 0.0;
		break;
	case '3':
		lightColor[0] = 0.0;
		lightColor[1] = 1.0;
		lightColor[2] = 0.0;
		break;
	case '4':
		lightColor[0] = 0.0;
		lightColor[1] = 0.0;
		lightColor[2] = 1.0;
		break;
	case 'L':
		if (!RotateMinus)
		{
			RotateMinus = true;
			RotateL = true;
		}
		else
			RotateL = !RotateL;
		break;
	case 'l':
		if (RotateMinus)
		{
			RotateMinus = false;
			RotateL = true;
		}
		else
			RotateL = !RotateL;
		break;
	case '.':
		RotateMinus = false;
		RotateL = false;
		RotateR = false;
		RotateY = false;
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
	if (RotateR)
		RotateCameraCenterY();
	if (RotateA)
		RotateCameraCenter();
	if (RotateY)
		cameraR = (GLfloat)((int)(cameraR + 1.0) % 360);
	if (MoveMinusZ || MovePlusZ)
		MoveCraneZ();
	if (RotateCranePlus || RotateCraneMinus)
		RotateCrane();
	if (RotateArm)
	{
		if (ArmPlus)
		{
			armR = armR + 1.0;
			if (armR >= 90.0)
				ArmPlus = false;
		}
		else
		{
			armR = armR - 1.0;
			if (armR <= -90.0)
				ArmPlus = true;
		}
	}
	if (RotateL)
	{
		if (RotateMinus)
			Lradius = (GLfloat)((int)(Lradius - 1.0f) % 360);
		else
			Lradius = (GLfloat)((int)(Lradius + 1.0f) % 360);
	}
	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void RotateCameraCenterY()
{
	GLfloat tmpX = cameraZ * glm::sin(glm::radians(1.0)) + cameraX * glm::cos(glm::radians(1.0));
	GLfloat tmpZ = cameraZ * glm::cos(glm::radians(1.0)) - cameraX * glm::sin(glm::radians(1.0));

	cameraX = tmpX;
	cameraZ = tmpZ;
}

void RotateCameraCenter()
{
	GLfloat tmpY = cameraY * glm::cos(glm::radians(1.0)) - cameraZ * glm::sin(glm::radians(1.0));
	GLfloat tmpZ = cameraY * glm::sin(glm::radians(1.0)) + cameraZ * glm::cos(glm::radians(1.0));

	cameraY = tmpY;
	cameraZ = tmpZ;
}

void RotateCamera()
{
	GLfloat tmpDirX = CameraDirX - cameraX;
	GLfloat tmpDirZ = CameraDirZ - cameraZ;
	GLfloat tmpX = tmpDirZ * glm::sin(glm::radians(cameraR)) + tmpDirX * glm::cos(glm::radians(cameraR));
	GLfloat tmpZ = tmpDirZ * glm::cos(glm::radians(cameraR)) - tmpDirX * glm::sin(glm::radians(cameraR));

	cameraDirection = glm::vec3(tmpX + cameraX, 0.0f, tmpZ + cameraZ);
}

void MoveCraneZ()
{
	if (MovePlusZ)
	{
		if (craneZ < 8.5f)
			craneZ += 0.1f;
	}
	else
	{
		if (craneZ > -8.5f)
			craneZ -= 0.1f;
	}
}

void RotateCrane()
{
	if (RotateCraneMinus)
	{
		cranebodyR = cranebodyR - 1.0;
		if (cranebodyR < 0.0)
			cranebodyR += 360.0;
	}
	else
	{
		cranebodyR = (GLfloat)((int)(cranebodyR + 1.0) % 360);
	}
}

void DrawMain()
{
	glm::mat4 transformMatrix[4] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };
	glm::mat4 rotateMatrix[4] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };
	glm::mat4 lightMatrix = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;
	unsigned int rotateLocation;

	int lightPosLocation;
	int lightColorLocation;
	int objColorLocation;
	int viewPosition;

	GLfloat tmpX = lightZ * glm::sin(glm::radians(Lradius)) + lightX * glm::cos(glm::radians(Lradius));
	GLfloat tmpZ = lightZ * glm::cos(glm::radians(Lradius)) - lightX * glm::sin(glm::radians(Lradius));

	projection = glm::perspective(glm::radians(130.0f), (float)width / (float)height, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -0.5));

	cameraPos = glm::vec3(cameraX, cameraY, cameraZ);
	cameraDirection = glm::vec3(CameraDirX, CameraDirY, CameraDirZ);
	RotateCamera();
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	// 좌표축
	glUseProgram(s_program[0]);
	glBindVertexArray(vao[0]);

	viewLocation = glGetUniformLocation(s_program[0], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[0]));

	rotateLocation = glGetUniformLocation(s_program[0], "rotateTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(rotateMatrix[0]));

	lightPosLocation = glGetUniformLocation(s_program[0], "lightPos");
	glUniform3f(lightPosLocation, tmpX, lightY, tmpZ);

	lightColorLocation = glGetUniformLocation(s_program[0], "lightColor");
	if(TurnOff)
		glUniform3f(lightColorLocation, 0.0, 0.0, 0.0);
	else
		glUniform3f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2]);


	glDrawArrays(GL_LINES, 0, 6);

	// 바닥
	transformMatrix[0] = glm::translate(transformMatrix[0], glm::vec3(0.0, -0.05, -BOXSIZE));
	transformMatrix[0] = glm::rotate(transformMatrix[0], (GLfloat)glm::radians(-90.0), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix[0] = glm::scale(transformMatrix[0], glm::vec3(20.0, 20.0, 1.0));
	transformMatrix[0] = glm::translate(transformMatrix[0], glm::vec3(0.0, 0.0, -BOXSIZE));

	rotateMatrix[0] = glm::rotate(rotateMatrix[0], (GLfloat)glm::radians(-90.0), glm::vec3(1.0, 0.0, 0.0));

	// 바닥깔기
	glUseProgram(s_program[1]);
	glBindVertexArray(vao[1]);

	viewLocation = glGetUniformLocation(s_program[1], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[1], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[0]));

	rotateLocation = glGetUniformLocation(s_program[1], "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(rotateMatrix[0]));

	lightPosLocation = glGetUniformLocation(s_program[1], "lightPos");
	glUniform3f(lightPosLocation, tmpX, lightY, tmpZ);

	lightColorLocation = glGetUniformLocation(s_program[1], "lightColor");
	if (TurnOff)
		glUniform3f(lightColorLocation, 0.0, 0.0, 0.0);
	else
		glUniform3f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2]);

	objColorLocation = glGetUniformLocation(s_program[1], "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 0.95, 0.82, 0.71);

	viewPosition = glGetUniformLocation(s_program[1], "viewPos");
	glUniform3f(viewPosition, cameraX, cameraY, cameraZ);

	glDrawArrays(GL_TRIANGLES, 0, 6);


	// 몸체
	transformMatrix[0] = glm::mat4(1.0f);
	transformMatrix[0] = glm::translate(transformMatrix[0], glm::vec3(0.0, BOXSIZE, craneZ));
	transformMatrix[0] = glm::scale(transformMatrix[0], glm::vec3(2.0, 1.0, 2.0));

	rotateMatrix[0] = glm::mat4(1.0f);
	// 크레인 몸체
	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[0]));

	rotateLocation = glGetUniformLocation(s_program[1], "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(rotateMatrix[0]));

	objColorLocation = glGetUniformLocation(s_program[1], "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 0.0, 0.0);

	glDrawArrays(GL_TRIANGLES, 0, 36);


	// 머리
	transformMatrix[1] = glm::translate(transformMatrix[1], glm::vec3(0.0, 2.0 * BOXSIZE, craneZ));
	transformMatrix[1] = glm::rotate(transformMatrix[1], (GLfloat)glm::radians(cranebodyR), glm::vec3(0.0, 1.0, 0.0));
	transformMatrix[1] = glm::scale(transformMatrix[1], glm::vec3(1.25, 1.0, 1.25));

	rotateMatrix[1] = glm::rotate(rotateMatrix[1], (GLfloat)glm::radians(cranebodyR), glm::vec3(0.0, 1.0, 0.0));

	// 크레인 머리

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[1]));

	rotateLocation = glGetUniformLocation(s_program[1], "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(rotateMatrix[1]));

	objColorLocation = glGetUniformLocation(s_program[1], "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 1.0, 0.0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 왼쪽 팔
	transformMatrix[2] = glm::translate(transformMatrix[2], glm::vec3(0.0, 0.0, craneZ));
	transformMatrix[2] = glm::rotate(transformMatrix[2], (GLfloat)glm::radians(cranebodyR), glm::vec3(0.0, 1.0, 0.0));
	transformMatrix[2] = glm::translate(transformMatrix[2], glm::vec3(-0.5 * BOXSIZE, 3.0 * BOXSIZE, 0.0));
	transformMatrix[2] = glm::rotate(transformMatrix[2], (GLfloat)glm::radians(armR), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix[2] = glm::translate(transformMatrix[2], glm::vec3(0, 1.5 * BOXSIZE, 0));
	transformMatrix[2] = glm::scale(transformMatrix[2], glm::vec3(0.25, 1.5, 0.25));

	rotateMatrix[2] = glm::rotate(rotateMatrix[2], (GLfloat)glm::radians(cranebodyR), glm::vec3(0.0, 1.0, 0.0));
	rotateMatrix[2] = glm::rotate(rotateMatrix[2], (GLfloat)glm::radians(armR), glm::vec3(1.0, 0.0, 0.0));

	// 크레인 왼쪽 팔

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[2]));

	rotateLocation = glGetUniformLocation(s_program[1], "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(rotateMatrix[2]));

	objColorLocation = glGetUniformLocation(s_program[1], "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 0.0, 1.0, 1.0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 오른쪽 팔
	transformMatrix[3] = glm::translate(transformMatrix[3], glm::vec3(0.0, 0.0, craneZ));
	transformMatrix[3] = glm::rotate(transformMatrix[3], (GLfloat)glm::radians(cranebodyR), glm::vec3(0.0, 1.0, 0.0));
	transformMatrix[3] = glm::translate(transformMatrix[3], glm::vec3(0.5 * BOXSIZE, 3.0 * BOXSIZE, 0.0));
	transformMatrix[3] = glm::rotate(transformMatrix[3], (GLfloat)glm::radians(-armR), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix[3] = glm::translate(transformMatrix[3], glm::vec3(0, 1.5 * BOXSIZE, 0));
	transformMatrix[3] = glm::scale(transformMatrix[3], glm::vec3(0.25, 1.5, 0.25));

	rotateMatrix[3] = glm::rotate(rotateMatrix[3], (GLfloat)glm::radians(cranebodyR), glm::vec3(0.0, 1.0, 0.0));
	rotateMatrix[3] = glm::rotate(rotateMatrix[3], (GLfloat)glm::radians(-armR), glm::vec3(1.0, 0.0, 0.0));

	// 크레인 오른쪽 팔

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[3]));

	rotateLocation = glGetUniformLocation(s_program[1], "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(rotateMatrix[3]));

	objColorLocation = glGetUniformLocation(s_program[1], "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 0.0, 1.0, 1.0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 빛
	lightMatrix = glm::translate(lightMatrix, glm::vec3(tmpX, lightY, tmpZ));
	lightMatrix = glm::rotate(lightMatrix, (GLfloat)glm::radians(Lradius), glm::vec3(0.0, 1.0, 0.0));
	lightMatrix = glm::scale(lightMatrix, glm::vec3(0.5, 0.5, 0.5));

	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(lightMatrix));

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(lightMatrix));

	objColorLocation = glGetUniformLocation(s_program[1], "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 0.8, 0.8, 0.8);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void InitVertices()
{
	for (int i = 0; i < 12; ++i)
	{
		vertices[18 * i] = Dots[Shapeindex[3 * i]][0];
		vertices[18 * i + 1] = Dots[Shapeindex[3 * i]][1];
		vertices[18 * i + 2] = Dots[Shapeindex[3 * i]][2];
		vertices[18 * i + 3] = normal[normalindex[3 * i]][0];
		vertices[18 * i + 4] = normal[normalindex[3 * i]][1];
		vertices[18 * i + 5] = normal[normalindex[3 * i]][2];

		vertices[18 * i + 6] = Dots[Shapeindex[3 * i + 1]][0];
		vertices[18 * i + 7] = Dots[Shapeindex[3 * i + 1]][1];
		vertices[18 * i + 8] = Dots[Shapeindex[3 * i + 1]][2];
		vertices[18 * i + 9] = normal[normalindex[3 * i + 1]][0];
		vertices[18 * i + 10] = normal[normalindex[3 * i + 1]][1];
		vertices[18 * i + 11] = normal[normalindex[3 * i + 1]][2];

		vertices[18 * i + 12] = Dots[Shapeindex[3 * i + 2]][0];
		vertices[18 * i + 13] = Dots[Shapeindex[3 * i + 2]][1];
		vertices[18 * i + 14] = Dots[Shapeindex[3 * i + 2]][2];
		vertices[18 * i + 15] = normal[normalindex[3 * i + 2]][0];
		vertices[18 * i + 16] = normal[normalindex[3 * i + 2]][1];
		vertices[18 * i + 17] = normal[normalindex[3 * i + 2]][2];
	}
}