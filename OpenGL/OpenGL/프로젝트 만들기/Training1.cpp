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

void main(int argc, char** argv)					//---������ ����ϰ� �ݹ��Լ� ����
{

	//---������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("Training 1"); // ������ ����(������ �̸�)

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
	glutKeyboardFunc(Keyboard); // Ű���� �Է� �ݹ��Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ��
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{ 
	glClearColor( fRed, fBlue, fGreen, 1.0f ); // ������ ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
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
		break; //--- ������ ���������� ����
	case 'G':
	case 'g':
		fRed = 0.0f;
		fBlue = 0.0f;
		fGreen = 1.0f;
		break; //--- ������ �ʷϻ����� ����
	case 'B':
	case 'b':
		fRed = 0.0f;
		fBlue = 1.0f;
		fGreen = 0.0f;
		break; //--- ������ �Ķ������� ����
	case 'A':
	case 'a':
		RandomColor();
		break; //--- ������ �����ϰ� ����
	case 'W':
	case 'w':
		fRed = 1.0f;
		fBlue = 1.0f;
		fGreen = 1.0f;
		break; //--- ������ ������� ����
	case 'K':
	case 'k':
		fRed = 0.0f;
		fBlue = 0.0f;
		fGreen = 0.0f;
		break; //--- ������ ���������� ����
	case 'T':
	case 't':
		NotCommandS = TRUE;
		glutTimerFunc(100, TimerFunction, 1);
		break; //--- Ÿ�̸� ����
	case 'S':
	case 's':
		NotCommandS = FALSE;
		break; //--- Ÿ�̸�����
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break; //--- ���α׷�����
	}
	glutPostRedisplay(); //--- ������ �ٲ𶧸��� ��� �ݹ��Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

void TimerFunction(int value)
{
	RandomColor();
	glutPostRedisplay();
	if (NotCommandS)
		glutTimerFunc(100, TimerFunction, 1); // Ÿ�̸��Լ� �� ����
}