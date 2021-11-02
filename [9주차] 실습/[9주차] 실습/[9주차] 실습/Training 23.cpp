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
GLuint vertexShader, fragmentShader[6];
GLuint s_program[6];
GLuint vao[3], vbo[3], EBO[3];
GLint width, height;

GLUquadricObj* qobj;
GLfloat SphereX[5], SphereY[5], SphereZ[5];
GLfloat cameraX = 0.0, cameraY = 0.0, cameraZ = 2.0;
GLfloat CameraDirX = 0.0, CameraDirY = 0.0, CameraDirZ = 0.0;
GLfloat BoxX[3], BoxY[3], BoxZ[3] = { -0.99 + BOXSIZE * 0.5f ,  -0.99 + BOXSIZE * 1.5 , -0.99 + BOXSIZE * 2.0 }, BoxSize[3] = { 0.5, 0.25, 0.125 }, BoxAccel[3], RRadius = 0.0f;
GLfloat floorR = 0.0f;
bool SphereMoveX[5], SphereMoveY[5], SphereMoveZ[5], RotateY = false;
bool Click = false, OpenFloor = false;
int mx;

glm::vec3 cameraPos = glm::vec3(cameraX, cameraY, cameraZ); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(CameraDirX, CameraDirY, CameraDirZ); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

void DrawMain();
void MoveSphere();
void MoveBox();
void RotateCameraCenterY();
void RandomSphere();
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void KeyBoard(unsigned char key, int x, int y);
void TimerFunc(int value);

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

unsigned int ShapeindexRHS[] = {
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

unsigned int ShapeindexLHS[] = {
	// 육면체
	0, 2, 1,
	0, 3, 2,
	4, 5, 6,
	4, 6, 7,
	3, 6, 2,
	3, 7, 6,
	4, 1, 5,
	4, 0, 1,
	4, 3, 0,
	4, 7, 3,
	1, 6, 5,
	1, 2, 6
};

unsigned int Lineindex[] = {
	// 육면체
	0, 1,
	0, 3,
	0, 4,
	1, 2,
	1, 5,
	2, 3,
	2, 6,
	3, 7,
	4, 5,
	4, 7,
	5, 6,
	6, 7,
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

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex_objects.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader);
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment_gray.glsl");
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[0], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[0]);

	fragmentsource = filetobuf("fragment_darkgray.glsl");
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[1]);

	fragmentsource = filetobuf("fragment_anothergray.glsl");
	fragmentShader[2] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[2], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[2]);

	fragmentsource = filetobuf("fragment_blue.glsl");
	fragmentShader[3] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[3], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[3]);

	fragmentsource = filetobuf("fragment_red.glsl");
	fragmentShader[4] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[4], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[4]);

	fragmentsource = filetobuf("fragment_black.glsl");
	fragmentShader[5] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[5], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[5]);
}

void InitBuffer()
{
	glGenVertexArrays(2, vao);
	glGenBuffers(3, vbo);
	glGenBuffers(3, EBO);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ShapeindexRHS), ShapeindexRHS, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ShapeindexLHS), ShapeindexLHS, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Lineindex), Lineindex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
}

