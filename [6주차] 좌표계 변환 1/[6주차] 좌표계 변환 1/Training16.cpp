#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>

#define nTriangles 36

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
GLuint vertexShader[2], fragmentShader;
GLuint s_program[2];
GLuint vao[2], vbo[4], EBO[2];
GLint width, height;

void KeyBoard(unsigned char key, int x, int y);
void DrawCase();
bool CommandNumber = false;
bool CommandAlpha = false;
bool IsTetrahedron = false;
int ShowIndex = 0;

GLfloat LineData[][3] = {
	{-1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},

	{0.0, -1.0, 0.0},
	{0.0, 1.0, 0.0},

	{0.0, 0.0, -1.0},
	{0.0, 0.0, 1.0}
};

GLfloat LineColor[][3] = {
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},

	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},

	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0}
};

GLfloat Dots[][3] = {
	{-0.15, 0.15, 0.15},
	{-0.15, -0.15, 0.15},
	{0.15, -0.15, 0.15},
	{0.15, 0.15, 0.15},
	{-0.15, 0.15, -0.15},
	{-0.15, -0.15, -0.15},
	{0.15, -0.15, -0.15},
	{0.15, 0.15, -0.15}
};

GLfloat colors[][3] = {
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 1.0, 0.0 },
	{ 1.0, 0.0, 1.0 },
	{ 0.0, 1.0, 1.0 },
	{ 0.5, 0.5, 0.5 },
	{ 1.0, 1.0, 1.0 }
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
	1, 6, 2
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
}

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	vertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[0], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[0]);

	vertexsource = filetobuf("vertex_opposite.glsl");
	vertexShader[1] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[1], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[1]);
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
	glGenVertexArrays(2, vao);
	glGenBuffers(4, vbo);
	glGenBuffers(2, EBO);

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineData), LineData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineColor), LineColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[1]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	
}

void InitShader()
{
	make_vertexShaders();
	make_fragmentShaders();

	s_program[0] = glCreateProgram();
	glAttachShader(s_program[0], vertexShader[0]);
	glAttachShader(s_program[0], fragmentShader);
	glLinkProgram(s_program[0]);

	s_program[1] = glCreateProgram();
	glAttachShader(s_program[1], vertexShader[1]);
	glAttachShader(s_program[1], fragmentShader);
	glLinkProgram(s_program[1]);
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 tmp1 = glm::mat4(1.0f);
	glm::mat4 tmp2 = glm::mat4(1.0f);

	glUseProgram(s_program[0]);
	glBindVertexArray(vao[0]);
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(30.0), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(30.0), glm::vec3(0.0, 1.0, 0.0));
	unsigned int modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glBindVertexArray(vao[0]);

	glDrawArrays(GL_LINES, 0, 6);

	tmp1 = glm::translate(transformMatrix, glm::vec3(-0.5, 0.0, 0.0));

	glUseProgram(s_program[1]);

	glm::mat4 transformMatrix1 = glm::mat4(1.0f);
	transformMatrix1 = tmp1 * transformMatrix1;
	unsigned int modelLocation1 = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation1, 1, GL_FALSE, glm::value_ptr(transformMatrix1));
	glBindVertexArray(vao[1]);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
	}
	glutPostRedisplay();
}

void DrawCase()
{
	
}