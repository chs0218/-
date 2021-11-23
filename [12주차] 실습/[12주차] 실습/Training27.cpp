// Reading Obj file
#define  _CRT_SECURE_NO_WARNINGS

#include "objRead.h"
#include "shape.h"
#include "objRead.cpp"
#include "shape.cpp"

using namespace std;

ObjRead objread;

GLuint g_window_w = 1400;
GLuint g_window_h = 900;

GLuint VAO[3];
GLuint VBO[2];
GLuint VBO_position;
GLuint VBO_normal;

int polygon_mode = 2;
int level = 0;

const int num_vertices = 3;
const int num_triangles = 1;

GLfloat lightX = 0.0f, lightY = 3.0f, lightZ = 8.0f, lightColor[3] = { 1.0f, 1.0f, 1.0f };
GLfloat Lradius = 0.0f, OrbitM = 0.0f, OrbitR = 0.0f, OrbitS = 0.0f, ambientlight = 0.3f;
bool RotateMinus = false, RotateL = false, RotateM = false, RotateO = false, RotateS = false, FallSnow = false;
void Display();
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void TimerFunc(int value);
void DrawCircle(glm::mat4 view, glm::mat4 projection, GLfloat ResultX, GLfloat ResultZ);
void DrawShapes(glm::mat4 view, glm::mat4 projection, GLfloat ResultX, GLfloat ResultZ);
void Drawlight(glm::mat4 view, glm::mat4 projection, GLfloat ResultX, GLfloat ResultZ);
void DrawSnow(glm::mat4 view, glm::mat4 projection, GLfloat ResultX, GLfloat ResultZ);
void InitBuffer();

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

	glutCreateWindow("Training 27");

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
	InitVertices();

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
	num_Triangle = objread.loadObj_normalize_center("sphere.obj");

	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(3, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &VBO_position);
	glGenBuffers(1, &VBO_normal);
	glGenVertexArrays(1, &SierpinskiVAO);
	glGenBuffers(1, &SierpinskiVBO);

	// 2 triangles for quad floor
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, objread.outvertex.size() * sizeof(glm::vec3), &objread.outvertex[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(s_program, "in_Position");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, objread.outnormal.size() * sizeof(glm::vec3), &objread.outnormal[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(s_program, "vNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);


	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(OrbitDots), OrbitDots, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shapevertices), shapevertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);
}


