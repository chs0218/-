//#include <gl/glew.h>
//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <glm/glm.hpp>
//#include <glm/ext.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <iostream>
//#include <fstream>
//
//void make_vertexShaders();
//void make_fragmentShaders();
//void InitShader();
//GLvoid InitBuffer();
//GLvoid Reshape(int w, int h);
//char* filetobuf(const char* file);
//GLvoid DrawScene();
//void InitTriangle();
//
//GLchar* vertexsource;
//GLchar* fragmentsource;
//GLuint vertexShader, fragmentShader;
//GLuint s_program;
//GLuint vao, vbo[2], ebo[2];
//GLint width, height;
//
//GLfloat xradius = 0.0f, yradius = 0.0f;
//
//void KeyBoard(unsigned char key, int x, int y);
//
//GLfloat Dots[][3] = {
//	{-0.5, 0.5, 0.5},
//	{-0.5, -0.5, 0.5},
//	{0.5, -0.5, 0.5},
//	{0.5, 0.5, 0.5},
//	{-0.5, 0.5, -0.5},
//	{-0.5, -0.5, -0.5},
//	{0.5, -0.5, -0.5},
//	{0.5, 0.5, -0.5},
//
//	{-1.0, 0.0, 0.0},
//	{1.0, 0.0, 0.0},
//	{0.0, -1.0, 0.0},
//	{0.0, 1.0, 0.0},
//	{0.0, 0.0, -1.0},
//	{0.0, 0.0, 1.0}
//};
//
//GLfloat colors[][3] = {
//	{ 1.0, 0.0, 0.0 },
//	{ 0.0, 1.0, 0.0 },
//	{ 0.0, 0.0, 1.0 },
//	{ 1.0, 1.0, 0.0 },
//	{ 1.0, 0.0, 1.0 },
//	{ 0.0, 1.0, 1.0 },
//	{ 1.0, 1.0, 1.0 },
//	{ 0.0, 0.0, 0.0 }
//};
//
//unsigned int index[]{
//	3, 2, 0,
//	0, 2, 1,
//
//	4, 7, 3,
//	4, 3, 0,
//
//	3, 7, 2,
//	7, 6, 2,
//
//	4, 1, 5,
//	4, 0, 1,
//
//	1, 6, 5,
//	1, 2, 6,
//
//	7, 5, 6,
//	7, 4, 5,
//
//	8, 9,
//	10, 11,
//	12, 13
//};
//
//GLfloat Triangles[42][3];
//GLfloat Triangles_Color[42][3];
//
//void InitTriangle()
//{
//	for (int i = 0; i < 36; ++i)
//	{
//		Triangles[i][0] = Dots[index[i]][0];
//		Triangles[i][1] = Dots[index[i]][1];
//		Triangles[i][2] = Dots[index[i]][2];
//
//		Triangles_Color[i][0] = colors[(int)(i / 6)][0];
//		Triangles_Color[i][1] = colors[(int)(i / 6)][1];
//		Triangles_Color[i][2] = colors[(int)(i / 6)][2];
//	}
//
//	for (int i = 36; i < 42; ++i)
//	{
//		Triangles[i][0] = Dots[index[i]][0];
//		Triangles[i][1] = Dots[index[i]][1];
//		Triangles[i][2] = Dots[index[i]][2];
//
//		if (i % 2 == 0)
//		{
//			Triangles_Color[i][0] = 1.0f;
//			Triangles_Color[i][1] = 0.0f;
//			Triangles_Color[i][2] = 0.0f;
//		}
//		else
//		{
//			Triangles_Color[i][0] = 0.0f;
//			Triangles_Color[i][1] = 0.0f;
//			Triangles_Color[i][2] = 1.0f;
//		}
//	}
//}
//
//void make_vertexShaders()
//{
//	GLint result;
//	GLchar errorLog[512];
//
//	vertexsource = filetobuf("vertex.glsl");
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, NULL);
//	glCompileShader(vertexShader);
//
//	
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
//		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
//	}
//}
//
//void make_fragmentShaders()
//{
//	GLint result;
//	GLchar errorLog[512];
//
//	fragmentsource = filetobuf("fragment.glsl");
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, NULL);
//	glCompileShader(fragmentShader);
//
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
//		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
//	}
//}
//
//void InitBuffer()
//{
//	glGenVertexArrays(1, &vao);
//	glGenBuffers(4, vbo);
//	glGenBuffers(2, ebo);
//
//	// Array
//	glBindVertexArray(vao);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangles), Triangles, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangles_Color), Triangles_Color, GL_STATIC_DRAW);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(1);
//
//	glBindVertexArray(vao);
//
//	// Element Array
//	/*glBindVertexArray(vao);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(1);*/
//}
//
//void InitShader()
//{
//	make_vertexShaders();
//	make_fragmentShaders();
//
//	s_program = glCreateProgram();
//	glAttachShader(s_program, vertexShader);
//	glAttachShader(s_program, fragmentShader);
//	glLinkProgram(s_program);
//
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//}
//
//void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glUseProgram(s_program);
//	glBindVertexArray(vao);
//
//	glm::mat4 transformMatrix = glm::mat4(1.0f);
//	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(xradius), glm::vec3(1.0, 0.0, 0.0));
//	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(yradius), glm::vec3(0.0, 1.0, 0.0));
//	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
//	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
//
//	glEnable(GL_DEPTH_TEST);
//	glPolygonMode(GL_FRONT, GL_FILL);
//	glPolygonMode(GL_BACK, GL_LINE);
//	glDrawArrays(GL_TRIANGLES, 0, 36);
//	glDrawArrays(GL_LINES, 36, 42);
//	/*glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
//	glDisable(GL_DEPTH_TEST);
//	glutSwapBuffers();
//}
//
//GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
//{
//	glViewport(0, 0, w, h);
//}
//
//char* filetobuf(const char* file)
//{
//	std::ifstream TextFile;
//	int size;
//	char* str = NULL;
//	TextFile.open(file);
//
//	if (TextFile.is_open())
//	{
//		TextFile.seekg(0, std::ios::end);
//		size = TextFile.tellg();
//		str = new char[size];
//		for (int i = 0; i < size; ++i)
//			str[i] = '\0';
//		TextFile.seekg(0, std::ios::beg);
//		TextFile.read(str, size);
//		TextFile.close();
//		return str;
//	}
//	else
//		return NULL;
//}
//
//void main(int argc, char** argv)	//---윈도우 출력하고 콜백함수 설정
//{
//	width = 900;
//	height = 900;
//	InitTriangle();
//
//	//---윈도우 생성하기
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowPosition(100, 30);
//	glutInitWindowSize(width, height);
//	glutCreateWindow("test");
//
//	//--- GLEW 초기화하기
//	glewExperimental = GL_TRUE;
//	glewInit();
//	InitShader();
//	InitBuffer();
//	glutDisplayFunc(DrawScene);
//	glutReshapeFunc(Reshape);
//	glutKeyboardFunc(KeyBoard);
//	glutMainLoop();
//}
//
//void KeyBoard(unsigned char key, int x, int y)
//{
//	switch (key) {
//	case 'x':
//		xradius = (int)(xradius + 5.0) % 360;
//		break;
//	case 'y':
//		yradius = (int)(yradius + 5.0) % 360;
//		break;
//	}
//	glutPostRedisplay();
//}

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

void KeyBoard(unsigned char key, int x, int y);

GLfloat Dots[8][3] = {
	{-0.5, 0.5, 0.5},
	{-0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5},
	{0.5, 0.5, 0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, -0.5, -0.5},
	{0.5, -0.5, -0.5},
	{0.5, 0.5, -0.5},
};
GLfloat colors[8][3] = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
	{1.0, 0.0, 1.0},
	{0.0, 1.0, 1.0},
	{0.0, 0.0, 0.0},
	{1.0, 1.0, 1.0},
};
unsigned int index[36] = {
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
};

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
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);
	glm::mat4 transformMatrix0 = glm::mat4(1.0f);
	transformMatrix0 = glm::rotate(transformMatrix0, (GLfloat)glm::radians(30.0), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix0 = glm::rotate(transformMatrix0, (GLfloat)glm::radians(-30.0), glm::vec3(0.0, 1.0, 0.0));
	unsigned int modelLocation0 = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation0, 1, GL_FALSE, glm::value_ptr(transformMatrix0));
	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void KeyBoard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'N':
	case 'n':
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