void InitShader()
{
	make_vertexShaders();
	make_fragmentShaders();

	s_program[0] = glCreateProgram();

	glAttachShader(s_program[0], vertexShader);
	glAttachShader(s_program[0], fragmentShader[0]);
	glLinkProgram(s_program[0]);

	s_program[1] = glCreateProgram();

	glAttachShader(s_program[1], vertexShader);
	glAttachShader(s_program[1], fragmentShader[1]);
	glLinkProgram(s_program[1]);

	s_program[2] = glCreateProgram();

	glAttachShader(s_program[2], vertexShader);
	glAttachShader(s_program[2], fragmentShader[2]);
	glLinkProgram(s_program[2]);

	s_program[3] = glCreateProgram();

	glAttachShader(s_program[3], vertexShader);
	glAttachShader(s_program[3], fragmentShader[3]);
	glLinkProgram(s_program[3]);

	s_program[4] = glCreateProgram();

	glAttachShader(s_program[4], vertexShader);
	glAttachShader(s_program[4], fragmentShader[4]);
	glLinkProgram(s_program[4]);

	s_program[5] = glCreateProgram();

	glAttachShader(s_program[5], vertexShader);
	glAttachShader(s_program[5], fragmentShader[5]);
	glLinkProgram(s_program[5]);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);
	glDeleteShader(fragmentShader[2]);
	glDeleteShader(fragmentShader[3]);
	glDeleteShader(fragmentShader[4]);
	glDeleteShader(fragmentShader[5]);
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);*/
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	DrawMain();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
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
	RandomSphere();

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training22");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(KeyBoard);
	glutTimerFunc(25, TimerFunc, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void KeyBoard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'R':
	case 'r':
		RandomSphere();
		cameraX = 0.0, cameraY = 0.0, cameraZ = 2.0;
		CameraDirX = 0.0, CameraDirY = 0.0, CameraDirZ = 0.0;
		for (int i = 0; i < 3; ++i)
		{
			BoxX[i] = 0;
			BoxY[i] = 0;
			BoxAccel[i] = 0;
		}
		BoxZ[0] = -0.99 + BOXSIZE * 0.5f;
		BoxZ[1] = -0.99 + BOXSIZE * 1.5f;
		BoxZ[2] = -0.99 + BOXSIZE * 2.0f;
		RRadius = 0.0f;
		floorR = 0.0f;
		RotateY = false;
		Click = false, OpenFloor = false;
		break;
	case 'O':
	case 'o':
		OpenFloor = true;
		break;
	case 'Z':
		cameraZ += 0.1f;
		break;
	case 'z':
		cameraZ -= 0.1f;
		break;
	case 'Y':
	case 'y':
		RotateY = !RotateY;
		break;
	case 'B':
	case 'b':
		RandomSphere();
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
	if (RotateY)
		RotateCameraCenterY();
	if (OpenFloor)
	{
		if (floorR < 90.0)
			floorR += 5.0f;
		else
			floorR = 90.0;
	}
	MoveBox();
	MoveSphere();
	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void DrawMain()
{
	glm::mat4 RotateMatrix = glm::mat4(1.0f);
	glm::mat4 FloorMatrix = glm::mat4(1.0f);
	glm::mat4 SphereMatrix = glm::mat4(1.0f);
	glm::mat4 BoxMatrix[3] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;

	RotateMatrix = glm::rotate(RotateMatrix, (GLfloat)glm::radians(RRadius), glm::vec3(0.0, 0.0, 1.0));

	//projection = glm::ortho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
	projection = glm::perspective(glm::radians(130.0f), (float)width / (float)height, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -0.5));

	cameraPos = glm::vec3(cameraX, cameraY, cameraZ);
	cameraDirection = glm::vec3(CameraDirX, CameraDirY, CameraDirZ);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);


	transformMatrix = glm::scale(transformMatrix, glm::vec3(2.0, 2.0, 2.0));
	transformMatrix = RotateMatrix * transformMatrix;

	FloorMatrix = glm::translate(FloorMatrix, glm::vec3(0.0, 2.0 * BOXSIZE, -2.0 * BOXSIZE));
	FloorMatrix = glm::rotate(FloorMatrix, (GLfloat)glm::radians(floorR), glm::vec3(1.0, 0.0, 0.0));
	FloorMatrix = glm::translate(FloorMatrix, glm::vec3(0.0, -2.0 * BOXSIZE, 2.0 * BOXSIZE));
	FloorMatrix = FloorMatrix * transformMatrix;
	// 상자 생성
	glUseProgram(s_program[0]);

	viewLocation = glGetUniformLocation(s_program[0], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");

	glBindVertexArray(vao[1]);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(FloorMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glUseProgram(s_program[1]);

	viewLocation = glGetUniformLocation(s_program[1], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[1], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");

	glBindVertexArray(vao[1]);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)));

	glUseProgram(s_program[2]);

	viewLocation = glGetUniformLocation(s_program[2], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[2], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[2], "modelTransform");

	glBindVertexArray(vao[1]);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(24 * sizeof(GLuint)));

	// 공 생성
	glUseProgram(s_program[3]);
	glBindVertexArray(vao[0]);

	viewLocation = glGetUniformLocation(s_program[3], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[3], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[3], "modelTransform");

	for (int i = 0; i < 5; ++i)
	{
		SphereMatrix = glm::mat4(1.0f);
		SphereMatrix = glm::translate(SphereMatrix, glm::vec3(SphereX[i], SphereY[i], SphereZ[i]));
		SphereMatrix = RotateMatrix * SphereMatrix;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(SphereMatrix));
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_FILL);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluSphere(qobj, 0.1, 20, 20);
	}

	// 육면체 생성
	glUseProgram(s_program[4]);
	glBindVertexArray(vao[0]);

	BoxMatrix[0] = glm::translate(BoxMatrix[0], glm::vec3(BoxX[0], BoxY[0], BoxZ[0]));
	BoxMatrix[0] = glm::scale(BoxMatrix[0], glm::vec3(0.5f, 0.5f, 0.5f));
	BoxMatrix[0] = RotateMatrix * BoxMatrix[0];

	BoxMatrix[1] = glm::translate(BoxMatrix[1], glm::vec3(BoxX[1], BoxY[1], BoxZ[1]));
	BoxMatrix[1] = glm::scale(BoxMatrix[1], glm::vec3(0.25f, 0.25f, 0.25f));
	BoxMatrix[1] = RotateMatrix * BoxMatrix[1];

	BoxMatrix[2] = glm::translate(BoxMatrix[2], glm::vec3(BoxX[2], BoxY[2], BoxZ[2]));
	BoxMatrix[2] = glm::scale(BoxMatrix[2], glm::vec3(0.125f, 0.125f, 0.125f));
	BoxMatrix[2] = RotateMatrix * BoxMatrix[2];

	viewLocation = glGetUniformLocation(s_program[4], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[4], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[4], "modelTransform");
	
	for (int i = 0; i < 3; ++i)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(BoxMatrix[i]));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
	// 육면체 테두리
	glUseProgram(s_program[5]);
	glBindVertexArray(vao[2]);

	viewLocation = glGetUniformLocation(s_program[5], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[5], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	BoxMatrix[0] = glm::scale(BoxMatrix[0], glm::vec3(1.001f, 1.001f, 1.001f));
	BoxMatrix[1] = glm::scale(BoxMatrix[1], glm::vec3(1.001f, 1.001f, 1.001f));
	BoxMatrix[2] = glm::scale(BoxMatrix[2], glm::vec3(1.001f, 1.001f, 1.001f));

	modelLocation = glGetUniformLocation(s_program[5], "modelTransform");
	for (int i = 0; i < 3; ++i)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(BoxMatrix[i]));
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	}
}

