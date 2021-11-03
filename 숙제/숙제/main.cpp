#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <random>

#define BOXSIZE 1.0

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader[3], fragmentShader[3];
GLuint s_program[3];
GLuint vao, vbo[2], EBO[2];
GLint width, height;
GLuint base;

void DrawMain();
void DrawText();
void RandomObjects();
void UDObjects();
void InitMaze();
void MakeMaze();
void GetSize();
void KeyBoard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void TimerFunc(int value);

GLfloat cameraX = 0.0, cameraY = 12.0, cameraZ = 12.0;
GLfloat SizeX = 0.0, SizeY[20][20], SizeZ = 0.0;
GLfloat Speed[20][20], MaxY[20][20], MinY[20][20];
glm::vec3 cameraPos = glm::vec3(cameraX, cameraY, cameraZ); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0, 0.0, 0.0); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

char left[2], right[2];
int CursorTimer = 0, lefti = -1, righti = -1;
int MazeX = 0, MazeY = 0;
bool Cursor = true, LeftSelected = true, drawMaze = false;
bool BloackMoveUD = false;
bool Show[20][20];
bool UpDown[20][20];

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
	{0.8, 0.8, 0.8},
	{0.3, 0.3, 0.3},
	{0.3, 0.3, 0.3},
	{0.8, 0.8, 0.8},
	{0.8, 0.8, 0.8},
	{0.3, 0.3, 0.3},
	{0.3, 0.3, 0.3},
	{0.8, 0.8, 0.8}
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
	vertexsource = filetobuf("vertex_text.glsl");
	vertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[0], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[0]);

	vertexsource = filetobuf("vertex_floor.glsl");
	vertexShader[1] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[1], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[1]);

	vertexsource = filetobuf("vertex.glsl");
	vertexShader[2] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[2], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[2]);
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment_text.glsl");
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[0], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[0]);

	fragmentsource = filetobuf("fragment_floor.glsl");
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[1]);

	fragmentsource = filetobuf("fragment.glsl");
	fragmentShader[2] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[2], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[2]);
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

	glAttachShader(s_program[2], vertexShader[2]);
	glAttachShader(s_program[2], fragmentShader[2]);
	glLinkProgram(s_program[2]);
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 780, 300, 100);
	DrawText();
	glViewport(400, 0, 800, 800);
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
	width = 1500;
	height = 900;
	RandomObjects();

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("미로만들기");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(KeyBoard);
	glutTimerFunc(25, TimerFunc, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void KeyBoard(unsigned char key, int x, int y)
{
	switch (key) {
	case 8:
		if (LeftSelected)
		{
			if (lefti == 1 || lefti == 0)
				--lefti;
		}
		else
		{
			if (righti == 1 || righti == 0)
				--righti;
		}
		break;
	case 9:
		LeftSelected = !LeftSelected;
		break;
	case 13:
		GetSize();
		if (MazeX > 4 && MazeX < 21 && MazeY > 4 && MazeY < 21)
		{
			SizeX = 6.0 / (GLfloat)MazeX;
			SizeZ = 6.0 / (GLfloat)MazeY;
			InitMaze();
			drawMaze = true;
		}
		else
			drawMaze = false;
		break;
	case '1':
		if (LeftSelected)
		{
			if (lefti < 1)
			{
				++lefti;
				left[lefti] = '1';
			}
		}
		else
		{
			if (righti < 1)
			{
				++righti;
				right[righti] = '1';
			}
		}
		break;
	case '2':
		if (LeftSelected)
		{
			if (lefti < 1)
			{
				++lefti;
				left[lefti] = '2';
			}
		}
		else
		{
			if (righti < 1)
			{
				++righti;
				right[righti] = '2';
			}
		}
		break;
	case '3':
		if (LeftSelected)
		{
			if (lefti < 1)
			{
				++lefti;
				left[lefti] = '3';
			}
		}
		else
		{
			if (righti < 1)
			{
				++righti;
				right[righti] = '3';
			}
		}
		break;
	case '4':
		if (LeftSelected)
		{
			if (lefti < 1)
			{
				++lefti;
				left[lefti] = '4';
			}
		}
		else
		{
			if (righti < 1)
			{
				++righti;
				right[righti] = '4';
			}
		}
		break;
	case '5':
		if (LeftSelected)
		{
			if (lefti < 1)
			{
				++lefti;
				left[lefti] = '5';
			}
		}

		else
		{
			if (righti < 1)
			{
				++righti;
				right[righti] = '5';
			}
		}
		break;
	case '6':
		if (LeftSelected)
		{
			if (lefti < 1)
			{
				++lefti;
				left[lefti] = '6';
			}
		}
		else
		{
			if (righti < 1)
			{
				++righti;
				right[righti] = '6';
			}
		}
		break;
	case '7':
		if (LeftSelected)
		{
			if (lefti < 1)
			{
				++lefti;
				left[lefti] = '7';
			}
		}
		else
		{
			if (righti < 1)
			{
				++righti;
				right[righti] = '7';
			}
		}
		break;
	case '8':
		if (LeftSelected)
		{
			if (lefti < 1)
			{
				++lefti;
				left[lefti] = '8';
			}
		}
		else
		{
			if (righti < 1)
			{
				++righti;
				right[righti] = '8';
			}
		}
		break;
	case '9':
		if (LeftSelected)
		{
			if (lefti < 1)
			{
				++lefti;
				left[lefti] = '9';
			}
		}
		else
		{
			if (righti < 1)
			{
				++righti;
				right[righti] = '9';
			}
		}
		break;
	case '0':
		if (LeftSelected)
		{
			if (lefti < 1)
			{
				++lefti;
				left[lefti] = '0';
			}
		}
		else
		{
			if (righti < 1)
			{
				++righti;
				right[righti] = '0';
			}
		}
		break;
	case 'R':
	case 'r':
		MakeMaze();
		break;
	case 'z':
		cameraZ += 0.1f;
		break;
	case 'Z':
		cameraZ -= 0.1f;
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
	CursorTimer = (CursorTimer + 1) % 20;
	UDObjects();
	if (CursorTimer == 0)
		Cursor = !Cursor;

	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void DrawText()
{
	glUseProgram(s_program[0]);

	glRasterPos3f(-0.5, 0.0, 0.0);
	if (lefti > -1)
	{
		if (lefti == 0)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, left[0]);
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, left[0]);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, left[1]);
		}
	}
	if (LeftSelected)
	{
		if (Cursor)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '|');
	}

	glRasterPos3f(0.5, 0.0, 0.0);
	if (righti > -1)
	{
		if (righti == 0)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, right[0]);
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, right[0]);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, right[1]);
		}
	}
	if (!LeftSelected)
	{
		if (Cursor)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '|');
	}
	glRasterPos3f(0.0, 0.0, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');
}

