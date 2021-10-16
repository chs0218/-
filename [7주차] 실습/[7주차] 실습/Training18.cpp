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

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader[2], fragmentShader[4];
GLuint s_program[4];
GLuint vao, vbo[2], EBO[2];
GLint width, height;


void DrawMain();
void InitDots();
void DrawSphere();

void KeyBoard(unsigned char key, int x, int y);
void TimerFunc(int value);

GLfloat Dots[360][3];
GLfloat Colors[360][3];
unsigned int Shapeindex[720];

GLfloat fRadius = 0.7, transX = 0.0, transY = 0.0, transZ = 0.0, Rradius = 0.0;
int iDegree[6] = { 180 , 180, 180, 180, 180, 180};

bool Perspective = false, ShowWire = false, RotateR = false;
const double PI = 3.141592;

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	vertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[0], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[0]);

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

	fragmentsource = filetobuf("fragment_planet_red.glsl");
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[1]);

	fragmentsource = filetobuf("fragment_planet_green.glsl");
	fragmentShader[2] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[2], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[2]);

	fragmentsource = filetobuf("fragment_planet_blue.glsl");
	fragmentShader[3] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[3], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[3]);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo);
	glGenBuffers(2, EBO);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Shapeindex), Shapeindex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
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

	s_program[3] = glCreateProgram();
	glAttachShader(s_program[3], vertexShader[1]);
	glAttachShader(s_program[3], fragmentShader[3]);
	glLinkProgram(s_program[3]);

	glDeleteShader(vertexShader[0]);
	glDeleteShader(vertexShader[1]);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);
	glDeleteShader(fragmentShader[2]);
	glDeleteShader(fragmentShader[3]);
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
	InitDots();

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training18");

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
	case 'w':
		if (transY < 0.5f)
			transY += 0.05f;
		break;
	case 's':
		if (transY > -0.5f)
			transY -= 0.05f;
		break;
	case 'a':
		if (transX > -0.5f)
			transX -= 0.05f;
		break;
	case 'd':
		if (transX < 0.5f)
			transX += 0.05f;
		break;
	case 'z':
		if (transZ > -0.5f)
			transZ -= 0.05f;
		break;
	case 'x':
		if (transZ < 0.5f)
			transZ += 0.05f;
		break;
	case 'y':
		RotateR = !RotateR;
		break;
	case 'm':
		ShowWire = !ShowWire;
		break;
	case 'p':
		Perspective = !Perspective;
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
		Rradius = (GLfloat)((int)(Rradius + 5.0) % 360);
	for (int i = 0; i < 6; ++i)
	{
		iDegree[i] = (iDegree[i] + (i + 2)) % 360;
	}
	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void DrawSphere()
{
	GLUquadricObj* qobj;

	qobj = gluNewQuadric();
	if (ShowWire)
		gluQuadricDrawStyle(qobj, GLU_LINE);
	else
		gluQuadricDrawStyle(qobj, GLU_FILL);

	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricOrientation(qobj, GLU_OUTSIDE);
	gluSphere(qobj, 0.3, 25, 25);
}

