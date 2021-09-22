#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLclampf fBoxRed = 1.0f, fBoxBlue = 1.0f, fBoxGreen = 1.0f;
GLclampf fBkRed = 0.0f, fBkBlue = 0.0f, fBkGreen = 0.0f;
void RandomBoxColor();
void RandomBkColor();
void Mouse(int button, int state, int x, int y);

void main(int argc, char** argv)					//---������ ����ϰ� �ݹ��Լ� ����
{

	//---������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(600, 600); // �������� ũ�� ����
	glutCreateWindow("Training 2"); // ������ ����(������ �̸�)

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
	glutMainLoop(); // �̺�Ʈ ó�� ��
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(fBkRed, fBkBlue, fBkGreen, 1.0f); // ������ ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�
	glColor3f(fBoxRed, fBoxGreen, fBoxBlue);
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

void RandomBkColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 9);

	fBkRed = (float)(dis(gen)) / 10.0f;
	fBkBlue = (float)(dis(gen)) / 10.0f;
	fBkGreen = (float)(dis(gen)) / 10.0f;
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

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (x >= 150 && x <= 450 && y >= 150 && y <= 450)
			RandomBoxColor();

		else
			RandomBkColor();
	}
	glutPostRedisplay();
}