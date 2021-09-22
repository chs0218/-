#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#include <fstream>
#include <random>

enum Direction {
	LeftUp = 0,
	RightUp,
	RightDown,
	LeftDown
};

enum Shape {
	ToLeft = 0,
	ToRight,
	ToTop,
	ToBottom
};
void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
void InitTriangle();
void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);
void Randomdir(int i);
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);
GLvoid DrawScene();
Direction dir[4];
Shape shape[4];
int iIndex = 0;

struct MyPoint {
	GLfloat Point_X;
	GLfloat Point_Y;
};

GLchar* vertexsource;
GLchar* fragmentsource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint s_program;
GLuint vao, vbo[2];
GLint width, height;

MyPoint myPoint[4] = {
	{-0.5, 0.5},
	{0.5, 0.5},
	{-0.5, -0.5},
	{0.5, -0.5}
};

GLfloat triShape[12][3];

GLfloat colors[12][3] = { //--- �ﰢ�� ������ ����
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.78, 0.41 },
	{ 0.95, 0.78, 0.41 },
	{ 0.95, 0.78, 0.41 },
	{ 0.91, 0.50, 0.21 },
	{ 0.91, 0.50, 0.21 },
	{ 0.91, 0.50, 0.21 },
	{ 0.53, 0.51, 0.64 },
	{ 0.53, 0.51, 0.64 },
	{ 0.53, 0.51, 0.64 } };

void InitTriangle()
{
	for (int i = 0; i < 4; ++i)
	{
		switch (shape[i])
		{
		case ToLeft:
			triShape[3 * i][0] = myPoint[i].Point_X - 0.1f;
			triShape[3 * i][1] = myPoint[i].Point_Y;
			triShape[3 * i + 1][0] = myPoint[i].Point_X + 0.1f;
			triShape[3 * i + 1][1] = myPoint[i].Point_Y - 0.1f;
			triShape[3 * i + 2][0] = myPoint[i].Point_X + 0.1f;
			triShape[3 * i + 2][1] = myPoint[i].Point_Y + 0.1f;
			break;
		case ToRight:
			triShape[3 * i][0] = myPoint[i].Point_X + 0.1f;
			triShape[3 * i][1] = myPoint[i].Point_Y;
			triShape[3 * i + 1][0] = myPoint[i].Point_X - 0.1f;
			triShape[3 * i + 1][1] = myPoint[i].Point_Y + 0.1f;
			triShape[3 * i + 2][0] = myPoint[i].Point_X - 0.1f;
			triShape[3 * i + 2][1] = myPoint[i].Point_Y - 0.1f;
			break;
		case ToTop:
			triShape[3 * i][0] = myPoint[i].Point_X - 0.1f;
			triShape[3 * i][1] = myPoint[i].Point_Y - 0.1f;
			triShape[3 * i + 1][0] = myPoint[i].Point_X + 0.1f;
			triShape[3 * i + 1][1] = myPoint[i].Point_Y - 0.1f;
			triShape[3 * i + 2][0] = myPoint[i].Point_X;
			triShape[3 * i + 2][1] = myPoint[i].Point_Y + 0.1f;
			break;
		case ToBottom:
			triShape[3 * i][0] = myPoint[i].Point_X;
			triShape[3 * i][1] = myPoint[i].Point_Y - 0.1f;
			triShape[3 * i + 1][0] = myPoint[i].Point_X + 0.1f;
			triShape[3 * i + 1][1] = myPoint[i].Point_Y + 0.1f;
			triShape[3 * i + 2][0] = myPoint[i].Point_X - 0.1f;
			triShape[3 * i + 2][1] = myPoint[i].Point_Y + 0.1f;
			break;
		default:

			break;
		}
	}
}

void Randomdir(int i)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 3);

	switch (dis(gen))
	{
	case 0:
		dir[i] = LeftUp;
		shape[i] = ToLeft;
		break;
	case 1:
		dir[i] = RightUp;
		shape[i] = ToRight;
		break;
	case 2:
		dir[i] = RightDown;
		shape[i] = ToRight;
		break;
	case 3:
		dir[i] = LeftDown;
		shape[i] = ToLeft;
		break;
	default:
		break;
	}

}

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, NULL);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
	}
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, NULL);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
	}
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(triShape), triShape, GL_DYNAMIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	//--- colors �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����

	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

