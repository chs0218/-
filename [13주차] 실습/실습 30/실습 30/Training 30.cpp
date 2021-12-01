#include <iostream>
#include <fstream>
#include <random>
#include "shape.h"
#include "shape.cpp"

#define BOXSIZE 0.25

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader[2], fragmentShader[2];
GLint width, height;

bool RotateY = false, RotateX = false;
bool ShowHexahedron = true;
GLfloat Yradius = 0.0f, Xradius = 0.0f;
void KeyBoard(unsigned char key, int x, int y);
void TimerFunc(int value);
void DrawMain();

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");
	vertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[0], 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader[0]);

	vertexsource = filetobuf("vertex_object.glsl");
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

	fragmentsource = filetobuf("fragment_object.glsl");
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[1]);
}

void InitBuffer()
{
	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Lines), Lines, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shapevertices), shapevertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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

	glDeleteShader(vertexShader[1]);
	glDeleteShader(fragmentShader[1]);
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	DrawMain();
	glDisable(GL_DEPTH_TEST);
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

	InitVertices();

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training30");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	InitTexture();
	glutDisplayFunc(DrawScene);
	glutKeyboardFunc(KeyBoard);
	glutTimerFunc(25, TimerFunc, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void KeyBoard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'C':
	case 'c':
		ShowHexahedron = false;
		break;
	case 'P':
	case 'p':
		ShowHexahedron = true;
		break;
	case 'X':
	case 'x':
		RotateX = !RotateX;
		break;
	case 'Y':
	case 'y':
		RotateY = !RotateY;
		break;
	case 'S':
	case 's':
		RotateY = false, RotateX = false;
		ShowHexahedron = true;
		Yradius = 0.0f, Xradius = 0.0f;
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

void TimerFunc(int value)
{
	if (RotateX)
		Xradius = (GLfloat)((int)(Xradius + 5.0f) % 360);

	if (RotateY)
		Yradius = (GLfloat)((int)(Yradius + 5.0f) % 360);
	
	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void DrawMain()
{
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 projection = glm::mat4(1.0f);
	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;
	int tLocation;
	projection = glm::perspective(glm::radians(65.0f), (float)width / (float)height, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -1.0));

	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(30.0), glm::vec3(1.0, -1.0, 0.0));
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	glUseProgram(s_program[0]);
	glBindVertexArray(vao[0]);

	viewLocation = glGetUniformLocation(s_program[0], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

	glDrawArrays(GL_LINES, 0, 6);

	glUseProgram(s_program[1]);
	glBindVertexArray(vao[1]);

	

	viewLocation = glGetUniformLocation(s_program[1], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[1], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

	tLocation = glGetUniformLocation(s_program[1], "outTexture");
	glUniform1i(tLocation, 0);

	transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0, 0.0, -10.0));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(5.0, 5.0, 5.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(Xradius), glm::vec3(1.0, 0.0, 0.0));
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(Yradius), glm::vec3(0.0, 1.0, 0.0));
	transformMatrix = glm::rotate(transformMatrix, (GLfloat)glm::radians(30.0), glm::vec3(1.0, -1.0, 0.0));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(0.3, 0.3, 0.3));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

	if (ShowHexahedron)
	{
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glDrawArrays(GL_TRIANGLES, 6, 6);

		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glDrawArrays(GL_TRIANGLES, 12, 6);

		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glDrawArrays(GL_TRIANGLES, 18, 6);

		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glDrawArrays(GL_TRIANGLES, 24, 6);

		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glDrawArrays(GL_TRIANGLES, 30, 6);
	}

	else
	{
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glDrawArrays(GL_TRIANGLES, 36, 3);

		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glDrawArrays(GL_TRIANGLES, 39, 3);

		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glDrawArrays(GL_TRIANGLES, 42, 3);

		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glDrawArrays(GL_TRIANGLES, 45, 3);

		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glDrawArrays(GL_TRIANGLES, 30, 6);
	}
}