void Display()
{
	glm::vec3 cameraPos = glm::vec3(0.0f, 6.0f, 8.0f); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	GLfloat ResultX = lightZ * glm::sin(glm::radians(Lradius)) + lightX * glm::cos(glm::radians(Lradius));
	GLfloat ResultZ = lightZ * glm::cos(glm::radians(Lradius)) - lightX * glm::sin(glm::radians(Lradius));

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	projection = glm::perspective(glm::radians(65.0f), (float)g_window_w / (float)g_window_h, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(s_program);
	DrawCircle(view, projection, ResultX, ResultZ);
	DrawShapes(view, projection, ResultX, ResultZ);
	Drawlight(view, projection, ResultX, ResultZ);
	if(FallSnow)
		DrawSnow(view, projection, ResultX, ResultZ);
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

	if (RotateM)
	{
		OrbitM = (GLfloat)((int)(OrbitM + 2.0f) % 360);
	}

	if (RotateO)
	{
		OrbitR = (GLfloat)((int)(OrbitR + 2.0f) % 360);
	}

	if (RotateS)
	{
		OrbitS = (GLfloat)((int)(OrbitS + 3.0f) % 360);
	}

	for(int i = 0; i<SnowNum; ++i)
		snows[i].falling();
	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '0':
		level = 0;
		break;
	case '1':
		level = 1;
		break;
	case '2':
		level = 2;
		break;
	case '3':
		level = 3;
		break;
	case '4':
		level = 4;
		break;
	case '5':
		level = 5;
		break;
	case '6':
		level = 6;
		break;
	case '7':
		level = 7;
		break;
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
	case 'W':
	case 'w':
		RotateS = !RotateS;
		break;
	case 'S':
	case 's':
		FallSnow = !FallSnow;
		break;
	case 'D':
	case 'd':
		RotateM = !RotateM;
		break;
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	case 'i':
		if (lightZ < 10.0)
			lightZ += 0.2f;
		break;
	case 'o':
		if (lightZ > 0.6)
			lightZ -= 0.2f;
		break;
	case '+':
		if (ambientlight < 1.0)
			ambientlight += 0.1f;
		std::cout << ambientlight << std::endl;
		break;
	case '-':
		if (ambientlight > 0.1)
			ambientlight -= 0.1f;
		std::cout << ambientlight << std::endl;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void DrawCircle(glm::mat4 view, glm::mat4 projection, GLfloat ResultX, GLfloat ResultZ)
{
	glm::mat4 transformMatrix[3] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };
	glm::mat4 OrbitMatrix = glm::mat4(1.0f);
	glm::mat4 OrbitRotateMatrix = glm::mat4(1.0f);
	glm::mat4 rotateMatrix[3] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;
	unsigned int rotateLocation;
	int ambientLocation;
	int lightPosLocation;
	int lightColorLocation;
	int objColorLocation;
	int viewPosition;

	rotateMatrix[0] = glm::rotate(rotateMatrix[0], (GLfloat)glm::radians(60.0), glm::vec3(1.0, 0.0, 1.0));
	rotateMatrix[0] = glm::rotate(rotateMatrix[0], (GLfloat)glm::radians(OrbitM), glm::vec3(0.0, 1.0, 0.0));

	transformMatrix[0] = glm::translate(transformMatrix[0], glm::vec3(0.0, 3.0, 0.0));
	transformMatrix[0] = glm::rotate(transformMatrix[0], (GLfloat)glm::radians(60.0), glm::vec3(1.0, 0.0, 1.0));
	transformMatrix[0] = glm::rotate(transformMatrix[0], (GLfloat)glm::radians(OrbitM), glm::vec3(0.0, 1.0, 0.0));
	transformMatrix[0] = glm::translate(transformMatrix[0], glm::vec3(-4.0, 0.0, 0.0));
	transformMatrix[0] = glm::scale(transformMatrix[0], glm::vec3(0.7, 0.7, 0.7));

	
	rotateMatrix[1] = glm::rotate(rotateMatrix[1], (GLfloat)glm::radians(60.0), glm::vec3(1.0, 1.0, 0.0));
	rotateMatrix[1] = glm::rotate(rotateMatrix[1], (GLfloat)glm::radians(OrbitR), glm::vec3(0.0, 1.0, 0.0));

	transformMatrix[1] = glm::translate(transformMatrix[1], glm::vec3(0.0, 3.0, 0.0));
	transformMatrix[1] = glm::rotate(transformMatrix[1], (GLfloat)glm::radians(60.0), glm::vec3(1.0, 1.0, 0.0));
	transformMatrix[1] = glm::rotate(transformMatrix[1], (GLfloat)glm::radians(OrbitR), glm::vec3(0.0, 1.0, 0.0));
	transformMatrix[1] = glm::translate(transformMatrix[1], glm::vec3(-4.0, 0.0, 0.0));
	transformMatrix[1] = glm::scale(transformMatrix[1], glm::vec3(0.7, 0.7, 0.7));

	rotateMatrix[2] = glm::rotate(rotateMatrix[2], (GLfloat)glm::radians(60.0), glm::vec3(0.0, 1.0, 1.0));
	rotateMatrix[2] = glm::rotate(rotateMatrix[2], (GLfloat)glm::radians(OrbitS), glm::vec3(0.0, 1.0, 0.0));

	transformMatrix[2] = glm::translate(transformMatrix[2], glm::vec3(0.0, 3.0, 0.0));
	transformMatrix[2] = glm::rotate(transformMatrix[2], (GLfloat)glm::radians(60.0), glm::vec3(0.0, 1.0, 1.0));
	transformMatrix[2] = glm::rotate(transformMatrix[2], (GLfloat)glm::radians(OrbitS), glm::vec3(0.0, 1.0, 0.0));
	transformMatrix[2] = glm::translate(transformMatrix[2], glm::vec3(-4.0, 0.0, 0.0));
	transformMatrix[2] = glm::scale(transformMatrix[2], glm::vec3(0.7, 0.7, 0.7));
	//*************************************************************************
	// Drawing circle
	glBindVertexArray(VAO[0]);

	viewLocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	glUniform3f(lightPosLocation, ResultX, lightY, ResultZ);

	ambientLocation = glGetUniformLocation(s_program, "ambient");
	glUniform1f(ambientLocation, ambientlight);

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
	// 궤도 생성
	glBindVertexArray(VAO[1]);
	OrbitMatrix = glm::mat4(1.0);
	OrbitMatrix = glm::translate(OrbitMatrix, glm::vec3(0.0, 3.0, 0.0));
	OrbitMatrix = glm::rotate(OrbitMatrix, (GLfloat)glm::radians(60.0), glm::vec3(1.0, 0.0, 1.0));
	OrbitMatrix = glm::scale(OrbitMatrix, glm::vec3(4.0, 1.0, 4.0));
	OrbitRotateMatrix = glm::rotate(OrbitRotateMatrix, (GLfloat)glm::radians(60.0), glm::vec3(1.0, 0.0, 1.0));

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
	OrbitMatrix = glm::translate(OrbitMatrix, glm::vec3(0.0, 3.0, 0.0));
	OrbitMatrix = glm::rotate(OrbitMatrix, (GLfloat)glm::radians(60.0), glm::vec3(1.0, 1.0, 0.0));
	OrbitMatrix = glm::scale(OrbitMatrix, glm::vec3(4.0, 1.0, 4.0));
	OrbitRotateMatrix = glm::rotate(OrbitRotateMatrix, (GLfloat)glm::radians(60.0), glm::vec3(1.0, 1.0, 0.0));

	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(OrbitMatrix));

	rotateLocation = glGetUniformLocation(s_program, "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(OrbitRotateMatrix));

	objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	glDrawArrays(GL_LINES, 0, 360);
	glDrawArrays(GL_LINES, 1, 359);

	// 궤도3 생성
	OrbitMatrix = glm::mat4(1.0);
	OrbitMatrix = glm::translate(OrbitMatrix, glm::vec3(0.0, 3.0, 0.0));
	OrbitMatrix = glm::rotate(OrbitMatrix, (GLfloat)glm::radians(60.0), glm::vec3(0.0, 1.0, 1.0));
	OrbitMatrix = glm::scale(OrbitMatrix, glm::vec3(4.0, 1.0, 4.0));
	OrbitRotateMatrix = glm::rotate(OrbitRotateMatrix, (GLfloat)glm::radians(60.0), glm::vec3(0.0, 1.0, 1.0));

	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(OrbitMatrix));

	rotateLocation = glGetUniformLocation(s_program, "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(OrbitRotateMatrix));

	objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	glDrawArrays(GL_LINES, 0, 360);
	glDrawArrays(GL_LINES, 1, 359);
}


void DrawShapes(glm::mat4 view, glm::mat4 projection, GLfloat ResultX, GLfloat ResultZ)
{
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::mat4 floorMatrix = glm::mat4(1.0f);
	glm::mat4 rotateMatrix = glm::mat4(1.0f);
	glm::mat4 tmp = glm::mat4(1.0f);

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;
	unsigned int rotateLocation;
	int ambientLocation;
	int lightPosLocation;
	int lightColorLocation;
	int objColorLocation;
	int viewPosition;

	viewLocation = glGetUniformLocation(s_program, "viewTransform");
	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	viewPosition = glGetUniformLocation(s_program, "viewPos");	//--- 카메라 위치
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	rotateLocation = glGetUniformLocation(s_program, "rotateTransform");
	objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	ambientLocation = glGetUniformLocation(s_program, "ambient");
	//*************************************************************************
	// Drawing circle
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	glUniform3f(lightPosLocation, ResultX, lightY, ResultZ);
	glUniform3f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2]);
	glUniform3f(viewPosition, 0.0, 0.0, 4.0);
	glUniform1f(ambientLocation, ambientlight);

	// 도형
	transformMatrix = glm::scale(transformMatrix, glm::vec3(2.0, 2.0, 2.0));
	transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0, BOXSIZE, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(rotateMatrix));
	glUniform3f(objColorLocation, 0.55, 0.0, 1.0);

	Sierpinski(Dots[Shapeindex[36]], Dots[Shapeindex[37]], Dots[Shapeindex[38]], normal[normalindex[36]], level);
	Sierpinski(Dots[Shapeindex[39]], Dots[Shapeindex[40]], Dots[Shapeindex[41]], normal[normalindex[39]], level);
	Sierpinski(Dots[Shapeindex[42]], Dots[Shapeindex[43]], Dots[Shapeindex[44]], normal[normalindex[42]], level);
	Sierpinski(Dots[Shapeindex[45]], Dots[Shapeindex[46]], Dots[Shapeindex[47]], normal[normalindex[45]], level);

	glBindVertexArray(VAO[2]);
	glDrawArrays(GL_TRIANGLES, 30, 6);

	// 바닥
	floorMatrix = glm::translate(floorMatrix, glm::vec3(0.0, -10.0 * BOXSIZE, 0.0));
	floorMatrix = glm::scale(floorMatrix, glm::vec3(10.0, 10.0, 10.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(floorMatrix));
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(floorMatrix));
	glUniform3f(objColorLocation, 0.8, 0.8, 0.8);
	glDrawArrays(GL_TRIANGLES, 24, 6);
}