void RandomSphere()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(-90, 90);
	std::uniform_int_distribution<int> tf(0, 1);

	for (int i = 0; i < 5; ++i)
	{
		SphereX[i] = (GLfloat)dis(gen) * 0.01;
		SphereY[i] = (GLfloat)dis(gen) * 0.01;
		SphereZ[i] = (GLfloat)dis(gen) * 0.01;
		SphereMoveX[i] = (bool)tf(gen);
		SphereMoveY[i] = (bool)tf(gen);
		SphereMoveZ[i] = (bool)tf(gen);
	}
}

void MoveSphere()
{
	for (int i = 0; i < 5; ++i)
	{
		if (SphereMoveX[i])
		{
			SphereX[i] += 0.05f;
			if (SphereX[i] > 0.89f && SphereZ[i] > -0.89f)
			{
				SphereX[i] = 0.9f;
				SphereMoveX[i] = false;
			}
		}

		else
		{
			SphereX[i] -= 0.05f;
			if (SphereX[i] < -0.89 && SphereZ[i] > -0.89f)
			{
				SphereX[i] = -0.9f;
				SphereMoveX[i] = true;
			}
		}

		if (SphereMoveY[i])
		{
			SphereY[i] += 0.05f;
			if (SphereY[i] > 0.89f && SphereZ[i] > -2.0f)
			{
				SphereY[i] = 0.9f;
				SphereMoveY[i] = false;
			}
		}

		else
		{
			SphereY[i] -= 0.05f;
			if (SphereY[i] < -0.89 && SphereZ[i] > -0.89f)
			{
				SphereY[i] = -0.9f;
				SphereMoveY[i] = true;
			}
		}

		if (SphereMoveZ[i])
		{
			SphereZ[i] += 0.05f;
			if (SphereZ[i] > 0.89f)
			{
				SphereZ[i] = 0.9f;
				SphereMoveZ[i] = false;
			}
		}

		else
		{
			SphereZ[i] -= 0.05f;
			if (!OpenFloor)
			{
				if (SphereZ[i] < -0.89f)
				{
					SphereZ[i] = -0.9f;
					SphereMoveZ[i] = true;
				}
			}
		}
	}
}

