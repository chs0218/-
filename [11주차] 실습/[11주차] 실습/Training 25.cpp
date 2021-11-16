// Reading Obj file
#define  _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "objRead.cpp"

using namespace std;


GLuint g_window_w = 1400;
GLuint g_window_h = 900;
const double PI = 3.141592;

GLuint VAO[2];
GLuint VBO;
GLuint VBO_position;
GLuint VBO_normal;

int polygon_mode = 2;

const int num_vertices = 3;
const int num_triangles = 1;

GLfloat OrbitDots[360][3];
GLfloat lightX = 0.0f, lightY = 0.0f, lightZ = 8.0f, lightColor[3] = { 1.0f, 1.0f, 1.0f };
GLfloat Lradius = 0.0f, OrbitR = 0.0f, Staellite = 0.0f;
bool RotateMinus = false, RotateL = false, RotateO = false, RotateS = false;
void Display();
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void TimerFunc(int value);
void InitBuffer();
void InitDots();

//--- load obj related variabales
float* sphere_object;
int num_Triangle;
float sunSize;


int main(int argc, char** argv)
{
	// create window using freeglut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_window_w, g_window_h);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Training 25");

	//////////////////////////////////////////////////////////////////////////////////////
	//// initialize GLEW
	//////////////////////////////////////////////////////////////////////////////////////
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//// Create shader program an register the shader
	//////////////////////////////////////////////////////////////////////////////////////
	InitDots();

	GLuint vShader;
	GLuint fShader;

	vShader = MakeVertexShader("vertex_object.glsl", 0);
	fShader = MakeFragmentShader("fragment_object.glsl", 0);

	// shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vShader);
	glAttachShader(s_program, fShader);
	glLinkProgram(s_program);
	checkCompileErrors(s_program, "PROGRAM");

	InitBuffer();

	// callback functions
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(25, TimerFunc, 1);

	// freeglut 윈도우 이벤트 처리 시작. 윈도우가 닫힐때까지 후한루프 실행.
	glutMainLoop();

	return 0;
}