void Drawlight(glm::mat4 view, glm::mat4 projection, GLfloat ResultX, GLfloat ResultZ)
{
	glm::mat4 lightMatrix = glm::mat4(1.0f);

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;
	unsigned int rotateLocation;
	int ambientLocation;
	int lightPosLocation;
	int lightColorLocation;
	int objColorLocation;
	int viewPosition;


	lightMatrix = glm::translate(lightMatrix, glm::vec3(ResultX, lightY, ResultZ));
	lightMatrix = glm::scale(lightMatrix, glm::vec3(0.2, 0.2, 0.2));
	//*************************************************************************
	// Drawing circle
	glBindVertexArray(VAO[0]);

	viewLocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	glUniform3f(lightPosLocation, ResultX, lightY, ResultZ);

	ambientLocation = glGetUniformLocation(s_program, "ambient");
	glUniform1f(ambientLocation, ambientlight);

	lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2]);

	viewPosition = glGetUniformLocation(s_program, "viewPos");	//--- 카메라 위치
	glUniform3f(viewPosition, 0.0, 0.0, 4.0);

	rotateLocation = glGetUniformLocation(s_program, "rotateTransform");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(lightMatrix));

	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(lightMatrix));

	objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 0.5, 0.5, 0.5);

	glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
}

