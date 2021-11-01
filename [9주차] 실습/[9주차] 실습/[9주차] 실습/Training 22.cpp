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
GLuint vertexShader, fragmentShader[9];
GLuint s_program[9];
GLuint vao, vbo, EBO;
GLint width, height;

GLfloat RandomX[2], RandomZ[2];
GLfloat cameraX = 0.0, cameraY = 0.0, cameraZ = 2.0, Fradius = 0.0f, armlegR = 0.0;
GLfloat CameraDirX = 0.0, CameraDirY = 0.0, CameraDirZ = 0.0;
GLfloat transX = 0.0, transY = 0.0, transZ = 0.0;
bool OpenF = false, CloseF = false, armlegPlus = true, modelMove = false, jumpUp = false, jumpDown = true, RotateY = false;
int dir = 0;
glm::vec3 cameraPos = glm::vec3(cameraX, cameraY, cameraZ); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(CameraDirX, CameraDirY, CameraDirZ); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

void DrawMain();
void RandomObjects();
void RotateCameraCenterY();
void KeyBoard(unsigned char key, int x, int y);
void TimerFunc(int value);
bool CheckCollision();

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
	vertexsource = filetobuf("vertex_objects.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader);
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment_skyblue.glsl");
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[0], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[0]);

	fragmentsource = filetobuf("fragment_white.glsl");
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[1]);

	fragmentsource = filetobuf("fragment_blue.glsl");
	fragmentShader[2] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[2], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[2]);

	fragmentsource = filetobuf("fragment_gray.glsl");
	fragmentShader[3] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[3], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[3]);

	fragmentsource = filetobuf("fragment_purple.glsl");
	fragmentShader[4] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[4], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[4]);

	fragmentsource = filetobuf("fragment_green.glsl");
	fragmentShader[5] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[5], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[5]);

	fragmentsource = filetobuf("fragment_yello.glsl");
	fragmentShader[6] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[6], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[6]);

	fragmentsource = filetobuf("fragment_brown.glsl");
	fragmentShader[7] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[7], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[7]);

	fragmentsource = filetobuf("fragment_red.glsl");
	fragmentShader[8] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[8], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[8]);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &EBO);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Shapeindex), Shapeindex, GL_STATIC_DRAW);
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

	s_program[6] = glCreateProgram();

	glAttachShader(s_program[6], vertexShader);
	glAttachShader(s_program[6], fragmentShader[6]);
	glLinkProgram(s_program[6]);

	s_program[7] = glCreateProgram();

	glAttachShader(s_program[7], vertexShader);
	glAttachShader(s_program[7], fragmentShader[7]);
	glLinkProgram(s_program[7]);

	s_program[8] = glCreateProgram();

	glAttachShader(s_program[8], vertexShader);
	glAttachShader(s_program[8], fragmentShader[8]);
	glLinkProgram(s_program[8]);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);
	glDeleteShader(fragmentShader[2]);
	glDeleteShader(fragmentShader[3]);
	glDeleteShader(fragmentShader[4]);
	glDeleteShader(fragmentShader[5]);
	glDeleteShader(fragmentShader[6]);
	glDeleteShader(fragmentShader[7]);
	glDeleteShader(fragmentShader[8]);
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
	glutCreateWindow("Training22");

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
	case 's':
		if (dir == 0 && modelMove)
			modelMove = false;
		else
		{
			modelMove = true;
			dir = 0;
		}
		break;
	case 'w':
		if (dir == 2 && modelMove)
			modelMove = false;
		else
		{
			modelMove = true;
			dir = 2;
		}
		break;
	case 'a':
		if (dir == 3 && modelMove)
			modelMove = false;
		else
		{
			modelMove = true;
			dir = 3;
		}
		break;
	case 'd':
		if (dir == 1 && modelMove)
			modelMove = false;
		else
		{
			modelMove = true;
			dir = 1;
		}
		break;
	case 'X':
		cameraX += 0.1f;
		CameraDirX += 0.1f;
		break;
	case 'x':
		cameraX -= 0.1f;
		CameraDirX -= 0.1f;
		break;
	case 'Z':
		cameraZ += 0.1f;
		CameraDirZ += 0.1f;
		break;
	case 'z':
		cameraZ -= 0.1f;
		CameraDirZ -= 0.1f;
		break;
	case 'O':
	case 'o':
		if (OpenF)
		{
			OpenF = false;
			CloseF = true;
		}
		else
			OpenF = true;
		break;
	case 'I':
	case 'i':
		RandomObjects();
		cameraX = 0.0, cameraY = 0.0, cameraZ = 2.0, Fradius = 0.0f, armlegR = 0.0;
		CameraDirX = 0.0, CameraDirY = 0.0, CameraDirZ = 0.0;
		transX = 0.0, transY = 0.0, transZ = 0.0;
		OpenF = false, CloseF = false, armlegPlus = true, modelMove = false, jumpUp = false, jumpDown = true, RotateY = false;
		dir = 0;
		break;
	case 'Y':
	case 'y':
		RotateY = !RotateY;
		break;
		break;
	case 'J':
	case 'j':
		if (!jumpUp)
			jumpUp = true;
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
	if (OpenF)
	{
		if (Fradius > -89.0)
		{
			Fradius = (GLfloat)((int)(Fradius - 1.0f) % 90);
		}
	}

	if (CloseF)
	{
		Fradius = (GLfloat)((int)(Fradius + 1.0f) % 90);
		if (Fradius > -1.0)
		{
			CloseF = false;
		}
	}

	if (modelMove)
	{
		if (armlegPlus)
		{
			armlegR = armlegR + 2.0;
			if (armlegR >= 45.0)
				armlegPlus = false;
		}
		else
		{
			armlegR = armlegR - 2.0;
			if (armlegR <= -45.0)
				armlegPlus = true;
		}

		switch (dir)
		{
		case 0:
			transZ += 0.02f;
			if (transZ > 1.0f)
				transZ = transZ - 2.0f;
			if(CheckCollision())
				transZ -= 0.02f;
			break;
		case 1:
			transX += 0.02f;
			if (transX > 1.0f)
				transX = transX - 2.0f;
			if (CheckCollision())
				transX -= 0.02f;
			break;
		case 2:
			transZ -= 0.02f;
			if (transZ < -1.0f)
				transZ = transZ + 2.0f;
			if (CheckCollision())
				transZ += 0.02f;
			break;
		case 3:
			transX -= 0.02f;
			if (transX < -1.0f)
				transX = transX + 2.0f;
			if (CheckCollision())
				transX += 0.02f;
			break;
		default:
			break;
		}
	}

	else
	{
		armlegR = 0.0f;
	}
	if (jumpUp)
	{
		transY = transY + 0.02f;
		if (transY > 0.5)
			jumpUp = false;
	}

	else
	{
		transY = transY - 0.02f;
		if (CheckCollision())
			transY += 0.02f;
		if (transY < 0.0)
			transY = 0.0;
	}
	
	if (RotateY)
		RotateCameraCenterY();
	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void DrawMain()
{
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::mat4 ObstacleMatrix = glm::mat4(1.0f);
	glm::mat4 modeltransformMatrix = glm::mat4(1.0f);

	glm::mat4 faceMatrix = glm::mat4(1.0f);
	glm::mat4 bodyMatrix = glm::mat4(1.0f);
	glm::mat4 headMatrix = glm::mat4(1.0f);
	glm::mat4 leftarmMatrix = glm::mat4(1.0f);
	glm::mat4 rightarmMatrix = glm::mat4(1.0f);
	glm::mat4 leftlegMatrix = glm::mat4(1.0f);
	glm::mat4 rightlegMatrix = glm::mat4(1.0f);

	glm::mat4 FrontMatrix = glm::mat4(1.0f);

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;

	
	//projection = glm::ortho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
	projection = glm::perspective(glm::radians(130.0f), (float)width / (float)height, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -0.5));

	cameraPos = glm::vec3(cameraX, cameraY, cameraZ);
	cameraDirection = glm::vec3(CameraDirX, CameraDirY, CameraDirZ);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	transformMatrix = glm::scale(transformMatrix, glm::vec3(2.0, 2.0, 2.0));

	FrontMatrix = glm::translate(FrontMatrix, glm::vec3(0.0, -BOXSIZE, +BOXSIZE));
	FrontMatrix = glm::rotate(FrontMatrix, (GLfloat)glm::radians(Fradius), glm::vec3(-1.0, 0.0, 0.0));
	FrontMatrix = glm::translate(FrontMatrix, glm::vec3(0.0, +BOXSIZE, -BOXSIZE));
	FrontMatrix = transformMatrix * FrontMatrix;

	modeltransformMatrix = glm::translate(modeltransformMatrix, glm::vec3(transX, -1.4 * BOXSIZE + transY, transZ));
	modeltransformMatrix = glm::rotate(modeltransformMatrix, (GLfloat)glm::radians(90.0 * (GLfloat)dir), glm::vec3(0.0, 1.0, 0.0));

	bodyMatrix = glm::translate(bodyMatrix, glm::vec3(0.0, 0.0, 0.0));
	bodyMatrix = glm::scale(bodyMatrix, glm::vec3(0.2, 0.3, 0.2));
	bodyMatrix = modeltransformMatrix * bodyMatrix;

	headMatrix = glm::translate(headMatrix, glm::vec3(0.0, BOXSIZE * 0.45, 0.0));
	headMatrix = glm::scale(headMatrix, glm::vec3(0.1, 0.15, 0.1));
	headMatrix = modeltransformMatrix * headMatrix;

	faceMatrix = glm::translate(faceMatrix, glm::vec3(0.0, BOXSIZE * 0.45, BOXSIZE * 0.1));
	faceMatrix = glm::scale(faceMatrix, glm::vec3(0.05, 0.05, 0.05));
	faceMatrix = modeltransformMatrix * faceMatrix;

	leftarmMatrix = glm::translate(leftarmMatrix, glm::vec3(-0.275 * BOXSIZE, 0.0, 0.0));
	leftarmMatrix = glm::translate(leftarmMatrix, glm::vec3(0.0, 0.075, 0.0));
	leftarmMatrix = glm::rotate(leftarmMatrix, (GLfloat)glm::radians(armlegR), glm::vec3(1.0, 0.0, 0.0));
	leftarmMatrix = glm::translate(leftarmMatrix, glm::vec3(0.0, -0.075, 0.0));
	leftarmMatrix = glm::rotate(leftarmMatrix, (GLfloat)glm::radians(-30.0), glm::vec3(0.0, 0.0, 1.0));
	leftarmMatrix = glm::scale(leftarmMatrix, glm::vec3(0.05, 0.15, 0.05));
	leftarmMatrix = modeltransformMatrix * leftarmMatrix;

	rightarmMatrix = glm::translate(rightarmMatrix, glm::vec3(0.275 * BOXSIZE, 0.0, 0.0));
	rightarmMatrix = glm::translate(rightarmMatrix, glm::vec3(0.0, 0.075, 0.0));
	rightarmMatrix = glm::rotate(rightarmMatrix, (GLfloat)glm::radians(-armlegR), glm::vec3(1.0, 0.0, 0.0));
	rightarmMatrix = glm::translate(rightarmMatrix, glm::vec3(0.0, -0.075, 0.0));
	rightarmMatrix = glm::rotate(rightarmMatrix, (GLfloat)glm::radians(30.0), glm::vec3(0.0, 0.0, 1.0));
	rightarmMatrix = glm::scale(rightarmMatrix, glm::vec3(0.05, 0.15, 0.05));
	rightarmMatrix = modeltransformMatrix * rightarmMatrix;

	leftlegMatrix = glm::translate(leftlegMatrix, glm::vec3(-0.1 * BOXSIZE, -BOXSIZE * 0.45, 0.0));
	leftlegMatrix = glm::rotate(leftlegMatrix, (GLfloat)glm::radians(-armlegR), glm::vec3(1.0, 0.0, 0.0));
	leftlegMatrix = glm::scale(leftlegMatrix, glm::vec3(0.05, 0.15, 0.05));
	leftlegMatrix = modeltransformMatrix * leftlegMatrix;

	rightlegMatrix = glm::translate(rightlegMatrix, glm::vec3(0.1 * BOXSIZE, -BOXSIZE * 0.45, 0.0));
	rightlegMatrix = glm::rotate(rightlegMatrix, (GLfloat)glm::radians(armlegR), glm::vec3(1.0, 0.0, 0.0));
	rightlegMatrix = glm::scale(rightlegMatrix, glm::vec3(0.05, 0.15, 0.05));
	rightlegMatrix = modeltransformMatrix * rightlegMatrix;

	
	// skyblue
	glUseProgram(s_program[0]);
	glBindVertexArray(vao);

	viewLocation = glGetUniformLocation(s_program[0], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(FrontMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(GLuint)));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(rightarmMatrix));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	// white
	glUseProgram(s_program[1]);
	glBindVertexArray(vao);

	viewLocation = glGetUniformLocation(s_program[1], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[1], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(30 * sizeof(GLuint)));

	// blue
	glUseProgram(s_program[2]);
	glBindVertexArray(vao);

	viewLocation = glGetUniformLocation(s_program[2], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[2], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[2], "modelTransform");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(bodyMatrix));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(headMatrix));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	// gray
	glUseProgram(s_program[3]);
	glBindVertexArray(vao);

	viewLocation = glGetUniformLocation(s_program[3], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[3], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[3], "modelTransform");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));

	// purple
	glUseProgram(s_program[4]);
	glBindVertexArray(vao);

	viewLocation = glGetUniformLocation(s_program[4], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[4], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[4], "modelTransform");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(GLuint)));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(leftarmMatrix));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// green
	glUseProgram(s_program[5]);
	glBindVertexArray(vao);

	viewLocation = glGetUniformLocation(s_program[5], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[5], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[5], "modelTransform");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(faceMatrix));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// yello
	glUseProgram(s_program[6]);
	glBindVertexArray(vao);

	viewLocation = glGetUniformLocation(s_program[6], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[6], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[6], "modelTransform");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(leftlegMatrix));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(rightlegMatrix));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// brown
	glUseProgram(s_program[7]);
	glBindVertexArray(vao);

	viewLocation = glGetUniformLocation(s_program[7], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[7], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[7], "modelTransform");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)));

	// red
	glUseProgram(s_program[8]);
	glBindVertexArray(vao);

	viewLocation = glGetUniformLocation(s_program[8], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[8], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[8], "modelTransform");

	ObstacleMatrix = glm::mat4(1.0f);
	ObstacleMatrix = glm::translate(ObstacleMatrix, glm::vec3(RandomX[0], -0.85, RandomZ[0]));
	ObstacleMatrix = glm::scale(ObstacleMatrix, glm::vec3(0.5, 0.3, 0.5));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ObstacleMatrix));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	ObstacleMatrix = glm::mat4(1.0f);
	ObstacleMatrix = glm::translate(ObstacleMatrix, glm::vec3(RandomX[1], -0.85, RandomZ[1]));
	ObstacleMatrix = glm::scale(ObstacleMatrix, glm::vec3(0.5, 0.3, 0.5));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ObstacleMatrix));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

