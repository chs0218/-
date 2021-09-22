#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define WIN_SIZE 800

enum Direction {
	LeftUp = 0,
	RightUp,
	RightDown,
	LeftDown
};

struct Box {
	int iOriginalX = 0;
	int iOriginalY = 0;
	Direction dir;
	GLclampf iRed = 0.0f;
	GLclampf iGreen = 0.0f;
	GLclampf iBlue = 0.0f;
	int iX = 0;
	int iY = 0;
	bool bBoxReveal = false;
};

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLclampf IntToFloat(int i, bool IsX);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);

Box MyBox[5];

int iBoxNum = 0;
int iBoxSize = 25;
bool Animation = false;
bool SizeUp = true;
bool CommandC = false;

void main(int argc, char** argv)					//---윈도우 출력하고 콜백함수 설정
{

	//---윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(WIN_SIZE, WIN_SIZE); // 윈도우의 크기 지정
	glutCreateWindow("Training 4"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse); // 마우스 입력 콜백함수 지정
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백함수 지정
	glutMainLoop(); // 이벤트 처리 시
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f); // 바탕색 설정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다
	for (int i = 0; i < 5; ++i)
	{
		if (MyBox[i].bBoxReveal)
		{
			glColor3f(MyBox[i].iRed, MyBox[i].iGreen, MyBox[i].iBlue);
			glRectf(IntToFloat(MyBox[i].iX - iBoxSize, true), IntToFloat(MyBox[i].iY + iBoxSize, false), IntToFloat(MyBox[i].iX + iBoxSize, true), IntToFloat(MyBox[i].iY - iBoxSize, false));
		}
	}
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

GLclampf IntToFloat(int i, bool IsX)
{
	GLclampf fResult;
	float fHalf = (float)WIN_SIZE / 2.0f;
	fResult = ((float)i - fHalf) / fHalf;

	if (IsX)
		return fResult;
	else
		return fResult * -1.0f;
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'A':
	case 'a':
		glutTimerFunc(50, TimerFunction, 1);
		Animation = true;
		break;
	case 'C':
	case 'c':
		if (CommandC)
			CommandC = false;
		else
			CommandC = true;
		break;
	case 'S':
	case 's':
		Animation = false;
		break;
	case 'm':
	case 'M':
		Animation = false;
		iBoxSize = 25;
		for (int i = 0; i < 5; ++i)
		{
			if (MyBox[i].bBoxReveal)
			{
				MyBox[i].iX = MyBox[i].iOriginalX;
				MyBox[i].iY = MyBox[i].iOriginalY;
			}
		}
		break;
	case 'R':
	case 'r':
		for (int i = 0; i < 5; ++i)
			if (MyBox[i].bBoxReveal)
				MyBox[i].bBoxReveal = false;
		iBoxNum = 0;
		iBoxSize = 25;
		Animation = false;
		break;
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	default:

		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔때마다 출력 콜백함수를 호출하여 화면을 refresh 한다
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (iBoxNum < 5)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(0, 9);
			MyBox[iBoxNum].iRed = (float)(dis(gen)) / 10.0f;
			MyBox[iBoxNum].iGreen = (float)(dis(gen)) / 10.0f;
			MyBox[iBoxNum].iBlue = (float)(dis(gen)) / 10.0f;
			MyBox[iBoxNum].iOriginalX = x;
			MyBox[iBoxNum].iX = x;
			MyBox[iBoxNum].iOriginalY = y;
			MyBox[iBoxNum].iY = y;
			MyBox[iBoxNum].bBoxReveal = true;

			switch (dis(gen) % 4)
			{
			case 0:
				MyBox[iBoxNum].dir = LeftUp;
				break;
			case 1:
				MyBox[iBoxNum].dir = RightUp;
				break;
			case 2:
				MyBox[iBoxNum].dir = RightDown;
				break;
			case 3:
				MyBox[iBoxNum].dir = LeftDown;
				break;
			default:
				break;
			}
			
		
			if (iBoxNum == 4)
				iBoxNum = 0;
			else
				++iBoxNum;
		}
	}

	glutPostRedisplay();
}

void TimerFunction(int value)
{
	for (int i = 0; i < 5; ++i)
	{
		if (CommandC)
		{
			if (SizeUp)
			{
				iBoxSize += 1;
				if (iBoxSize > 50)
					SizeUp = false;
			}
			else
			{
				iBoxSize -= 1;
				if (iBoxSize < 15)
					SizeUp = true;

			}

		}
		if (MyBox[i].bBoxReveal)
		{
			switch (MyBox[i].dir)
			{
			case LeftUp:
				MyBox[i].iX -= 10;
				MyBox[i].iY -= 10;
				if (MyBox[i].iX - iBoxSize < 0)
					MyBox[i].dir = RightUp;
				if(MyBox[i].iY - iBoxSize < 0)
					MyBox[i].dir = LeftDown;
				break;
			case LeftDown:
				MyBox[i].iX -= 10;
				MyBox[i].iY += 10;
				if (MyBox[i].iX - iBoxSize < 0)
					MyBox[i].dir = RightDown;
				if (MyBox[i].iY + iBoxSize > 799)
					MyBox[i].dir = LeftUp;
				break;
			case RightUp:
				MyBox[i].iX += 10;
				MyBox[i].iY -= 10;
				if (MyBox[i].iX + iBoxSize > 799)
					MyBox[i].dir = LeftUp;
				if (MyBox[i].iY - iBoxSize < 0)
					MyBox[i].dir = RightDown;
				break;
			case RightDown:
				MyBox[i].iX += 10;
				MyBox[i].iY += 10;
				if (MyBox[i].iX + iBoxSize > 799)
					MyBox[i].dir = LeftDown;
				if (MyBox[i].iY + iBoxSize > 799)
					MyBox[i].dir = RightUp;
				break;
			default:
				break;
			}
		}
	}
	glutPostRedisplay();
	if(Animation)
		glutTimerFunc(50, TimerFunction, 1); // 타이머함수 재 설정
}