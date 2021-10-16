#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>

#define nTriangles 48

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);
GLvoid DrawScene();
void InitTriangle();

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader[2], fragmentShader[2];
GLuint s_program[2];
GLuint vao[2], vbo[4];
GLint width, height;

void KeyBoard(unsigned char key, int x, int y);
void DrawCase();
bool CommandNumber = false;
bool CommandAlpha = false;
bool IsTetrahedron = false;
int ShowIndex = 0;

GLfloat LineData[][3] = {
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},

	{0.0, -1.0, 0.0},
	{0.0, 1.0, 0.0}
};

GLfloat LineColor[][3] = {
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},

	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0}
};

GLfloat Dots[][3] = {
	{-0.5, 0.5, 0.5},
	{-0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5},
	{0.5, 0.5, 0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, -0.5, -0.5},
	{0.5, -0.5, -0.5},
	{0.5, 0.5, -0.5},

	{0.0, 0.5, 0.0},
	{-0.5, -0.25, -0.55},
	{-0.15, -0.25, 0.35},
	{0.5, -0.25, -0.35},

	{-1.0, 0.0, -1.0},
	{1.0, 0.0, -1.0},
	{0.0, 1.0, -1.0},
	{0.0, -1.0, -1.0},
};

GLfloat colors[][3] = {
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 1.0, 0.0 },
	{ 1.0, 0.0, 1.0 },
	{ 0.0, 1.0, 1.0 },

	{ 0.12, 0.71, 0.65 },
	{ 0.71, 0.46, 0.13 },
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.78, 0.41 }
};

unsigned int index[]{
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
	1, 6, 2,

	9, 11, 10,
	8, 9, 10,
	8, 10, 11,
	8, 11, 9
};

GLfloat Triangles[nTriangles][3];
GLfloat Triangles_Color[nTriangles][3];

void InitTriangle()
{
	for (int i = 0; i < 36; ++i)
	{
		Triangles[i][0] = Dots[index[i]][0];
		Triangles[i][1] = Dots[index[i]][1];
		Triangles[i][2] = Dots[index[i]][2];

		Triangles_Color[i][0] = colors[(int)(i / 6)][0];
		Triangles_Color[i][1] = colors[(int)(i / 6)][1];
		Triangles_Color[i][2] = colors[(int)(i / 6)][2];
	}

	for (int i = 36; i < nTriangles; ++i)
	{
		Triangles[i][0] = Dots[index[i]][0];
		Triangles[i][1] = Dots[index[i]][1];
		Triangles[i][2] = Dots[index[i]][2];

		Triangles_Color[i][0] = colors[6 + (i - 36) / 3][0];
		Triangles_Color[i][1] = colors[6 + (i - 36) / 3][1];
		Triangles_Color[i][2] = colors[6 + (i - 36) / 3][2];
	}
}

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	vertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[0], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[0]);

	vertexsource = filetobuf("vertex_line.glsl");
	vertexShader[1] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[1], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[1]);
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment.glsl");
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[0], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[0]);

	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[1]);
}

void InitBuffer()
{
	glGenVertexArrays(2, vao);
	glGenBuffers(4, vbo);

	glBindVertexArray(vao[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangles), Triangles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangles_Color), Triangles_Color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[1]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineData), LineData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineColor), LineColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
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

	glDeleteShader(vertexShader[0]);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(vertexShader[1]);
	glDeleteShader(fragmentShader[1]);
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program[1]);
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_LINES, 0, 4);

	glUseProgram(s_program[0]);
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(30.0), glm::vec3(-1.0, 0.0, 0.0));
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(30.0), glm::vec3(0.0, 1.0, 0.0));
	unsigned int modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glBindVertexArray(vao[0]);
	
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	DrawCase();
	glDisable (GL_DEPTH_TEST);
	glutSwapBuffers();
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
	InitTriangle();

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training13");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyBoard);
	glutMainLoop();
}

void KeyBoard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
		CommandAlpha = false;
		CommandNumber = true;
		IsTetrahedron = false;
		ShowIndex = (int)key - 49;
		break;
	case '7':
	case '8':
	case '9':
		CommandAlpha = false;
		CommandNumber = true;
		IsTetrahedron = true;
		ShowIndex = (int)key - 55;
		break;
	case '0':
		CommandAlpha = false;
		CommandNumber = true;
		IsTetrahedron = true;
		ShowIndex = 3;
		break;
	case 'a':
	case 'b':
	case 'c':
		CommandAlpha = true;
		CommandNumber = false;
		IsTetrahedron = false;
		ShowIndex = (int)key - 97;
		break;
	case 'e':
	case 'f':
	case 'g':
		CommandAlpha = true;
		CommandNumber = false;
		IsTetrahedron = true;
		ShowIndex = (int)key - 101;
		break;
	case VK_TAB:
		IsTetrahedron = !IsTetrahedron;
		break;
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void DrawCase()
{
	if (CommandAlpha)
	{
		if (IsTetrahedron)
		{
			glDrawArrays(GL_TRIANGLES, 36, 3);
			glDrawArrays(GL_TRIANGLES, 39 + ShowIndex * 3, 3);
		}
		else
			glDrawArrays(GL_TRIANGLES, ShowIndex * 12, 12);
	}

	else if (CommandNumber)
	{
		if (IsTetrahedron)
			glDrawArrays(GL_TRIANGLES, 36 + 3 * ShowIndex, 3);
		else
			glDrawArrays(GL_TRIANGLES, ShowIndex * 6, 6);
	}

	else
	{
		if(IsTetrahedron)
			glDrawArrays(GL_TRIANGLES, 36, 12);
		else
			glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}