void DrawSnow(glm::mat4 view, glm::mat4 projection, GLfloat ResultX, GLfloat ResultZ)
{
	glm::mat4 SnowMatrix = glm::mat4(1.0f);

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;
	unsigned int rotateLocation;
	int lightPosLocation;
	int lightColorLocation;
	int objColorLocation;
	int ambientLocation;
	int viewPosition;

	//*************************************************************************
	// Drawing circle
	glBindVertexArray(VAO[2]);

	viewLocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	glUniform3f(lightPosLocation, ResultX, lightY, ResultZ);

	ambientLocation = glGetUniformLocation(s_program, "ambient");
	glUniform1f(ambientLocation, ambientlight);

	lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2]);

	viewPosition = glGetUniformLocation(s_program, "viewPos");	//--- 카메라 위치
	glUniform3f(viewPosition, 0.0, 0.0, 4.0);

	for (int i = 0; i < SnowNum; ++i)
	{
		SnowMatrix = glm::mat4(1.0f);
		SnowMatrix = glm::translate(SnowMatrix, glm::vec3(snows[i].getSnowX(), snows[i].getSnowY(), snows[i].getSnowZ()));
		SnowMatrix = glm::scale(SnowMatrix, glm::vec3(0.1, 0.1, 0.1));

		rotateLocation = glGetUniformLocation(s_program, "rotateTransform");
		glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(SnowMatrix));

		modelLocation = glGetUniformLocation(s_program, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(SnowMatrix));

		objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}