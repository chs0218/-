#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>

#define nTriangles 52

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
GLuint vertexShader, fragmentShader;
GLuint s_program;
GLuint vao, vbo[2];
GLint width, height;

void KeyBoard(unsigned char key, int x, int y);
void DrawCase();
bool CommandNumber = false;
bool CommandAlpha = false;
bool IsTetrahedron = false;
int ShowIndex = 0;

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
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.78, 0.41 },
	{ 0.91, 0.50, 0.21 },
	{ 0.53, 0.51, 0.64 },
	{ 0.47, 0.60, 0.50 },
	{ 0.33, 0.78, 0.82 },

	{ 0.12, 0.71, 0.65 },
	{ 0.71, 0.46, 0.13 },
	{ 0.95, 0.82, 0.71 },
	{ 0.95, 0.78, 0.41 }
};

unsigned int index[]{
	0, 1, 2,
	0, 2, 3,
	4, 5, 6,
	4, 6, 7,
	3, 2, 6,
	3, 6, 7,
	4, 5, 1,
	4, 1, 0,
	4, 0, 3,
	4, 3, 7,
	1, 6, 5,
	1, 6, 2,

	9, 11, 10,
	8, 9, 10,
	8, 10, 11,
	8, 11, 9,

	12, 13,
	14, 15
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

	for (int i = 36; i < 48; ++i)
	{
		Triangles[i][0] = Dots[index[i]][0];
		Triangles[i][1] = Dots[index[i]][1];
		Triangles[i][2] = Dots[index[i]][2];

		Triangles_Color[i][0] = colors[6 + (i - 36) / 3][0];
		Triangles_Color[i][1] = colors[6 + (i - 36) / 3][1];
		Triangles_Color[i][2] = colors[6 + (i - 36) / 3][2];
	}

	for (int i = 48; i < 52; ++i)
	{
		Triangles[i][0] = Dots[index[i]][0];
		Triangles[i][1] = Dots[index[i]][1];
		Triangles[i][2] = Dots[index[i]][2];

		if (i < 50)
		{
			Triangles_Color[i][0] = 1.0f;
			Triangles_Color[i][1] = 0.0f;
			Triangles_Color[i][2] = 0.0f;
		}
		else 
		{
			Triangles_Color[i][0] = 0.0f;
			Triangles_Color[i][1] = 0.0f;
			Triangles_Color[i][2] = 1.0f;
		}
	}
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

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangles), Triangles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangles_Color), Triangles_Color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
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
	glUseProgram(s_program);
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(30.0), glm::vec3(-1.0, 1.0, 0.0));
	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glBindVertexArray(vao);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
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