void MoveBox()
{
	for (int i = 0; i < 3; ++i)
	{
		if (OpenFloor)
		{
			BoxZ[i] -= (0.05f + BoxAccel[i]);
			BoxAccel[i] += 0.01f;
		}

		if (RRadius == 0)
		{
			if (BoxY[i] > -0.99 + BOXSIZE * BoxSize[i])
			{
				BoxY[i] -= (0.05f + BoxAccel[i]);
				BoxAccel[i] += 0.01f;
			}

			if (BoxY[i] < -0.99 + BOXSIZE * BoxSize[i] && BoxZ[i] > -0.99)
			{
				BoxY[i] = -0.99 + BOXSIZE * BoxSize[i];
				BoxAccel[i] = 0.0f;
			}
		}

		else if (RRadius > 0 && RRadius < 90)
		{
			if (BoxX[i] > -0.99 + BOXSIZE * BoxSize[i])
			{
				BoxX[i] -= (0.05f + BoxAccel[i]);
				BoxAccel[i] += 0.01f;
			}

			if (BoxX[i] < -0.99 + BOXSIZE * BoxSize[i] && BoxZ[i] > -0.99)
			{
				BoxX[i] = -0.99 + BOXSIZE * BoxSize[i];
				BoxAccel[i] = 0.0f;
			}

			if (BoxY[i] > -0.99 + BOXSIZE * BoxSize[i])
			{
				BoxY[i] -= (0.05f + BoxAccel[i]);
				BoxAccel[i] += 0.01f;
			}

			if (BoxY[i] < -0.99 + BOXSIZE * BoxSize[i] && BoxZ[i] > -0.99)
			{
				BoxY[i] = -0.99 + BOXSIZE * BoxSize[i];
				BoxAccel[i] = 0.0f;
			}
		}

		else if (RRadius == 90)
		{
			if (BoxX[i] > -0.99 + BOXSIZE * BoxSize[i])
			{
				BoxX[i] -= (0.05f + BoxAccel[i]);
				BoxAccel[i] += 0.01f;
			}

			if (BoxX[i] < -0.99 + BOXSIZE * BoxSize[i] && BoxZ[i] > -0.99)
			{
				BoxX[i] = -0.99 + BOXSIZE * BoxSize[i];
				BoxAccel[i] = 0.0f;
			}
		}

		else if (RRadius > 90 && RRadius < 180)
		{
			if (BoxX[i] > -0.99 + BOXSIZE * BoxSize[i])
			{
				BoxX[i] -= (0.05f + BoxAccel[i]);
				BoxAccel[i] += 0.01f;
			}

			if (BoxX[i] < -0.99 + BOXSIZE * BoxSize[i] && BoxZ[i] > -0.99)
			{
				BoxX[i] = -0.99 + BOXSIZE * BoxSize[i];
				BoxAccel[i] = 0.0f;
			}

			if (BoxY[i] < 0.99 - BOXSIZE * BoxSize[i])
			{
				BoxY[i] += (0.05f + BoxAccel[i]);
				BoxAccel[i] += 0.01f;
			}

			if(BoxY[i] > 0.99 - BOXSIZE * BoxSize[i] && BoxZ[i] > -0.99)
			{
				BoxY[i] = 0.99 - BOXSIZE * BoxSize[i];
				BoxAccel[i] += 0.0f;
			}
		}

		else if (RRadius == 180)
		{
			if (BoxY[i] < 0.99 - BOXSIZE * BoxSize[i])
			{
				BoxY[i] += (0.05f + BoxAccel[i]);
				BoxAccel[i] += 0.01f;
			}

			if (BoxY[i] > 0.99 - BOXSIZE * BoxSize[i] && BoxZ[i] > -0.99)
			{
				BoxY[i] = 0.99 - BOXSIZE * BoxSize[i];
				BoxAccel[i] += 0.0f;
			}
		}

		else if (RRadius > 180 && RRadius < 270)
		{
			if (BoxX[i] < 0.99 - BOXSIZE * BoxSize[i])
			{
				BoxX[i] += (0.05f + BoxAccel[i]);
				BoxAccel[i] += 0.01f;
			}

			if (BoxX[i] > 0.99 - BOXSIZE * BoxSize[i] && BoxZ[i] > -0.99)
			{
				BoxX[i] = 0.99 - BOXSIZE * BoxSize[i];
				BoxAccel[i] += 0.0f;
			}

			if (BoxY[i] < 0.99 - BOXSIZE * BoxSize[i])
			{
				BoxY[i] += (0.05f + BoxAccel[i]);
				BoxAccel[i] += 0.01f;
			}

			if (BoxY[i] > 0.99 - BOXSIZE * BoxSize[i] && BoxZ[i] > -0.99)
			{
				BoxY[i] = 0.99 - BOXSIZE * BoxSize[i];
				BoxAccel[i] += 0.0f;
			}
		}

		else if (RRadius == 270)
		{
			if (BoxX[i] < 0.99 - BOXSIZE * BoxSize[i])
			{
				BoxX[i] += (0.05f + BoxAccel[i]);
				BoxAccel[i] += 0.01f;
			}

			if (BoxX[i] > 0.99 - BOXSIZE * BoxSize[i])
			{
				BoxX[i] = 0.99 - BOXSIZE * BoxSize[i];
				BoxAccel[i] += 0.0f;
			}
		}

		else if (RRadius > 270 && RRadius < 360)
		{
			if (BoxX[i] < 0.99 - BOXSIZE * BoxSize[i])
			{
				BoxX[i] += (0.05f + BoxAccel[i]);
				BoxAccel[i] += 0.01f;
			}

			if (BoxX[i] > 0.99 - BOXSIZE * BoxSize[i])
			{
				BoxX[i] = 0.99 - BOXSIZE * BoxSize[i];
				BoxAccel[i] += 0.0f;
			}

			if (BoxY[i] > -0.99 + BOXSIZE * BoxSize[i])
			{
				BoxY[i] -= (0.05f + BoxAccel[i]);
				BoxAccel[i] += 0.01f;
			}

			if (BoxY[i] < -0.99 + BOXSIZE * BoxSize[i])
			{
				BoxY[i] = -0.99 + BOXSIZE * BoxSize[i];
				BoxAccel[i] = 0.0f;
			}
		}
	}
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mx = x;
		Click = true;
	}
	

	if (state == GLUT_UP)
		Click = false;
		
}

void Motion(int x, int y)
{
	if (Click)
	{
		if (mx > x)
		{
			RRadius = (GLfloat)((int)(RRadius + 5.5) % 360);
		}

		else if (mx < x)
		{
			RRadius = (GLfloat)RRadius - 5.5;
			if (RRadius < 0.0)
				RRadius = RRadius + 360.0;
		}
		mx = x;
	}

}


void RotateCameraCenterY()
{
	GLfloat tmpX = cameraZ * glm::sin(glm::radians(1.0)) + cameraX * glm::cos(glm::radians(1.0));
	GLfloat tmpZ = cameraZ * glm::cos(glm::radians(1.0)) - cameraX * glm::sin(glm::radians(1.0));

	cameraX = tmpX;
	cameraZ = tmpZ;
}