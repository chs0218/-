#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <random>

#define BOXSIZE 0.25

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader, fragmentShader;
GLuint s_program[2];
GLuint vao[2], vbo[4], EBO[4];
GLint width, height;

bool HideBehind = true;
bool RotateR = false, RotateT = false, OpenF = false, CloseF = false, OpenO = false;
bool OpenLeftRight = false, CloseLeftRight = false;
bool ShowHexahedron = true, Perspective = true;
GLfloat Rradius = 0.0f, Tradius = 0.0f, Fradius = 0.0f, OpenClose = 0.0f, Oradius = 0.0f;
void KeyBoard(unsigned char key, int x, int y);
void TimerFunc(int value);
void DrawMain();

GLfloat LineDots[][3] = {
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0},
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
	{BOXSIZE, BOXSIZE, -BOXSIZE},

	// 사각뿔
	{-BOXSIZE, -BOXSIZE, -BOXSIZE},
	{-BOXSIZE, -BOXSIZE, +BOXSIZE},
	{+BOXSIZE, -BOXSIZE, +BOXSIZE},
	{+BOXSIZE, -BOXSIZE, -BOXSIZE},
	{0.0, BOXSIZE, 0.0}
};

GLfloat Colors[][3] = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
	{1.0, 0.0, 1.0},
	{0.0, 1.0, 1.0},
	{0.0, 0.0, 0.0},
	{1.0, 1.0, 1.0},

	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
	{1.0, 0.0, 1.0}
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
	1, 6, 2,

	// 사각뿔
	8, 11, 9,
	11, 10, 9,
	12, 9, 10,
	12, 10, 11,
	12, 11, 8,
	12, 8, 9
};

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader);
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader);
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

	glAttachShader(s_program[0], vertexShader);
	glAttachShader(s_program[0], fragmentShader);
	glLinkProgram(s_program[0]);

	s_program[1] = glCreateProgram();
	glAttachShader(s_program[1], vertexShader);
	glAttachShader(s_program[1], fragmentShader);
	glLinkProgram(s_program[1]);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (HideBehind)
		glEnable(GL_DEPTH_TEST);
	DrawMain();
	if (HideBehind)
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

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training1");

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
	if (key == 'o')
		ShowHexahedron = false;
	else if(key != 'y')
		ShowHexahedron = true;

	switch (key) {
	case '1':
		OpenLeftRight = true;
		CloseLeftRight = false;
		break;
	case '2':
		OpenLeftRight = false;
		CloseLeftRight = true;
		break;
	case 't':
		RotateT = !RotateT;
		break;
	case 'h':
		HideBehind = !HideBehind;
		break;
	case 'y':
		RotateR = !RotateR;
		break;
	case 'f':
		if (OpenF)
		{
			OpenF = false;
			CloseF = true;
		}
		else
			OpenF = true;
		break;
	case 'p':
		Perspective = !Perspective;
		break;
	case 'N':
	case 'n':
		break;
	case 'o':
		OpenO = !OpenO;
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

void TimerFunc(int value)
{
	if (RotateR)
		Rradius = (GLfloat)((int)(Rradius + 5.0f) % 360);
	if (RotateT)
		Tradius = (GLfloat)((int)(Tradius - 5.0f) % 360);
	
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

	if (OpenLeftRight)
	{
		if (OpenClose < 0.5f)
			OpenClose += 0.1f;
		else
			OpenLeftRight = false;
	}

	if (CloseLeftRight)
	{
		if (OpenClose >= 0.1f)
			OpenClose -= 0.1f;
		else
		{
			CloseLeftRight = false;
		}
	}

	if (!ShowHexahedron)
	{
		if (OpenO)
		{
			if (Oradius < 233.2f)
				Oradius += 5.0f;
				
			else
				Oradius = 233.2f;
		}

		else
		{
			if (Oradius > 0.0f)
				Oradius -= 5.0f;
			else
				Oradius = 0.0f;
		}
	}
	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void DrawMain()
{
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	unsigned int projectionLocation;
	unsigned int modelLocation;

	if (Perspective)
	{
		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 50.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));
	}

	else
	{
		projection = glm::ortho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	}

	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(30.0), glm::vec3(1.0, 1.0, 0.0));
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(Rradius), glm::vec3(0.0, 1.0, 0.0));

	glUseProgram(s_program[0]);

	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

	glBindVertexArray(vao[0]);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

	glUseProgram(s_program[1]);
	glBindVertexArray(vao[1]);

	projectionLocation = glGetUniformLocation(s_program[1], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	if (ShowHexahedron)
	{
		glm::mat4 TopMatrix = glm::mat4(1.0f);
		glm::mat4 FrontMatrix = glm::mat4(1.0f);
		glm::mat4 LeftRightMatrix = glm::mat4(1.0f);

		TopMatrix = glm::translate(TopMatrix, glm::vec3(0.0, +BOXSIZE, 0.0));
		TopMatrix = glm::rotate(TopMatrix, (GLfloat)glm::radians(Tradius), glm::vec3(1.0, 0.0, 0.0));
		TopMatrix = glm::translate(TopMatrix, glm::vec3(0.0, -BOXSIZE, 0.0));
		TopMatrix = transformMatrix * TopMatrix;

		FrontMatrix = glm::translate(FrontMatrix, glm::vec3(0.0, -BOXSIZE, +BOXSIZE));
		FrontMatrix = glm::rotate(FrontMatrix, (GLfloat)glm::radians(Fradius), glm::vec3(-1.0, 0.0, 0.0));
		FrontMatrix = glm::translate(FrontMatrix, glm::vec3(0.0, +BOXSIZE, -BOXSIZE));
		FrontMatrix = transformMatrix * FrontMatrix;

		LeftRightMatrix = glm::translate(LeftRightMatrix, glm::vec3(0.0, OpenClose, 0.0));
		LeftRightMatrix = transformMatrix * LeftRightMatrix;

		// 윗면
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TopMatrix));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(GLuint)));

		// 앞면
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(FrontMatrix));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// 옆면
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(LeftRightMatrix));
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)));

		// 나머지
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(30 * sizeof(GLuint)));
	}

	else
	{
		glm::mat4 Pyramid[4] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };
		Pyramid[0] = glm::translate(Pyramid[0], glm::vec3(0.0, -BOXSIZE, +BOXSIZE));
		Pyramid[0] = glm::rotate(Pyramid[0], (GLfloat)glm::radians(Oradius), glm::vec3(1.0, 0.0, 0.0));
		Pyramid[0] = glm::translate(Pyramid[0], glm::vec3(0.0, +BOXSIZE, -BOXSIZE));
		Pyramid[0] = transformMatrix * Pyramid[0];

		Pyramid[1] = glm::translate(Pyramid[1], glm::vec3(+BOXSIZE, -BOXSIZE, 0.0));
		Pyramid[1] = glm::rotate(Pyramid[1], (GLfloat)glm::radians(Oradius), glm::vec3(0.0, 0.0, -1.0));
		Pyramid[1] = glm::translate(Pyramid[1], glm::vec3(-BOXSIZE, +BOXSIZE, 0.0));
		Pyramid[1] = transformMatrix * Pyramid[1];

		Pyramid[2] = glm::translate(Pyramid[2], glm::vec3(0.0, -BOXSIZE, -BOXSIZE));
		Pyramid[2] = glm::rotate(Pyramid[2], (GLfloat)glm::radians(Oradius), glm::vec3(-1.0, 0.0, 0.0));
		Pyramid[2] = glm::translate(Pyramid[2], glm::vec3(0.0, +BOXSIZE, +BOXSIZE));
		Pyramid[2] = transformMatrix * Pyramid[2];

		Pyramid[3] = glm::translate(Pyramid[3], glm::vec3(-BOXSIZE, -BOXSIZE, 0.0));
		Pyramid[3] = glm::rotate(Pyramid[3], (GLfloat)glm::radians(Oradius), glm::vec3(0.0, 0.0, 1.0));
		Pyramid[3] = glm::translate(Pyramid[3], glm::vec3(+BOXSIZE, +BOXSIZE, 0.0));
		Pyramid[3] = transformMatrix * Pyramid[3];

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(36 * sizeof(GLuint)));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Pyramid[0]));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(42 * sizeof(GLuint)));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Pyramid[1]));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(45 * sizeof(GLuint)));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Pyramid[2]));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(48 * sizeof(GLuint)));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Pyramid[3]));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(51 * sizeof(GLuint)));
	}
}