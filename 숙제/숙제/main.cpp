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
GLuint vertexShader[4], fragmentShader[4];
GLuint s_program[4];
GLuint vao, vbo[2], EBO[2];
GLint width, height;
GLuint base;

void DrawMain();
void DrawMap();
void DrawMainMap();
void DrawText();
void MoveObjects();
void ChooseDir();
void CorrectLocation();
void CorrectCamera();
void RandomObjects();
void UDObjects();
void Init();
void setFlag();
void LocateCamera();
void RotateCameraX();
void RotateCameraY();
void InitMaze();
void MakeMaze();
void GetSize();
bool CheckCollide(int tmpDir);
bool checkFlag();
GLfloat GetX(int x);
GLfloat GetZ(int z);
void KeyBoard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void TimerFunc(int value);

GLUquadricObj* qobj;
GLfloat cameraX = 0.0, cameraY = 18.0, cameraZ = 12.0, CameraR = 0.0;
GLfloat SizeX = 0.0, SizeY[50][50], SizeZ = 0.0, ObjectSize = 0.0f, MainX = 0.0, MainZ = 0.0, ObjectSpeed = 0.05f;
GLfloat Speed[50][50], MaxY[50][50], MinY[50][50];

glm::vec3 cameraPos = glm::vec3(cameraX, cameraY, cameraZ); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0, 0.0, 0.0); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

glm::vec3 firstcameraPos = glm::vec3(MainX, ObjectSize, MainZ); //--- 카메라 위치
glm::vec3 firstcameraDirection = glm::vec3(0.0, 0.0, 0.0); //--- 카메라 바라보는 방향

char left[2], right[2];
int CursorTimer = 0, lefti = -1, righti = -1;
int MazeX = 0, MazeY = 0, cameradir = 0, dir = 0;
int ObjectX = 0, ObjectZ = 0;