void DrawMain()
{
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;
	GLfloat tmpX = 0.0, tmpZ = 0.0;

	cameraPos = glm::vec3(cameraX, cameraY, cameraZ);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	projection = glm::perspective(glm::radians(65.0f), (float)800 / (float)800, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -1.0));


	// 바닥깔기
	transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0, -0.05, 0.0));
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(-90.0), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(6.0, 6.0, 1.0));
	transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0, 0.0, -BOXSIZE));

	glUseProgram(s_program[1]);
	glBindVertexArray(vao);

	viewLocation = glGetUniformLocation(s_program[1], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[1], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// 도형그리기
	glUseProgram(s_program[2]);
	glBindVertexArray(vao);

	viewLocation = glGetUniformLocation(s_program[2], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[2], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	if (drawMaze)
	{
		for (int i = 0; i < MazeX; ++i)
		{
			for (int j = 0; j < MazeY; ++j)
			{
				if (Show[i][j])
				{
					tmpX = 6.0 - SizeX * (GLfloat)i * 2.0 - SizeX;
					tmpZ = 6.0 - SizeZ * (GLfloat)j * 2.0 - SizeZ;
					transformMatrix = glm::mat4(1.0f);
					transformMatrix = glm::translate(transformMatrix, glm::vec3(tmpX, BOXSIZE * SizeY[i][j], tmpZ));
					transformMatrix = glm::scale(transformMatrix, glm::vec3(SizeX, SizeY[i][j], SizeZ));

					modelLocation = glGetUniformLocation(s_program[2], "modelTransform");
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

					glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
				}

			}
		}
	}
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (x < 120 && x > 30 && y < 80 && y > 40)
			LeftSelected = true;

		if (x > 180 && x < 265 && y < 80 && y > 40)
			LeftSelected = false;
	}
}

void GetSize()
{
	if (lefti > -1 && righti > -1)
	{
		MazeX = 0, MazeY = 0;
		for (int i = 0; i < lefti + 1; ++i)
		{
			MazeX = 10 * MazeX + ((int)left[i] - 48);
		}

		for (int i = 0; i < righti + 1; ++i)
		{
			MazeY = 10 * MazeY+ ((int)right[i] - 48);
		}
	}
}

void RandomObjects()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 30);
	std::uniform_int_distribution<int> spd(5, 20);
	std::uniform_int_distribution<int> xy(5, 10);

	GLfloat tmp;
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			tmp = (GLfloat)dis(gen);
			tmp = tmp * 0.1;
			SizeY[i][j] = tmp;

			tmp = (GLfloat)spd(gen);
			tmp = tmp * 0.005;
			Speed[i][j] = tmp;

			tmp = (GLfloat)xy(gen);
			tmp = tmp * 0.05;
			MaxY[i][j] = tmp;

			tmp = (GLfloat)xy(gen);
			tmp = tmp * 0.05;
			MinY[i][j] = tmp;

			if (dis(gen) % 2 == 0)
			{
				UpDown[i][j] = true;
			}
			else
			{
				UpDown[i][j] = false;
			}
		}
	}
}

void UDObjects()
{
	for (int i = 0; i < MazeX; ++i)
	{
		for (int j = 0; j < MazeY; ++j)
		{
			if (UpDown[i][j])
			{
				SizeY[i][j] += Speed[i][j];
				if (SizeY[i][j] > (4.0f - MaxY[i][j]))
					UpDown[i][j] = false;
			}

			else
			{
				SizeY[i][j] -= Speed[i][j];
				if (SizeY[i][j] < MinY[i][j])
					UpDown[i][j] = true;
			}
		}
	}
}

void InitMaze()
{
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			Show[i][j] = true;
		}
	}
}

void MakeMaze()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 19);

	int n = 0;
	int Seed = 0;
	int tmpX = 0, tmpY = 0;
	Show[tmpX][tmpY] = false;
	while (n < 5)
	{
		Seed = dis(gen);

		if (Seed < 5)
		{
			for (int i = 1; i < Seed + 1; ++i)
			{
				Show[tmpX + i][tmpY] = false;
			}
			tmpX = tmpX + Seed + 1;
		}

		else if (Seed < 10)
		{

		}

		else if (Seed < 15)
		{

		}

		else if (Seed < 20)
		{

		}

		++n;
	}
}