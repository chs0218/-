#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#include <fstream>

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
void TimerFunction(int value);
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);
GLvoid DrawScene();

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader, fragmentShader;
GLuint s_program;
GLuint vao, vbo[2], EBO[2];
GLint width, height;


GLfloat Dots[21][3];
GLfloat colors[21][3];
int Animation[4];

unsigned int index[]{
	0, 1, 2,
	3, 4, 5,
	3, 5, 6,
	7, 8, 9,
	9, 10, 11,
	7, 9, 11,
	12, 13, 16,
	13, 14, 16,
	14, 15, 16,
	17, 18,
	19, 20
};

void InitDots();
void SetAnimation();

void main(int argc, char** argv)	//---윈도우 출력하고 콜백함수 설정
{
	width = 900;
	height = 900;

	InitDots();

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training6");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(50, TimerFunction, 1);
	glutMainLoop();
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

void DrawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);
	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, 0);
	
	glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, (void*)(27 * sizeof(GLuint)));
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void TimerFunction(int value)
{
	SetAnimation();
	InitBuffer();
	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction, 1); // 타이머함수 재 설정
}

void InitDots()
{
	// 선 -> 삼각형
	Dots[0][0] = -0.85;
	Dots[0][1] = 0.1;
	Dots[1][0] = -0.2;
	Dots[1][1] = 0.9;
	Dots[2][0] = -0.2;
	Dots[2][1] = 0.9;

	for (int i = 0; i < 3; ++i)
	{
		colors[i][0] = 0.95;
		colors[i][1] = 0.82;
		colors[i][2] = 0.71;
	}

	// 삼각형 -> 사각형
	Dots[3][0] = 0.2;
	Dots[3][1] = 0.2;
	Dots[4][0] = 0.8;
	Dots[4][1] = 0.2;
	Dots[5][0] = 0.5;
	Dots[5][1] = 0.8;
	Dots[6][0] = 0.5;
	Dots[6][1] = 0.8;

	for (int i = 3; i < 7; ++i)
	{
		colors[i][0] = 0.95;
		colors[i][1] = 0.78;
		colors[i][2] = 0.41;
	}

	// 사각형 -> 오각형
	Dots[7][0] = -0.7;
	Dots[7][1] = -0.9;
	Dots[8][0] = -0.3;
	Dots[8][1] = -0.9;
	Dots[9][0] = -0.3;
	Dots[9][1] = -0.3;
	Dots[10][0] = -0.5;
	Dots[10][1] = -0.3;
	Dots[11][0] = -0.7;
	Dots[11][1] = -0.3;

	for (int i = 7; i < 12; ++i)
	{
		colors[i][0] = 0.91;
		colors[i][1] = 0.50;
		colors[i][2] = 0.21;
	}

	// 오각형 -> 점
	Dots[12][0] = 0.5;
	Dots[12][1] = -0.2;
	Dots[13][0] = 0.15;
	Dots[13][1] = -0.43;
	Dots[14][0] = 0.3;
	Dots[14][1] = -0.8;
	Dots[15][0] = 0.7;
	Dots[15][1] = -0.8;
	Dots[16][0] = 0.85;
	Dots[16][1] = -0.43;

	for (int i = 12; i < 17; ++i)
	{
		colors[i][0] = 0.53;
		colors[i][1] = 0.51;
		colors[i][2] = 0.64;
	}

	// 경계선
	Dots[17][0] = -1.0;
	Dots[17][1] = 0;
	Dots[18][0] = 1.0;
	Dots[18][1] = 0;
	Dots[19][0] = 0;
	Dots[19][1] = 1.0;
	Dots[20][0] = 0;
	Dots[20][1] = -1.0;

	for (int i = 17; i < 21; ++i)
	{
		colors[i][0] = 0.0;
		colors[i][1] = 0.0;
		colors[i][2] = 0.0;
	}
}

void SetAnimation()
{
	// 선 -> 삼각형
	if (Animation[0] < 5)
		++Animation[0];

	else if (Animation[0] < 21)
	{
		++Animation[0];
		Dots[1][0] += 0.01;
		Dots[1][1] -= 0.05;
	}

	else if(Animation[0] > 35)
	{
		Animation[0] = 0;
		Dots[1][0] = -0.2;
		Dots[1][1] = 0.9;
	}

	else
		++Animation[0];

	// 삼각형 -> 사각형
	if (Animation[1] < 5)
		++Animation[1];

	else if (Animation[1] < 15)
	{
		++Animation[1];
		Dots[5][0] += 0.03;
		Dots[6][0] -= 0.03;
	}

	else if (Animation[1] > 30)
	{
		Animation[1] = 0;
		Dots[5][0] = 0.5;
		Dots[5][1] = 0.8;
		Dots[6][0] = 0.5;
		Dots[6][1] = 0.8;
	}

	else
		++Animation[1];

	// 사각형 -> 오각형
	if (Animation[2] < 5)
		++Animation[2];

	else if (Animation[2] < 15)
	{
		++Animation[2];
		Dots[7][0] += 0.001;
		Dots[7][1] += 0.01;
		Dots[8][0] -= 0.001;
		Dots[8][1] += 0.01;
		Dots[9][0] += 0.01;
		Dots[9][1] -= 0.01;
		Dots[10][1] += 0.02;
		Dots[11][0] -= 0.01;
		Dots[11][1] -= 0.01;
	}

	else if (Animation[2] > 30)
	{
		Animation[2] = 0;
		Dots[7][0] = -0.7;
		Dots[7][1] = -0.9;
		Dots[8][0] = -0.3;
		Dots[8][1] = -0.9;
		Dots[9][0] = -0.3;
		Dots[9][1] = -0.3;
		Dots[10][0] = -0.5;
		Dots[10][1] = -0.3;
		Dots[11][0] = -0.7;
		Dots[11][1] = -0.3;
	}

	else
		++Animation[2];

	// 오각형 -> 점
	if (Animation[3] < 5)
		++Animation[3];

	else if (Animation[3] < 23)
	{
		++Animation[3];
		Dots[12][1] -= 0.015;
		Dots[13][0] += 0.0175;
		Dots[13][1] -= 0.0035;
		Dots[14][0] += 0.01;
		Dots[14][1] += 0.015;
		Dots[15][0] -= 0.01;
		Dots[15][1] += 0.015;
		Dots[16][0] -= 0.0175;
		Dots[16][1] -= 0.0035;
	}

	else if (Animation[3] > 40)
	{
		Animation[3] = 0;
		Dots[12][0] = 0.5;
		Dots[12][1] = -0.2;
		Dots[13][0] = 0.15;
		Dots[13][1] = -0.43;
		Dots[14][0] = 0.3;
		Dots[14][1] = -0.8;
		Dots[15][0] = 0.7;
		Dots[15][1] = -0.8;
		Dots[16][0] = 0.85;
		Dots[16][1] = -0.43;
	}

	else
		++Animation[3];
}
