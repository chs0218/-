#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#include <fstream>
#include <random>

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader, fragmentShader;
GLuint s_program;
GLuint vao, vbo[2], EBO[2];
GLint width, height;


void SetRectangle();
void SetColor();
void SetIndex();
void SetEraser();
void CheckCollision();
bool ButtonDown = false;
void KeyBoard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

GLfloat Dots[404][3];
GLfloat colors[404][3];
unsigned int index[606];

GLfloat BOX_SIZE = 0.03f;
GLfloat MainX;
GLfloat MainY;

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
	}
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
	}
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);
	glGenBuffers(2, EBO);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShaders();
	make_fragmentShaders();

	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(s_program);
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	SetEraser();
	InitBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);
	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_FILL);
	if(ButtonDown)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 600, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));
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
	SetRectangle();
	SetColor();
	SetIndex();

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
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void SetRectangle()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 900);

	GLfloat float_X;
	GLfloat float_Y;

	for (int i = 1; i < 101; ++i)
	{
		float_X = ((GLfloat)dis(gen) / (GLfloat)width - 0.5f) * 2.0f;
		float_Y = ((GLfloat)dis(gen) / (GLfloat)height - 0.5f) * -2.0f;
		Dots[i * 4][0] = float_X - BOX_SIZE;
		Dots[i * 4][1] = float_Y + BOX_SIZE;
		Dots[i * 4][2] = -0.5f;
		Dots[i * 4 + 1][0] = float_X - BOX_SIZE;
		Dots[i * 4 + 1][1] = float_Y - BOX_SIZE;
		Dots[i * 4 + 1][2] = -0.5f;
		Dots[i * 4 + 2][0] = float_X + BOX_SIZE;
		Dots[i * 4 + 2][1] = float_Y - BOX_SIZE;
		Dots[i * 4 + 2][2] = -0.5f;
		Dots[i * 4 + 3][0] = float_X + BOX_SIZE;
		Dots[i * 4 + 3][1] = float_Y + BOX_SIZE;
		Dots[i * 4 + 3][2] = -0.5f;

	}
}

void SetColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 255);

	GLfloat float_R;
	GLfloat float_G;
	GLfloat float_B;

	for (int i = 0; i < 4; ++i)
	{
		colors[i][0] = 1.0f;
		colors[i][1] = 1.0f;
		colors[i][2] = 1.0f;
	}

	for (int i = 1; i < 101; ++i)
	{
		float_R = (GLfloat)dis(gen) / (GLfloat)255;
		float_G = (GLfloat)dis(gen) / (GLfloat)255;
		float_B = (GLfloat)dis(gen) / (GLfloat)255;

		for (int j = 0; j < 4; ++j)
		{
			colors[i * 4 + j][0] = float_R;
			colors[i * 4 + j][1] = float_G;
			colors[i * 4 + j][2] = float_B;
		}
	}
}

void SetIndex()
{
	for (int i = 0; i < 101; ++i)
	{
		index[i * 6] = i * 4;
		index[i * 6 + 1] = i * 4 + 1;
		index[i * 6 + 2] = i * 4 + 2;
		index[i * 6 + 3] = i * 4;
		index[i * 6 + 4] = i * 4 + 2;
		index[i * 6 + 5] = i * 4 + 3;
	}
}

void SetEraser()
{
	Dots[0][0] = MainX - BOX_SIZE * 2.0;
	Dots[0][1] = MainY + BOX_SIZE * 2.0;
	Dots[1][0] = MainX - BOX_SIZE * 2.0;
	Dots[1][1] = MainY - BOX_SIZE * 2.0;
	Dots[2][0] = MainX + BOX_SIZE * 2.0;
	Dots[2][1] = MainY - BOX_SIZE * 2.0;
	Dots[3][0] = MainX + BOX_SIZE * 2.0;
	Dots[3][1] = MainY + BOX_SIZE * 2.0;
}

void CheckCollision()
{

	for (int i = 1; i < 101; ++i)
	{
		if (Dots[i * 4 + 1][0] < Dots[3][0] && Dots[i * 4 + 1][1] < Dots[3][1] && Dots[i * 4 + 3][0] > Dots[1][0] && Dots[i * 4 + 3][1] > Dots[1][1])
		{
			Dots[i * 4][0] = 0.0f;
			Dots[i * 4][1] = 0.0f;
			Dots[i * 4 + 1][0] = 0.0f;
			Dots[i * 4 + 1][1] = 0.0f;
			Dots[i * 4 + 2][0] = 0.0f;
			Dots[i * 4 + 2][1] = 0.0f;
			Dots[i * 4 + 3][0] = 0.0f;
			Dots[i * 4 + 3][1] = 0.0f;
		}
	}
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		ButtonDown = true;
		MainX = ((GLfloat)x / (GLfloat)width - 0.5f) * 2.0f;
		MainY = ((GLfloat)y / (GLfloat)height - 0.5f) * -2.0f;
	}

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		ButtonDown = false;
	}
}

void Motion(int x, int y)
{
	if (ButtonDown)
	{
		MainX = ((GLfloat)x / (GLfloat)width - 0.5f) * 2.0f;
		MainY = ((GLfloat)y / (GLfloat)height - 0.5f) * -2.0f;
		CheckCollision();
	}

	glutPostRedisplay();
}

void KeyBoard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'N':
	case 'n':
		SetRectangle();
		SetColor();
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