void DrawScene() //--- glutDisplayFunc()�Լ��� ����� �׸��� �ݹ� �Լ�
{
	InitTriangle();
	InitBuffer();
	//--- ����� ���� ����
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(s_program);
	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao);
	//--- �ﰢ�� �׸���
	glDrawArrays(GL_TRIANGLES, 0, 18);
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		myPoint[iIndex].Point_X = ((GLfloat)x / (GLfloat)width - 0.5f) * 2.0f;
		myPoint[iIndex].Point_Y = ((GLfloat)y / (GLfloat)height - 0.5f) * -2.0f;
		iIndex += 1;
		Randomdir(iIndex);
		if (iIndex == 4)
			iIndex = 0;
	}
	glutPostRedisplay();
}

void TimerFunction(int value)
{
	for (int i = 0; i < 4; ++i)
	{
		switch (dir[i])
		{
		case LeftUp:
			myPoint[i].Point_X -= 0.05f;
			myPoint[i].Point_Y += 0.05f;
			if (myPoint[i].Point_X - 0.1f < -1.0f)
			{
				dir[i] = RightUp;
				if (shape[i] == ToTop)
					shape[i] = ToRight;
				else
					shape[i] = ToTop;
			}
			if (myPoint[i].Point_Y + 0.1f > 1.0f)
			{
				dir[i] = LeftDown;
				if (shape[i] == ToTop)
					shape[i] = ToLeft;
				else
					shape[i] = ToBottom;
			}
			break;
		case LeftDown:
			myPoint[i].Point_X -= 0.05f;
			myPoint[i].Point_Y -= 0.05f;
			if (myPoint[i].Point_X - 0.1f < -1.0f)
			{
				dir[i] = RightDown;
				if (shape[i] == ToBottom)
					shape[i] = ToRight;
				else
					shape[i] = ToBottom;
			}
			if (myPoint[i].Point_Y - 0.1f < -1.0f)
			{
				dir[i] = LeftUp;
				if (shape[i] == ToBottom)
					shape[i] = ToLeft;
				else
					shape[i] = ToTop;
			}
			break;
		case RightUp:
			myPoint[i].Point_X += 0.05f;
			myPoint[i].Point_Y += 0.05f;
			if (myPoint[i].Point_X + 0.1f > 1.0f)
			{
				dir[i] = LeftUp;
				if (shape[i] == ToTop)
					shape[i] = ToLeft;
				else
					shape[i] = ToTop;
			}
			if (myPoint[i].Point_Y + 0.1f > 1.0f)
			{
				dir[i] = RightDown;
				if (shape[i] == ToTop)
					shape[i] = ToRight;
				else
					shape[i] = ToBottom;
			}
			break;
		case RightDown:
			myPoint[i].Point_X += 0.05f;
			myPoint[i].Point_Y -= 0.05f;
			if (myPoint[i].Point_X + 0.1f > 1.0f)
			{
				dir[i] = LeftDown;
				if (shape[i] == ToBottom)
					shape[i] = ToLeft;
				else
					shape[i] = ToBottom;
			}
			if (myPoint[i].Point_Y - 0.1f < -1.0f)
			{
				dir[i] = RightUp;
				if (shape[i] == ToBottom)
					shape[i] = ToRight;
				else 
					shape[i] = ToTop;
			}
			break;
		default:
			break;
		}
		
	}
	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction, 1); // Ÿ�̸��Լ� �� ����
}

void main(int argc, char** argv)	//---������ ����ϰ� �ݹ��Լ� ����
{
	width = 900;
	height = 900;

	for (int i = 0; i < 4; ++i)
		Randomdir(i);
	InitTriangle();

	//---������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training2");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutMouseFunc(Mouse);
	glutReshapeFunc(Reshape);
	glutTimerFunc(50, TimerFunction, 1);
	glutMainLoop();
}











