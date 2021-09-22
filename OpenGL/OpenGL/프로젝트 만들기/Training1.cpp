#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLclampf fRed = 1.0f, fBlue = 1.0f, fGreen = 1.0f;
BOOL NotCommandS = FALSE;
void RandomColor();
void TimerFunction(int value);

void main(int argc, char** argv)					//---윈도우 출력하고 콜백함수 설정
{

	//---윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Training 1"); // 윈도우 생성(윈도우 이름)

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
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백함수 지정
	glutMainLoop(); // 이벤트 처리 시
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{ 
	glClearColor( fRed, fBlue, fGreen, 1.0f ); // 바탕색 설정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다

	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

void RandomColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 9);

	fRed = (float)(dis(gen)) / 10.0f;
	fBlue = (float)(dis(gen)) / 10.0f;
	fGreen = (float)(dis(gen)) / 10.0f;
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'R': 
	case 'r':
		fRed = 1.0f;
		fBlue = 0.0f;
		fGreen = 0.0f;
		break; //--- 배경색을 빨강색으로 설정
	case 'G':
	case 'g':
		fRed = 0.0f;
		fBlue = 0.0f;
		fGreen = 1.0f;
		break; //--- 배경색을 초록색으로 설정
	case 'B':
	case 'b':
		fRed = 0.0f;
		fBlue = 1.0f;
		fGreen = 0.0f;
		break; //--- 배경색을 파랑색으로 설정
	case 'A':
	case 'a':
		RandomColor();
		break; //--- 배경색을 랜덤하게 설정
	case 'W':
	case 'w':
		fRed = 1.0f;
		fBlue = 1.0f;
		fGreen = 1.0f;
		break; //--- 배경색을 흰색으로 설정
	case 'K':
	case 'k':
		fRed = 0.0f;
		fBlue = 0.0f;
		fGreen = 0.0f;
		break; //--- 배경색을 검은색으로 설정
	case 'T':
	case 't':
		NotCommandS = TRUE;
		glutTimerFunc(100, TimerFunction, 1);
		break; //--- 타이머 설정
	case 'S':
	case 's':
		NotCommandS = FALSE;
		break; //--- 타이머종료
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break; //--- 프로그램종료
	}
	glutPostRedisplay(); //--- 배경색이 바뀔때마다 출력 콜백함수를 호출하여 화면을 refresh 한다
}

void TimerFunction(int value)
{
	RandomColor();
	glutPostRedisplay();
	if (NotCommandS)
		glutTimerFunc(100, TimerFunction, 1); // 타이머함수 재 설정
}