void InitBuffer()
{
	num_Triangle = loadObj_normalize_center("sphere.obj");

	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(2, VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO_position);
	glGenBuffers(1, &VBO_normal);

	// 2 triangles for quad floor
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, outvertex.size() * sizeof(glm::vec3), &outvertex[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(s_program, "in_Position");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, outnormal.size() * sizeof(glm::vec3), &outnormal[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(s_program, "vNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);


	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(OrbitDots), OrbitDots, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);
}


void Display()
{
	glm::mat4 transformMatrix[3] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };
	glm::mat4 OrbitMatrix = glm::mat4(1.0f);
	glm::mat4 OrbitRotateMatrix = glm::mat4(1.0f);
	glm::mat4 lightMatrix = glm::mat4(1.0f);
	glm::mat4 rotateMatrix[3] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };
	glm::mat4 view = glm::mat4(1.0f);
	glm::vec3 cameraPos = glm::vec3(0.0f, 4.0f, 8.0f); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 projection = glm::mat4(1.0f);

	GLfloat tmpX = lightZ * glm::sin(glm::radians(Lradius)) + lightX * glm::cos(glm::radians(Lradius));
	GLfloat tmpZ = lightZ * glm::cos(glm::radians(Lradius)) - lightX * glm::sin(glm::radians(Lradius));

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;
	unsigned int rotateLocation;
	int lightPosLocation;
	int lightColorLocation;
	int objColorLocation;
	int viewPosition;

	projection = glm::perspective(glm::radians(65.0f), (float)g_window_w / (float)g_window_h, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	rotateMatrix[1] = glm::rotate(rotateMatrix[1], (GLfloat)glm::radians(OrbitR), glm::vec3(0.0, 1.0, 0.0));

	transformMatrix[1] = glm::rotate(transformMatrix[1], (GLfloat)glm::radians(OrbitR), glm::vec3(0.0, 1.0, 0.0));
	transformMatrix[1] = glm::translate(transformMatrix[1], glm::vec3(-4.0, 0.0, 0.0));
	transformMatrix[1] = glm::scale(transformMatrix[1], glm::vec3(0.7, 0.7, 0.7));

	rotateMatrix[2] = glm::rotate(rotateMatrix[2], (GLfloat)glm::radians(OrbitR), glm::vec3(0.0, 1.0, 0.0));
	rotateMatrix[2] = glm::rotate(rotateMatrix[2], (GLfloat)glm::radians(Staellite), glm::vec3(0.0, 1.0, 0.0));

	transformMatrix[2] = glm::rotate(transformMatrix[2], (GLfloat)glm::radians(OrbitR), glm::vec3(0.0, 1.0, 0.0));
	transformMatrix[2] = glm::translate(transformMatrix[2], glm::vec3(-4.0, 0.0, 0.0));
	transformMatrix[2] = glm::rotate(transformMatrix[2], (GLfloat)glm::radians(Staellite), glm::vec3(0.0, 1.0, 0.0));
	transformMatrix[2] = glm::translate(transformMatrix[2], glm::vec3(-1.5, 0.0, 0.0));
	transformMatrix[2] = glm::scale(transformMatrix[2], glm::vec3(0.4, 0.4, 0.4));
	//*************************************************************************
	// Drawing circle

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUseProgram(s_program);
	glBindVertexArray(VAO[0]);

	viewLocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	glUniform3f(lightPosLocation, tmpX, 0.0, tmpZ);

	lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2]);

	viewPosition = glGetUniformLocation(s_program, "viewPos");	//--- 카메라 위치
	glUniform3f(viewPosition, 0.0, 0.0, 4.0);

	// 빨간색 행성
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[0]));

	rotateLocation = glGetUniformLocation(s_program, "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(rotateMatrix[0]));

	objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 0.85, 0.26, 0.33);

	glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

	// 초록색 행성
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[1]));

	rotateLocation = glGetUniformLocation(s_program, "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(rotateMatrix[1]));

	objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 0.23, 0.70, 0.44);

	glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

	// 파란색 행성
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[2]));

	rotateLocation = glGetUniformLocation(s_program, "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(rotateMatrix[2]));

	objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 0.29, 0.54, 0.86);

	glDrawArrays(GL_TRIANGLES, 0, num_Triangle);


	lightMatrix = glm::translate(lightMatrix, glm::vec3(tmpX, lightY, tmpZ));
	lightMatrix = glm::scale(lightMatrix, glm::vec3(0.2, 0.2, 0.2));


	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(lightMatrix));

	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(lightMatrix));

	objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 0.8, 0.8, 0.8);

	glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

	// 궤도 생성

	glBindVertexArray(VAO[1]);
	OrbitMatrix = glm::scale(OrbitMatrix, glm::vec3(4.0, 1.0, 4.0));
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(OrbitMatrix));

	rotateLocation = glGetUniformLocation(s_program, "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(OrbitRotateMatrix));

	objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	glDrawArrays(GL_LINES, 0, 360);
	glDrawArrays(GL_LINES, 1, 359);

	// 궤도2 생성

	OrbitMatrix = glm::mat4(1.0);
	OrbitMatrix = glm::rotate(OrbitMatrix, (GLfloat)glm::radians(OrbitR), glm::vec3(0.0, 1.0, 0.0));
	OrbitMatrix = glm::translate(OrbitMatrix, glm::vec3(-4.0, 0.0, 0.0));
	OrbitMatrix = glm::scale(OrbitMatrix, glm::vec3(1.5, 1.0, 1.5));
	OrbitRotateMatrix = glm::rotate(OrbitRotateMatrix, (GLfloat)glm::radians(OrbitR), glm::vec3(0.0, 1.0, 0.0));
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(OrbitMatrix));

	rotateLocation = glGetUniformLocation(s_program, "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(OrbitRotateMatrix));

	objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	glDrawArrays(GL_LINES, 0, 360);
	glDrawArrays(GL_LINES, 1, 359);
	glutSwapBuffers();
}


void Reshape(int w, int h)
{
	g_window_w = w;
	g_window_h = h;
	glViewport(0, 0, w, h);
}

void TimerFunc(int value)
{
	if (RotateL)
	{
		if (RotateMinus)
			Lradius = (GLfloat)((int)(Lradius - 1.0f) % 360);
		else
			Lradius = (GLfloat)((int)(Lradius + 1.0f) % 360);
	}

	if (RotateO)
	{
		OrbitR = (GLfloat)((int)(OrbitR + 2.0f) % 360);
	}

	if (RotateS)
	{
		Staellite = (GLfloat)((int)(Staellite + 3.0f) % 360);
	}

	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'R':
		if (!RotateMinus)
		{
			RotateMinus = true;
			RotateL = true;
		}
		else
			RotateL = !RotateL;
		break;
	case 'r':
		if (RotateMinus)
		{
			RotateMinus = false;
			RotateL = true;
		}
		else
			RotateL = !RotateL;
		break;
	case 'A':
	case 'a':
		RotateO = !RotateO;
		break;
	case 'S':
	case 's':
		RotateS = !RotateS;
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
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void InitDots()
{
	int iDegree = 0;
	for (int i = 0; i < 360; ++i)
	{
		OrbitDots[i][0] = 1.0 * (GLfloat)cos(iDegree * PI / 180.0f);
		OrbitDots[i][2] = 1.0 * (GLfloat)sin(iDegree * PI / 180.0f);
		iDegree = (iDegree + 1) % 360;
	}
}