void RandomObjects()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(-75, 75);

	RandomX[0] = (GLfloat)dis(gen) * 0.01;
	RandomX[1] = (GLfloat)dis(gen) * 0.01;
	RandomZ[0] = (GLfloat)dis(gen) * 0.01;
	RandomZ[1] = (GLfloat)dis(gen) * 0.01;

	while (RandomX[0] < 0.2 && RandomX[0] > -0.2)
		RandomX[0] = (GLfloat)dis(gen) * 0.01;
	while (RandomX[1] < 0.2 && RandomX[1] > -0.2)
		RandomX[1] = (GLfloat)dis(gen) * 0.01;
	while (RandomZ[0] < 0.2 && RandomZ[0] > -0.2)
		RandomZ[0] = (GLfloat)dis(gen) * 0.01;
	while (RandomZ[1] < 0.2 && RandomZ[1] > -0.2)
		RandomZ[1] = (GLfloat)dis(gen) * 0.01;
}

bool CheckCollision()
{
	if (RandomX[0] - BOXSIZE * 0.499 < transX + BOXSIZE * 0.199 &&
		RandomX[0] + BOXSIZE * 0.499 > transX - BOXSIZE * 0.199 &&
		-1.0 < -1.1 * BOXSIZE + transY &&
		-0.7 > -1.95 * BOXSIZE + transY &&
		RandomZ[0] - BOXSIZE * 0.499 < transZ + BOXSIZE * 0.199 &&
		RandomZ[0] + BOXSIZE * 0.499 > transZ - BOXSIZE * 0.199)
		return true;

	if (RandomX[1] - BOXSIZE * 0.499 < transX + BOXSIZE * 0.199 &&
		RandomX[1] + BOXSIZE * 0.499 > transX - BOXSIZE * 0.199 &&
		-1.0 < -1.1 * BOXSIZE + transY &&
		-0.7 > -1.95 * BOXSIZE + transY &&
		RandomZ[1] - BOXSIZE * 0.499 < transZ + BOXSIZE * 0.199 &&
		RandomZ[1] + BOXSIZE * 0.499 > transZ - BOXSIZE * 0.199)
		return true;

	return false;
}

void RotateCameraCenterY()
{
	GLfloat tmpX = cameraZ * glm::sin(glm::radians(1.0)) + cameraX * glm::cos(glm::radians(1.0));
	GLfloat tmpZ = cameraZ * glm::cos(glm::radians(1.0)) - cameraX * glm::sin(glm::radians(1.0));

	cameraX = tmpX;
	cameraZ = tmpZ;
}