void DrawMain()
{
	glm::mat4 transAll = glm::mat4(1.0f);
	glm::mat4 rotateR = glm::mat4(1.0f);
	glm::mat4 transformMatrix[4] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.3f, 1.0f); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 halfScale = glm::mat4(1.0f);

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;

	halfScale = glm::scale(halfScale, glm::vec3(0.5, 0.5, 0.5));
	transAll = glm::translate(transAll, glm::vec3(transX, transY, transZ));
	rotateR = glm::rotate(rotateR, (GLfloat)glm::radians(Rradius), glm::vec3(0.0, 1.0, 0.0));

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	if (Perspective)
	{
		projection = glm::perspective(glm::radians(130.0f), (float)width / (float)height, 0.1f, 50.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -0.5));
	}

	else
	{
		projection = glm::ortho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	}

	transformMatrix[0] = rotateR * transAll * transformMatrix[0];

	// xz 평면 transformMatrix
	transformMatrix[1] = glm::translate(transformMatrix[1], glm::vec3(fRadius * (GLfloat)cos(iDegree[0] * PI / 180.0f), 0.0, fRadius * (GLfloat)sin(iDegree[0] * PI / 180.0f)));
	transformMatrix[1] = glm::scale(transformMatrix[1], glm::vec3(0.5, 0.5, 0.5));
	transformMatrix[1] = rotateR * transAll * transformMatrix[1];

	// xz 평면 시계방향 45 transformMatrix
	transformMatrix[2] = glm::rotate(transformMatrix[2], (GLfloat)glm::radians(45.0), glm::vec3(0.0, 0.0, 1.0));
	transformMatrix[2] = glm::translate(transformMatrix[2], glm::vec3(fRadius * (GLfloat)cos(iDegree[1] * PI / 180.0f), 0.0, fRadius * (GLfloat)sin(iDegree[1] * PI / 180.0f)));
	transformMatrix[2] = glm::rotate(transformMatrix[2], (GLfloat)glm::radians(-45.0), glm::vec3(0.0, 0.0, 1.0));
	transformMatrix[2] = glm::scale(transformMatrix[2], glm::vec3(0.5, 0.5, 0.5));
	transformMatrix[2] = rotateR * transAll * transformMatrix[2] * rotateR;

	// xz 평면 반시계방향 45 transformMatrix
	transformMatrix[3] = glm::rotate(transformMatrix[3], (GLfloat)glm::radians(135.0), glm::vec3(0.0, 0.0, 1.0));
	transformMatrix[3] = glm::translate(transformMatrix[3], glm::vec3(fRadius * (GLfloat)cos(iDegree[2] * PI / 180.0f), 0.0, fRadius * (GLfloat)sin(iDegree[2] * PI / 180.0f)));
	transformMatrix[3] = glm::rotate(transformMatrix[3], (GLfloat)glm::radians(-135.0), glm::vec3(0.0, 0.0, 1.0));
	transformMatrix[3] = glm::scale(transformMatrix[3], glm::vec3(0.5, 0.5, 0.5));
	transformMatrix[3] = rotateR * transAll * transformMatrix[3] * rotateR;

	// 고리 생성
	glUseProgram(s_program[0]);
	glBindVertexArray(vao);

	viewLocation = glGetUniformLocation(s_program[0], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	// 중심 행성 고리
	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[0]));

	glDrawElements(GL_LINES, 720, GL_UNSIGNED_INT, 0);

	transformMatrix[0] = glm::rotate(transformMatrix[0], (GLfloat)glm::radians(45.0), glm::vec3(0.0, 0.0, 1.0));
	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[0]));

	glDrawElements(GL_LINES, 720, GL_UNSIGNED_INT, 0);

	transformMatrix[0] = glm::rotate(transformMatrix[0], (GLfloat)glm::radians(90.0), glm::vec3(0.0, 0.0, 1.0));
	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[0]));

	glDrawElements(GL_LINES, 720, GL_UNSIGNED_INT, 0);


	// xz 평면 행성 고리
	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[1]));

	glDrawElements(GL_LINES, 720, GL_UNSIGNED_INT, 0);


	// xz 평면 시계방향 45 행성 고리

	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[2]));

	glDrawElements(GL_LINES, 720, GL_UNSIGNED_INT, 0);

	// xz 평면 반시계방향 45 행성 고리

	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[3]));

	glDrawElements(GL_LINES, 720, GL_UNSIGNED_INT, 0);


	// 중심 항성 생성

	glUseProgram(s_program[1]);

	viewLocation = glGetUniformLocation(s_program[1], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[1], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[0]));

	DrawSphere();

	// 행성 생성
	glUseProgram(s_program[2]);

	viewLocation = glGetUniformLocation(s_program[2], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[2], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	// xz 평면 행성
	modelLocation = glGetUniformLocation(s_program[2], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[1]));

	DrawSphere();

	// xz 평면 시계방향 45 행성

	modelLocation = glGetUniformLocation(s_program[2], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[2]));

	DrawSphere();

	// xz 평면 반시계방향 45 행성

	modelLocation = glGetUniformLocation(s_program[2], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[3]));

	DrawSphere();

	// 위성 생성
	glUseProgram(s_program[3]);

	viewLocation = glGetUniformLocation(s_program[3], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[3], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	// xz 평면 위성
	transformMatrix[1] = glm::translate(transformMatrix[1], glm::vec3(fRadius * (GLfloat)cos(iDegree[3] * PI / 180.0f), 0.0, fRadius * (GLfloat)sin(iDegree[3] * PI / 180.0f)));
	transformMatrix[1] = transformMatrix[1] * halfScale;
	modelLocation = glGetUniformLocation(s_program[3], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[1]));

	DrawSphere();

	// xz 평면 시계방향 45 위성
	transformMatrix[2] = glm::translate(transformMatrix[2], glm::vec3(fRadius * (GLfloat)cos(iDegree[4] * PI / 180.0f), 0.0, fRadius * (GLfloat)sin(iDegree[4] * PI / 180.0f)));
	transformMatrix[2] = transformMatrix[2] * halfScale;
	modelLocation = glGetUniformLocation(s_program[3], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[2]));

	DrawSphere();

	// xz 평면 반시계방향 45 위성
	transformMatrix[3] = glm::translate(transformMatrix[3], glm::vec3(fRadius * (GLfloat)cos(iDegree[5] * PI / 180.0f), 0.0, fRadius * (GLfloat)sin(iDegree[5] * PI / 180.0f)));
	transformMatrix[3] = transformMatrix[3] * halfScale;
	modelLocation = glGetUniformLocation(s_program[3], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix[3]));

	DrawSphere();

}

void InitDots()
{
	for (int i = 0; i < 360; ++i)
	{
		Dots[i][0] = fRadius * (GLfloat)cos(iDegree[0] * PI / 180.0f);
		Dots[i][2] = fRadius * (GLfloat)sin(iDegree[0] * PI / 180.0f);
		Colors[i][0] = 1.0f;
		Colors[i][1] = 1.0f;
		Colors[i][2] = 1.0f;
		iDegree[0] = (iDegree[0] + 1) % 360;
	}

	for (int i = 0; i < 359; ++i)
	{
		Shapeindex[2 * i] = i;
		Shapeindex[2 * i + 1] = i + 1;
	}
	Shapeindex[718] = 359;
	Shapeindex[719] = 0;
}