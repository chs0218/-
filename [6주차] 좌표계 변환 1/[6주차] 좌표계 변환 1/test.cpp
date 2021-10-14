#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>

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
GLuint vao, vbo[2], ebo[2];
GLint width, height;

GLfloat xradius = 0.0f, yradius = 0.0f;

void KeyBoard(unsigned char key, int x, int y);

GLfloat Dots[][3] = {
	{-0.5, 0.5, 0.5},
	{-0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5},
	{0.5, 0.5, 0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, -0.5, -0.5},
	{0.5, -0.5, -0.5},
	{0.5, 0.5, -0.5},
};

GLfloat colors[][3] = {
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 1.0, 0.0 },
	{ 1.0, 0.0, 1.0 },
	{ 0.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 0.0, 0.0, 0.0 }
};

unsigned int index[]{
	3, 2, 0,
	0, 2, 1,

	4, 7, 3,
	4, 3, 0,

	3, 7, 2,
	7, 6, 2,

	4, 1, 5,
	4, 0, 1,

	1, 6, 5,
	1, 2, 6,

	7, 5, 6,
	7, 4, 5
};

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader);
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(4, vbo);
	glGenBuffers(2, ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
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
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);
	glBindVertexArray(vao);

	glm::mat4 transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(xradius), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(yradius), glm::vec3(0.0, 1.0, 0.0));
	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT, GL_LINE);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDisable(GL_DEPTH_TEST);
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

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("test");

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
	case 'x':
		xradius = (int)(xradius + 5.0) % 360;
		break;
	case 'y':
		yradius = (int)(yradius + 5.0) % 360;
		break;
	}
	glutPostRedisplay();
}