bool flag[4] = {false, false, false, false};
bool ManualMove = false, AutoMove = false, Maze = false, Perspective = true;
bool Cursor = true, LeftSelected = true, drawMaze = false, redir = false;
bool BlockMoveUD = false, RotateX = false, RotateY = false, RotateMinus = false;
bool showObject = false;
bool Original = true;
bool Show[50][50];
bool UpDown[50][50];

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

	vertexsource = filetobuf("vertex_block.glsl");
	vertexShader[2] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[2], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[2]);

	vertexsource = filetobuf("vertex_object.glsl");
	vertexShader[3] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[3], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[3]);
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

	fragmentsource = filetobuf("fragment_block.glsl");
	fragmentShader[2] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[2], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[2]);

	fragmentsource = filetobuf("fragment_object.glsl");
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

	glAttachShader(s_program[2], vertexShader[2]);
	glAttachShader(s_program[2], fragmentShader[2]);
	glLinkProgram(s_program[2]);

	s_program[3] = glCreateProgram();

	glAttachShader(s_program[3], vertexShader[3]);
	glAttachShader(s_program[3], fragmentShader[3]);
	glLinkProgram(s_program[3]);

	glDeleteShader(vertexShader[0]);
	glDeleteShader(vertexShader[1]);
	glDeleteShader(vertexShader[2]);
	glDeleteShader(vertexShader[3]);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);
	glDeleteShader(fragmentShader[2]);
	glDeleteShader(fragmentShader[3]);
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
	glViewport(1250, 500, 200, 200);
	glEnable(GL_DEPTH_TEST);
	DrawMap();
	glDisable(GL_DEPTH_TEST);
	glViewport(1250, 100, 200, 200);
	glEnable(GL_DEPTH_TEST);
	DrawMainMap();
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
		std::cout << "Backspace: 입력 수정\n";
		break;
	case 9:
		LeftSelected = !LeftSelected;
		std::cout << "Tab: 입력위치 변경\n";
		break;
	case 13:
		RandomObjects();
		GetSize();
		if (MazeX > 4 && MazeX < 41 && MazeY > 4 && MazeY < 41)
		{
			SizeX = 6.0 / (GLfloat)MazeX;
			SizeZ = 6.0 / (GLfloat)MazeY;

			if (SizeX < SizeZ)
				ObjectSize = SizeX;
			else
				ObjectSize = SizeZ;

			ObjectX = 0;
			ObjectZ = 0;
			MainX = GetX(ObjectX);
			MainZ = GetZ(ObjectZ);
			Maze = false;
			showObject = false;
			drawMaze = true;
			AutoMove = false;
			ManualMove = false;
			InitMaze();
			std::cout << "Enter : 육면체 생성(성공)\n";
		}
		else
		{
			drawMaze = false;
			std::cout << "Enter : 육면체 생성(실패)\n";
		}
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
	case 'E':
	case 'e':
		if (Maze)
		{
			if (AutoMove)
			{
				CorrectLocation();
				AutoMove = false;
			}
			else
			{
				setFlag();
				redir = true;
				ChooseDir();
				AutoMove = true;
				ManualMove = false;
			}
			std::cout << "E/e : 물체 자동 이동\n";
		}
		break;
	case 'W':
	case 'w':
		if (!ManualMove && Maze)
		{
			if (AutoMove)
			{
				AutoMove = false;
				CorrectLocation();
			}

			if (Original)
			{
				if (ObjectZ < MazeY - 1 && !CheckCollide(0))
				{
					dir = 0;
					ManualMove = true;
				}
			}

			else if (ObjectZ < MazeY - 1 && !CheckCollide(dir))
				ManualMove = true;

			std::cout << "W/w : 물체 전진 이동\n";
		}
		break;
	case 'S':
	case 's':
		if (!ManualMove && Maze)
		{
			if (AutoMove)
			{
				AutoMove = false;
				CorrectLocation();
			}

			if (Original)
			{
				if (ObjectZ > 0 && !CheckCollide(1))
				{
					dir = 1;
					ManualMove = true;
				}
			}

			else
			{
				switch (dir)
				{
				case 0:
					if (ObjectZ > 0 && !CheckCollide(1))
						ManualMove = true;
					dir = 1;
					break;
				case 1:
					if (ObjectZ < MazeY - 1 && !CheckCollide(0))
						ManualMove = true;
					dir = 0;
					break;
				case 2:
					if (ObjectX > 0 && !CheckCollide(3))
						ManualMove = true;
					dir = 3;
					break;
				case 3:
					if (ObjectX < MazeX - 1 && !CheckCollide(2))
						ManualMove = true;
					dir = 2;
					break;
				}

			}
			std::cout << "S/s : 물체 후진 이동\n";
		}
		break;
	case 'A':
	case 'a':
		if (!ManualMove && Maze)
		{
			if (AutoMove)
			{
				AutoMove = false;
				CorrectLocation();
			}
			if (Original)
			{
				if (ObjectX < MazeX - 1 && !CheckCollide(2))
				{
					dir = 2;
					ManualMove = true;
				}
			}
			else
			{
				switch (dir)
				{
				case 0:
					if (ObjectX < MazeX - 1 && !CheckCollide(2))
						ManualMove = true;
					dir = 2;
					break;
				case 1:
					if (ObjectX > 0 && !CheckCollide(3))
						ManualMove = true;
					dir = 3;
					break;
				case 2:
					if (ObjectZ > 0 && !CheckCollide(1))
						ManualMove = true;
					dir = 1;
					break;
				case 3:
					if (ObjectZ < MazeY - 1 && !CheckCollide(0))
						ManualMove = true;
					dir = 0;
					break;
				}

			}
			std::cout << "S/s : 물체 좌측 이동\n";
		}
		break;
	case 'D':
	case 'd':
		if (!ManualMove && Maze)
		{
			if (AutoMove)
			{
				AutoMove = false;
				CorrectLocation();
			}
			if (Original)
			{
				if (ObjectX > 0 && !CheckCollide(3))
				{
					dir = 3;
					ManualMove = true;
				}
			}

			else
			{
				switch (dir)
				{
				case 0:
					if (ObjectX > 0 && !CheckCollide(3))
						ManualMove = true;
					dir = 3;
					break;
				case 1:
					if (ObjectX < MazeX - 1 && !CheckCollide(2))
						ManualMove = true;
					dir = 2;
					break;
				case 2:
					if (ObjectZ < MazeY - 1 && !CheckCollide(0))
						ManualMove = true;
					dir = 0;
					break;
				case 3:
					if (ObjectZ > 0 && !CheckCollide(1))
						ManualMove = true;
					dir = 1;
					break;
				}

			}
			std::cout << "S/s : 물체 우측 이동\n";
		}
		break;
	case 'R':
	case 'r':
		if (drawMaze)
		{
			std::cout << "R/r : 미로 생성\n";
			InitMaze();
			MakeMaze();
			ObjectX = 0;
			ObjectZ = 0;
			MainX = GetX(ObjectX);
			MainZ = GetZ(ObjectZ);
			Maze = true;
			showObject = true;
		}
		break;
	case 'M':
	case 'm':
		BlockMoveUD = !BlockMoveUD;
		std::cout << "M/m : 육면체 이동/정지\n";
		break;
	case 'V':
	case 'v':
		BlockMoveUD = !BlockMoveUD;
		if (!BlockMoveUD)
		{
			for (int i = 0; i < MazeY; ++i)
			{
				for (int j = 0; j < MazeX; ++j)
					SizeY[i][j] = 0.5;
			}
		}
		std::cout << "V/v : 육면체 이동 or (낮은높이 + 정지)\n";
		break;
	case 'X':
		if (RotateX && RotateMinus)
		{
			RotateX = false;
			std::cout << "X : 카메라 바닥 x축 음의 방향 회전(중지)\n";
		}

		else
		{
			RotateX = true;
			RotateMinus = true;
			std::cout << "X : 카메라 바닥 x축 음의 방향 회전\n";
		}
		break;
	case 'x':
		if (RotateX && !RotateMinus)
		{
			RotateX = false;
			std::cout << "x : 카메라 바닥 x축 양의 방향 회전(중지)\n";
		}
		else
		{
			RotateX = true;
			RotateMinus = false;
			std::cout << "x : 카메라 바닥 x축 양의 방향 회전\n";
		}
		break;
	case 'Y':
		if (RotateY && RotateMinus)
		{
			std::cout << "Y : 카메라 바닥 y축 음의 방향 회전(중지)\n";
			RotateY = false;
		}
		else
		{
			RotateY = true;
			RotateMinus = true;
			std::cout << "Y : 카메라 바닥 y축 음의 방향 회전\n";
		}
		break;
	case 'y':
		if (RotateY && !RotateMinus)
		{
			RotateY = false;
			std::cout << "y : 카메라 바닥 y축 양의 방향 회전(중지)\n";
		}
		else
		{
			RotateY = true;
			RotateMinus = false;
			std::cout << "y : 카메라 바닥 y축 양의 방향 회전\n";
		}
		break;
	case 'z':
		cameraZ += 0.1f;
		std::cout << "z : 카메라 위치 z축 +0.1f 이동\n";
		break;
	case 'Z':
		cameraZ -= 0.1f;
		std::cout << "Z : 카메라 위치 z축 -0.1f 이동\n";
		break;
	case 'C':
	case 'c':
		Init();
		std::cout << "C/c : 초기화\n";
		break;
	case '+':
		if (ObjectSpeed < 0.1)
			ObjectSpeed += 0.005f;
		else
			ObjectSpeed = 0.1;
		std::cout << "+ : 물체 속도 증가\n";
		std::cout << "물체 속도: " << ObjectSpeed << "\n";
		break;
	case '-':
		if (ObjectSpeed > 0.005f)
			ObjectSpeed -= 0.005f;
		std::cout << "- : 물체 속도 감소\n";
		std::cout << "물체 속도: " << ObjectSpeed << "\n";
		break;
	case ',':
		Original = true;
		std::cout << ", : 3인칭시점(쿼터뷰)\n";
		break;
	case '.':
		Original = false;
		std::cout << ". : 1인칭시점(객체시점)\n";
		break;
	case 'O':
	case 'o':
		Perspective = false;
		std::cout << "O/o : 직각투영\n";
		break;
	case 'P':
	case 'p':
		Perspective = true;
		std::cout << "P/p : 원근투영\n";
		break;
	case 'Q':
	case 'q':
		std::cout << "Q/q : 종료\n";
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void TimerFunc(int value)
{
	CursorTimer = (CursorTimer + 1) % 50;
	if (BlockMoveUD)
		UDObjects();
	if (RotateY)
		RotateCameraY();
	if (RotateX)
		RotateCameraX();
	if (CursorTimer == 0)
		Cursor = !Cursor;

	MoveObjects();
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

	if (Original)
	{
		if (Perspective)
			projection = glm::perspective(glm::radians(65.0f), (float)800 / (float)800, 0.1f, 50.0f);
		else
			projection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, -15.0f, 25.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -1.0));
		cameraPos = glm::vec3(cameraX, cameraY, cameraZ);
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	}
	else
	{
		LocateCamera();
		projection = glm::perspective(glm::radians(65.0f), (float)800 / (float)800, 0.1f, 50.0f);
		view = glm::lookAt(firstcameraPos, firstcameraDirection, cameraUp);
	}


	// 바닥깔기
	transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0, 0.0, 0.0));
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
		for (int i = 0; i < MazeY; ++i)
		{
			for (int j = 0; j < MazeX; ++j)
			{
				if (Show[i][j])
				{
					tmpX = GetX(j);
					tmpZ = GetZ(i);

					transformMatrix = glm::mat4(1.0f);
					transformMatrix = glm::translate(transformMatrix, glm::vec3(tmpX, SizeY[i][j], tmpZ));
					transformMatrix = glm::scale(transformMatrix, glm::vec3(SizeX, SizeY[i][j], SizeZ));

					modelLocation = glGetUniformLocation(s_program[2], "modelTransform");
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

					glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
				}

			}
		}
	}

	// 객체 생성
	if (Original && showObject)
	{
		glUseProgram(s_program[3]);
		glBindVertexArray(vao);

		viewLocation = glGetUniformLocation(s_program[3], "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

		projectionLocation = glGetUniformLocation(s_program[3], "projectionTransform");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

		transformMatrix = glm::mat4(1.0f);
		transformMatrix = glm::translate(transformMatrix, glm::vec3(MainX, ObjectSize, MainZ));
		modelLocation = glGetUniformLocation(s_program[3], "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_FILL);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluSphere(qobj, ObjectSize, 25, 25);
	}
}

void DrawMap()
{
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;
	GLfloat tmpX = 0.0, tmpZ = 0.0;

	glm::vec3 tmpcameraPos = glm::vec3(0.0, 12.0, 0.0); //--- 카메라 위치
	glm::vec3 tmpcameraDirection = glm::vec3(0.0, 0.0, 0.0); //--- 카메라 바라보는 방향
	glm::vec3 tmpcameraUp = glm::vec3(0.0f, 0.0f, -1.0f); //--- 카메라 위쪽 방향

	projection = glm::perspective(glm::radians(50.0f), (float)800 / (float)800, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -1.0));
	view = glm::lookAt(tmpcameraPos, tmpcameraDirection, tmpcameraUp);
	


	// 바닥깔기
	transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0, 0.0, 0.0));
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
		for (int i = 0; i < MazeY; ++i)
		{
			for (int j = 0; j < MazeX; ++j)
			{
				if (Show[i][j])
				{
					tmpX = GetX(j);
					tmpZ = GetZ(i);

					transformMatrix = glm::mat4(1.0f);
					transformMatrix = glm::translate(transformMatrix, glm::vec3(tmpX, SizeY[i][j], tmpZ));
					transformMatrix = glm::scale(transformMatrix, glm::vec3(SizeX, SizeY[i][j], SizeZ));

					modelLocation = glGetUniformLocation(s_program[2], "modelTransform");
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

					glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
				}

			}
		}
	}

	// 객체 생성
	if (showObject)
	{
		glUseProgram(s_program[3]);
		glBindVertexArray(vao);

		viewLocation = glGetUniformLocation(s_program[3], "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

		projectionLocation = glGetUniformLocation(s_program[3], "projectionTransform");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

		transformMatrix = glm::mat4(1.0f);
		transformMatrix = glm::translate(transformMatrix, glm::vec3(MainX, ObjectSize, MainZ));
		modelLocation = glGetUniformLocation(s_program[3], "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_FILL);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluSphere(qobj, ObjectSize, 25, 25);
	}
}

void DrawMainMap()
{
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;
	GLfloat tmpX = 0.0, tmpZ = 0.0;

	glm::vec3 tmpcameraPos = glm::vec3(MainX, 8.0, MainZ); //--- 카메라 위치
	glm::vec3 tmpcameraDirection = glm::vec3(MainX, 0.0, MainZ); //--- 카메라 바라보는 방향
	glm::vec3 tmpcameraUp = glm::vec3(0.0f, 0.0f, -1.0f); //--- 카메라 위쪽 방향

	projection = glm::perspective(glm::radians(65.0f), (float)800 / (float)800, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -1.0));
	view = glm::lookAt(tmpcameraPos, tmpcameraDirection, tmpcameraUp);



	// 바닥깔기
	transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0, 0.0, 0.0));
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
		for (int i = 0; i < MazeY; ++i)
		{
			for (int j = 0; j < MazeX; ++j)
			{
				if (Show[i][j])
				{
					tmpX = GetX(j);
					tmpZ = GetZ(i);

					transformMatrix = glm::mat4(1.0f);
					transformMatrix = glm::translate(transformMatrix, glm::vec3(tmpX, SizeY[i][j], tmpZ));
					transformMatrix = glm::scale(transformMatrix, glm::vec3(SizeX, SizeY[i][j], SizeZ));

					modelLocation = glGetUniformLocation(s_program[2], "modelTransform");
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

					glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
				}

			}
		}
	}

	// 객체 생성
	if (showObject)
	{
		glUseProgram(s_program[3]);
		glBindVertexArray(vao);

		viewLocation = glGetUniformLocation(s_program[3], "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

		projectionLocation = glGetUniformLocation(s_program[3], "projectionTransform");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

		transformMatrix = glm::mat4(1.0f);
		transformMatrix = glm::translate(transformMatrix, glm::vec3(MainX, ObjectSize, MainZ));
		modelLocation = glGetUniformLocation(s_program[3], "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_FILL);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluSphere(qobj, ObjectSize, 25, 25);
	}
}

void LocateCamera()
{
	GLfloat tmpX = 0.0f, tmpZ = -2.0;
	GLfloat ResultX = 0.0f, ResultZ = 0.0f;
	ResultX = tmpZ * glm::sin(glm::radians(CameraR)) + tmpX * glm::cos(glm::radians(CameraR));
	ResultZ = tmpZ * glm::cos(glm::radians(CameraR)) - tmpX * glm::sin(glm::radians(CameraR));

	firstcameraDirection = glm::vec3(MainX + ResultX, ObjectSize, MainZ + ResultZ);
	firstcameraPos = glm::vec3(MainX, ObjectSize, MainZ);
}


void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (x < 150 && x > 30 && y < 80 && y > 40)
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

GLfloat GetX(int x)
{
	return 6.0 - ((GLfloat)x * 2.0 + 1) * SizeX;
}

GLfloat GetZ(int z)
{
	return 6.0 - ((GLfloat)z * 2.0 + 1) * SizeZ;
}

void RandomObjects()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 30);
	std::uniform_int_distribution<int> spd(5, 50);
	std::uniform_int_distribution<int> xyMax(25, 40);
	std::uniform_int_distribution<int> xyMin(0, 15);

	GLfloat tmp;
	for (int i = 0; i < 50; ++i)
	{
		for (int j = 0; j < 50; ++j)
		{
			tmp = (GLfloat)dis(gen);
			tmp = tmp * 0.1;
			SizeY[i][j] = tmp;

			tmp = (GLfloat)spd(gen);
			tmp = tmp * 0.002;
			Speed[i][j] = tmp;

			tmp = (GLfloat)xyMax(gen);
			tmp = tmp * 0.1;
			MaxY[i][j] = tmp;

			tmp = (GLfloat)xyMin(gen);
			tmp = tmp * 0.1;
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
	for (int i = 0; i < MazeY; ++i)
	{
		for (int j = 0; j < MazeX; ++j)
		{
			if (UpDown[i][j])
			{
				SizeY[i][j] += Speed[i][j];
				if (SizeY[i][j] > (MaxY[i][j]))
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
	for (int i = 0; i < MazeY; ++i)
	{
		for (int j = 0; j < MazeX; ++j)
		{
			Show[i][j] = true;
		}
	}
}

void CorrectLocation()
{
	switch (dir)
	{
	case 0:
		ObjectZ += 1;
		MainZ = GetZ(ObjectZ);
		break;
	case 1:
		ObjectZ -= 1;
		MainZ = GetZ(ObjectZ);
		break;
	case 2:
		ObjectX += 1;
		MainX = GetX(ObjectX);
		break;
	case 3:
		ObjectX -= 1;
		MainX = GetX(ObjectX);
		break;
	default:
		break;
	}
}

void RotateCameraX()
{
	GLfloat tmpY = 0.0f, tmpZ = 0.0f;

	if (RotateMinus)
	{
		tmpY = cameraY * glm::cos(glm::radians(-1.0)) - cameraZ * glm::sin(glm::radians(-1.0));
		tmpZ = cameraY * glm::sin(glm::radians(-1.0)) + cameraZ * glm::cos(glm::radians(-1.0));
	}
	else
	{
		tmpY = cameraY * glm::cos(glm::radians(1.0)) - cameraZ * glm::sin(glm::radians(1.0));
		tmpZ = cameraY * glm::sin(glm::radians(1.0)) + cameraZ * glm::cos(glm::radians(1.0));
	}

	cameraY = tmpY;
	cameraZ = tmpZ;
}

void RotateCameraY()
{
	GLfloat tmpX = 0.0f, tmpZ = 0.0f;

	if (RotateMinus)
	{
		tmpX = cameraZ * glm::sin(glm::radians(-1.0)) + cameraX * glm::cos(glm::radians(-1.0));
		tmpZ = cameraZ * glm::cos(glm::radians(-1.0)) - cameraX * glm::sin(glm::radians(-1.0));
	}

	else
	{
		tmpX = cameraZ * glm::sin(glm::radians(1.0)) + cameraX * glm::cos(glm::radians(1.0));
		tmpZ = cameraZ * glm::cos(glm::radians(1.0)) - cameraX * glm::sin(glm::radians(1.0));
	}

	cameraX = tmpX;
	cameraZ = tmpZ;
}

void Init()
{
	RandomObjects();
	SizeX = 0.0, SizeZ = 0.0, ObjectSize = 0.0f, MainX = 0.0, MainZ = 0.0, ObjectSpeed = 0.1f;
	cameraX = 0.0, cameraY = 18.0, cameraZ = 12.0;
	CursorTimer = 0, lefti = -1, righti = -1;
	MazeX = 0, MazeY = 0, dir = 0;
	ObjectX = 0, ObjectZ = 0;
	ManualMove = false, AutoMove = false, Maze = false;
	Original = true, Perspective = true;
	Cursor = true, LeftSelected = true, drawMaze = false, showObject = false;
	BlockMoveUD = false, RotateX = false, RotateY = false, RotateMinus = false;
}

void CorrectCamera()
{
	switch (dir)
	{
	case 0:
		if (CameraR > 180.0)
			CameraR += 2.0;
		else
			CameraR -= 2.0;
		
		if (CameraR == 0.0 || CameraR == 360.0)
		{
			CameraR = 0.0;
			cameradir = 0;
		}
		break;
	case 1:
		if (CameraR < 180.0)
			CameraR += 2.0;
		else
			CameraR -= 2.0;

		if (CameraR == 180.0)
			cameradir = 1;
		break;
	case 2:
		if (CameraR > 90.0 && CameraR <= 270.0)
			CameraR -= 2.0;
		else
			CameraR += 2.0;

		if (CameraR == 90.0)
			cameradir = 2;
		break;
	case 3:
		if (CameraR >= 90.0 && CameraR < 270.0)
			CameraR += 2.0;
		else
			CameraR -= 2.0;

		if (CameraR == 270.0 || CameraR == -90.0)
		{
			CameraR = 270.0;
			cameradir = 3;
		}
		break;
	}
}

void MoveObjects()
{
	if (ManualMove)
	{
		if (!Original && cameradir != dir)
			CorrectCamera();
		else
		{
			switch (dir)
			{
			case 0:
				if (MainZ > GetZ(ObjectZ + 1))
					MainZ -= ObjectSpeed;
				else
				{
					MainZ = GetZ(ObjectZ + 1);
					ObjectZ += 1;
					ManualMove = false;
				}
				break;
			case 1:
				if (MainZ < GetZ(ObjectZ - 1))
					MainZ += ObjectSpeed;
				else
				{
					MainZ = GetZ(ObjectZ - 1);
					ObjectZ -= 1;
					ManualMove = false;
				}
				break;
			case 2:
				if (MainX > GetX(ObjectX + 1))
					MainX -= ObjectSpeed;
				else
				{
					MainX = GetX(ObjectX + 1);
					ObjectX += 1;
					ManualMove = false;
				}
				break;
			case 3:
				if (MainX < GetX(ObjectX - 1))
					MainX += ObjectSpeed;
				else
				{
					MainX = GetX(ObjectX - 1);
					ObjectX -= 1;
					ManualMove = false;
				}
				break;
			default:
				break;
			}
		}
	}

	else if (AutoMove)
	{
		if (!Original && cameradir != dir)
			CorrectCamera();
		else
		{
			switch (dir)
			{
			case 0:
				if (MainZ > GetZ(ObjectZ + 1))
					MainZ -= ObjectSpeed;
				else
				{
					ObjectZ += 1;
					MainZ = GetZ(ObjectZ);
					ChooseDir();
				}
				break;
			case 1:
				if (MainZ < GetZ(ObjectZ - 1))
					MainZ += ObjectSpeed;
				else
				{
					ObjectZ -= 1;
					MainZ = GetZ(ObjectZ);
					ChooseDir();
				}
				break;
			case 2:
				if (MainX > GetX(ObjectX + 1))
					MainX -= ObjectSpeed;
				else
				{
					ObjectX += 1;
					MainX = GetX(ObjectX);
					ChooseDir();
				}
				break;
			case 3:
				if (MainX < GetX(ObjectX - 1))
					MainX += ObjectSpeed;
				else
				{
					ObjectX -= 1;
					MainX = GetX(ObjectX);
					ChooseDir();
				}
				break;
			default:
				break;
			}
		}
	}

	else if (cameradir != dir)
		CorrectCamera();

}

bool CheckCollide(int tmpDir)
{
	switch (tmpDir)
	{
	case 0:
		if (Show[ObjectZ + 1][ObjectX] || ObjectZ == MazeY - 1)
			return true;
		break;
	case 1:
		if (Show[ObjectZ - 1][ObjectX] || ObjectZ == 0)
			return true;
		break;
	case 2:
		if (Show[ObjectZ][ObjectX + 1] || ObjectX == MazeX - 1)
			return true;
		break;
	case 3:
		if (Show[ObjectZ][ObjectX - 1] || ObjectX == 0)
			return true;
		break;
	default:
		break;
	}
	return false;
}

void setFlag()
{
	for (int i = 0; i < 4; ++i)
	{
		flag[i] = false;
	}
}

bool checkFlag()
{
	for (int i = 0; i < 4; ++i)
	{
		if (!flag[i])
			return false;
	}
	return true;
}

void ChooseDir()
{
	if (redir)
	{
		switch (dir)
		{
		case 0:
			if (ObjectZ < MazeY - 1 && !CheckCollide(dir))
				dir = dir;
			else
			{
				flag[0] = true;

				if (ObjectX < MazeX - 1 && !CheckCollide(2))
					dir = 2;
				else if (ObjectZ > 0 && !CheckCollide(1))
				{
					dir = 1;
					flag[2] = true;
				}
				else
				{
					flag[1] = true;
					flag[2] = true;
					dir = 3;
				}
			}
			break;
		case 1:
			if (ObjectZ > 0 && !CheckCollide(dir))
				dir = dir;
			else
			{
				flag[1] = true;

				if (ObjectX > 0 && !CheckCollide(3))
					dir = 3;
				else if (ObjectZ < MazeY - 1 && !CheckCollide(0))
				{
					flag[3] = true;
					dir = 0;
				}
				else
				{
					flag[0] = true;
					flag[3] = true;
					dir = 2;
				}
			}
			break;
		case 2:
			if (ObjectX < MazeX - 1 && !CheckCollide(dir))
				dir = dir;
			else
			{
				flag[2] = true;
				if (ObjectZ > 0 && !CheckCollide(1))
					dir = 1;
				else if (ObjectX > 0 && !CheckCollide(3))
				{
					flag[1] = true;
					dir = 3;
				}
				else
				{
					flag[1] = true;
					flag[3] = true;
					dir = 0;
				}
			}
			break;
		case 3:
			if (ObjectX > 0 && !CheckCollide(dir))
				dir = dir;
			else
			{
				flag[3] = true;
				if (ObjectZ < MazeY - 1 && !CheckCollide(0))
					dir = 0;
				else if (ObjectX < MazeX - 1 && !CheckCollide(2))
				{
					flag[0] = true;
					dir = 2;
				}
				else
				{
					flag[0] = true;
					flag[1] = true;
					dir = 1;
				}
			}
			break;
		default:
			break;
		}

		if (checkFlag())
		{
			setFlag();
			redir = false;
		}
		
	}

	else
	{
		switch (dir)
		{
		case 0:
			if (ObjectX > 0 && !CheckCollide(3))
				dir = 3;
			else if (ObjectZ < MazeY - 1 && !CheckCollide(dir))
				dir = dir;
			else if (ObjectX < MazeX - 1 && !CheckCollide(2))
				dir = 2;
			else
				dir = 1;

			if (dir != 3)
				setFlag();
			else
				flag[0] = true;
			break;
		case 1:
			if (ObjectX < MazeX - 1 && !CheckCollide(2))
				dir = 2;
			else if (ObjectZ > 0 && !CheckCollide(dir))
				dir = dir;
			else if (ObjectX > 0 && !CheckCollide(3))
				dir = 3;
			else
				dir = 0;

			if (dir != 2)
				setFlag();
			else
				flag[1] = true;
			break;
		case 2:
			if (ObjectZ < MazeY - 1 && !CheckCollide(0))
				dir = 0;
			else if (ObjectX < MazeX - 1 && !CheckCollide(dir))
				dir = dir;
			else if (ObjectZ > 0 && !CheckCollide(1))
				dir = 1;
			else
				dir = 3;

			if (dir != 0)
				setFlag();
			else
				flag[2] = true;
			break;
		case 3:
			if (ObjectZ > 0 && !CheckCollide(1))
				dir = 1;
			else if (ObjectX > 0 && !CheckCollide(dir))
				dir = dir;
			else if (ObjectZ < MazeY - 1 && !CheckCollide(0))
				dir = 0;
			else
				dir = 2;

			if (dir != 1)
				setFlag();
			else
				flag[3] = true;
			break;
		default:
			break;
		}

		if (checkFlag())
		{
			setFlag();
			redir = true;
		}
	}
	
}

void MakeMaze()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 19);

	int Seed = 0;
	int iDistance = 0;
	int tmpX = 0, tmpY = 0;
	bool Reset = true;
	Show[0][0] = false;

	while(Show[MazeY - 1][MazeX - 1])
	{
		Seed = dis(gen) % 4;

		if (Seed < 2)
			iDistance = dis(gen) % 2 + 3;
		else
			iDistance = dis(gen) % 3 + 1;

		switch (Seed)
		{
		case 0:
			for (int i = 0; i < iDistance; ++i)
			{
				if (tmpX < MazeX - 1)
				{
					Show[tmpY][++tmpX] = false;
				}
				else
					break;
			}
			break;
		case 1:
			for (int i = 0; i < iDistance; ++i)
			{
				if (tmpY < MazeY - 1)
				{
					Show[++tmpY][tmpX] = false;
				}
				else
					break;
			}
			break;
		case 2:
			for (int i = 0; i < iDistance; ++i)
			{
				if (tmpX > 0)
				{
					Show[tmpY][--tmpX] = false;
				}
				else
					break;
			}
			break;
		case 3:
			for (int i = 0; i < iDistance; ++i)
			{
				if (tmpY > 0)
				{
					Show[--tmpY][tmpX] = false;
				}
				else
					break;
			}
			break;
		default:
			break;
		}
	}
}