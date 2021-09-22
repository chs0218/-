#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define WIN_SIZE 800
#define BOX_SIZE 50

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLclampf IntToFloat(int i, bool IsX);
void RandomBoxColor();
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

GLclampf fBoxRed = 1.0f, fBoxBlue = 1.0f, fBoxGreen = 1.0f;
int iBoxX = 400, iBoxY = 400;
bool LeftButton = false;

void main(int argc, char** argv)					//---������ ����ϰ� �ݹ��Լ� ����
{

	//---������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(WIN_SIZE, WIN_SIZE); // �������� ũ�� ����
	glutCreateWindow("Training 3"); // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse); // ���콺 �Է� �ݹ��Լ� ����
	glutMotionFunc(Motion);
	glutMainLoop(); // �̺�Ʈ ó�� ��
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f); // ������ ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�
	glColor3f(fBoxRed, fBoxGreen, fBoxBlue);
	glRectf(IntToFloat(iBoxX - BOX_SIZE, true), IntToFloat(iBoxY + BOX_SIZE, false), IntToFloat(iBoxX + BOX_SIZE, true), IntToFloat(iBoxY - BOX_SIZE, false));

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
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

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (x >= iBoxX - BOX_SIZE && x <= iBoxX + BOX_SIZE && y >= iBoxY - BOX_SIZE && y <= iBoxY + BOX_SIZE)
		{
			RandomBoxColor();
		}

		else
		{
			RandomBoxColor();
			iBoxX = x;
			iBoxY = y;
		}


		LeftButton = true;
	}

	if (state == GLUT_UP)
	{
		LeftButton = false;
	}
	glutPostRedisplay();
}

void Motion(int x, int y)
{
	if (LeftButton == true)
	{
		iBoxX = x;
		iBoxY = y;
	}
	glutPostRedisplay();
}

void RandomBoxColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 9);

	fBoxRed = (float)(dis(gen)) / 10.0f;
	fBoxBlue = (float)(dis(gen)) / 10.0f;
	fBoxGreen = (float)(dis(gen)) / 10.0f;
}