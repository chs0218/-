#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <random>

#define BOXSIZE 0.5

void make_vertexShaders();
void make_fragmentShaders();
void InitShader();
GLvoid InitBuffer();
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);

GLchar* vertexsource;
GLchar* fragmentsource;
GLuint vertexShader, fragmentShader[6];
GLuint s_program[6];
GLuint vao[3], vbo[3], EBO[3];
GLint width, height;

GLUquadricObj* qobj;
GLfloat SphereX[5], SphereY[5], SphereZ[5];
GLfloat cameraX = 0.0, cameraY = 0.0, cameraZ = 2.0;
GLfloat CameraDirX = 0.0, CameraDirY = 0.0, CameraDirZ = 0.0;
bool SphereMoveX[5], SphereMoveY[5], SphereMoveZ[5], RotateY = false;;

glm::vec3 cameraPos = glm::vec3(cameraX, cameraY, cameraZ); //--- ī�޶� ��ġ
glm::vec3 cameraDirection = glm::vec3(CameraDirX, CameraDirY, CameraDirZ); //--- ī�޶� �ٶ󺸴� ����
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����

void DrawMain();
void MoveSphere();
void RotateCameraCenterY();
void RandomSphere();
void KeyBoard(unsigned char key, int x, int y);
void TimerFunc(int value);

GLfloat Dots[][3] = {
	// ����ü
	{-BOXSIZE, BOXSIZE, BOXSIZE},
	{-BOXSIZE, -BOXSIZE, BOXSIZE},
	{BOXSIZE, -BOXSIZE, BOXSIZE},
	{BOXSIZE, BOXSIZE, BOXSIZE},
	{-BOXSIZE, BOXSIZE, -BOXSIZE},
	{-BOXSIZE, -BOXSIZE, -BOXSIZE},
	{BOXSIZE, -BOXSIZE, -BOXSIZE},
	{BOXSIZE, BOXSIZE, -BOXSIZE}
};

unsigned int ShapeindexRHS[] = {
	// ����ü
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

unsigned int ShapeindexLHS[] = {
	// ����ü
	0, 2, 1,
	0, 3, 2,
	4, 5, 6,
	4, 6, 7,
	3, 6, 2,
	3, 7, 6,
	4, 1, 5,
	4, 0, 1,
	4, 3, 0,
	4, 7, 3,
	1, 6, 5,
	1, 2, 6
};

unsigned int Lineindex[] = {
	// ����ü
	0, 1,
	0, 3,
	0, 4,
	1, 2,
	1, 5,
	2, 3,
	2, 6,
	3, 7,
	4, 5,
	4, 7,
	5, 6,
	6, 7,
};

GLfloat Colors[][3] = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
	{0.0, 1.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 1.0, 1.0},
	{0.0, 0.0, 0.0}
};

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex_objects.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexShader);
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment_gray.glsl");
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[0], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[0]);

	fragmentsource = filetobuf("fragment_darkgray.glsl");
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[1]);

	fragmentsource = filetobuf("fragment_anothergray.glsl");
	fragmentShader[2] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[2], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[2]);

	fragmentsource = filetobuf("fragment_blue.glsl");
	fragmentShader[3] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[3], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[3]);

	fragmentsource = filetobuf("fragment_red.glsl");
	fragmentShader[4] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[4], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[4]);

	fragmentsource = filetobuf("fragment_black.glsl");
	fragmentShader[5] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[5], 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmentShader[5]);
}

void InitBuffer()
{
	glGenVertexArrays(2, vao);
	glGenBuffers(3, vbo);
	glGenBuffers(3, EBO);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ShapeindexRHS), ShapeindexRHS, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ShapeindexLHS), ShapeindexLHS, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Dots), Dots, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Lineindex), Lineindex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
}

void InitShader()
{
	make_vertexShaders();
	make_fragmentShaders();

	s_program[0] = glCreateProgram();

	glAttachShader(s_program[0], vertexShader);
	glAttachShader(s_program[0], fragmentShader[0]);
	glLinkProgram(s_program[0]);

	s_program[1] = glCreateProgram();

	glAttachShader(s_program[1], vertexShader);
	glAttachShader(s_program[1], fragmentShader[1]);
	glLinkProgram(s_program[1]);

	s_program[2] = glCreateProgram();

	glAttachShader(s_program[2], vertexShader);
	glAttachShader(s_program[2], fragmentShader[2]);
	glLinkProgram(s_program[2]);

	s_program[3] = glCreateProgram();

	glAttachShader(s_program[3], vertexShader);
	glAttachShader(s_program[3], fragmentShader[3]);
	glLinkProgram(s_program[3]);

	s_program[4] = glCreateProgram();

	glAttachShader(s_program[4], vertexShader);
	glAttachShader(s_program[4], fragmentShader[4]);
	glLinkProgram(s_program[4]);

	s_program[5] = glCreateProgram();

	glAttachShader(s_program[5], vertexShader);
	glAttachShader(s_program[5], fragmentShader[5]);
	glLinkProgram(s_program[5]);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);
	glDeleteShader(fragmentShader[2]);
	glDeleteShader(fragmentShader[3]);
	glDeleteShader(fragmentShader[4]);
	glDeleteShader(fragmentShader[5]);
}

void DrawScene() //--- glutDisplayFunc()�Լ��� ����� �׸��� �ݹ� �Լ�
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);*/
	glEnable(GL_DEPTH_TEST);
	DrawMain();
	glDisable(GL_DEPTH_TEST);
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

void main(int argc, char** argv)	//---������ ����ϰ� �ݹ��Լ� ����
{
	width = 900;
	height = 900;
	RandomSphere();

	//---������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training22");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(DrawScene);
	glutKeyboardFunc(KeyBoard);
	glutTimerFunc(25, TimerFunc, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void KeyBoard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'Z':
		cameraZ += 0.1f;
		break;
	case 'z':
		cameraZ -= 0.1f;
		break;
	case 'Y':
	case 'y':
		RotateY = !RotateY;
		break;
	case 'B':
	case 'b':
		RandomSphere();
		break;
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void TimerFunc(int value)
{
	if (RotateY)
		RotateCameraCenterY();
	MoveSphere();
	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void DrawMain()
{
	glm::mat4 SphereMatrix = glm::mat4(1.0f);
	glm::mat4 BoxMatrix[3] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;


	//projection = glm::ortho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
	projection = glm::perspective(glm::radians(130.0f), (float)width / (float)height, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -0.5));

	cameraPos = glm::vec3(cameraX, cameraY, cameraZ);
	cameraDirection = glm::vec3(CameraDirX, CameraDirY, CameraDirZ);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	transformMatrix = glm::scale(transformMatrix, glm::vec3(2.0, 2.0, 2.0));

	// ���� ����
	glUseProgram(s_program[0]);

	viewLocation = glGetUniformLocation(s_program[0], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[0], "modelTransform");

	glBindVertexArray(vao[1]);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));

	glUseProgram(s_program[1]);

	viewLocation = glGetUniformLocation(s_program[1], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[1], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[1], "modelTransform");

	glBindVertexArray(vao[1]);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)));

	glUseProgram(s_program[2]);

	viewLocation = glGetUniformLocation(s_program[2], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[2], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[2], "modelTransform");

	glBindVertexArray(vao[1]);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(24 * sizeof(GLuint)));

	// �� ����
	glUseProgram(s_program[3]);
	glBindVertexArray(vao[0]);

	viewLocation = glGetUniformLocation(s_program[3], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[3], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[3], "modelTransform");

	for (int i = 0; i < 5; ++i)
	{
		SphereMatrix = glm::mat4(1.0f);
		SphereMatrix = glm::translate(SphereMatrix, glm::vec3(SphereX[i], SphereY[i], SphereZ[i]));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(SphereMatrix));
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_FILL);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluSphere(qobj, 0.1, 20, 20);
	}

	// ����ü ����
	glUseProgram(s_program[4]);
	glBindVertexArray(vao[0]);

	BoxMatrix[0] = glm::translate(BoxMatrix[0], glm::vec3(-0.99 + BOXSIZE * 0.5f, -0.99 + BOXSIZE * 0.5f, -0.99 + BOXSIZE * 0.5f));
	BoxMatrix[0] = glm::scale(BoxMatrix[0], glm::vec3(0.5f, 0.5f, 0.5f));

	BoxMatrix[1] = glm::translate(BoxMatrix[1], glm::vec3(-0.99 + BOXSIZE * 0.25f, -0.99 + BOXSIZE * 0.25f, -0.99 + BOXSIZE * 1.5));
	BoxMatrix[1] = glm::scale(BoxMatrix[1], glm::vec3(0.25f, 0.25f, 0.25f));

	BoxMatrix[2] = glm::translate(BoxMatrix[2], glm::vec3(-0.99 + BOXSIZE * 0.125f, -0.99 + BOXSIZE * 0.125f, -0.99 + BOXSIZE * 2.0));
	BoxMatrix[2] = glm::scale(BoxMatrix[2], glm::vec3(0.125f, 0.125f, 0.125f));

	viewLocation = glGetUniformLocation(s_program[4], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[4], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	modelLocation = glGetUniformLocation(s_program[4], "modelTransform");
	
	for (int i = 0; i < 3; ++i)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(BoxMatrix[i]));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
	// ����ü �׵θ�
	glUseProgram(s_program[5]);
	glBindVertexArray(vao[2]);

	viewLocation = glGetUniformLocation(s_program[5], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program[5], "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	BoxMatrix[0] = glm::scale(BoxMatrix[0], glm::vec3(1.001f, 1.001f, 1.001f));
	BoxMatrix[1] = glm::scale(BoxMatrix[1], glm::vec3(1.001f, 1.001f, 1.001f));
	BoxMatrix[2] = glm::scale(BoxMatrix[2], glm::vec3(1.001f, 1.001f, 1.001f));

	modelLocation = glGetUniformLocation(s_program[5], "modelTransform");
	for (int i = 0; i < 3; ++i)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(BoxMatrix[i]));
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	}
}

void RandomSphere()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(-90, 90);
	std::uniform_int_distribution<int> tf(0, 1);

	for (int i = 0; i < 5; ++i)
	{
		SphereX[i] = (GLfloat)dis(gen) * 0.01;
		SphereY[i] = (GLfloat)dis(gen) * 0.01;
		SphereZ[i] = (GLfloat)dis(gen) * 0.01;
		SphereMoveX[i] = (bool)tf(gen);
		SphereMoveY[i] = (bool)tf(gen);
		SphereMoveZ[i] = (bool)tf(gen);
	}
}

void MoveSphere()
{
	for (int i = 0; i < 5; ++i)
	{
		if (SphereMoveX[i])
		{
			SphereX[i] += 0.05f;
			if (SphereX[i] > 0.89f)
			{
				SphereX[i] = 0.9f;
				SphereMoveX[i] = false;
			}
		}

		else
		{
			SphereX[i] -= 0.05f;
			if (SphereX[i] < -0.89)
			{
				SphereX[i] = -0.9f;
				SphereMoveX[i] = true;
			}
		}

		if (SphereMoveY[i])
		{
			SphereY[i] += 0.05f;
			if (SphereY[i] > 0.89f)
			{
				SphereY[i] = 0.9f;
				SphereMoveY[i] = false;
			}
		}

		else
		{
			SphereY[i] -= 0.05f;
			if (SphereY[i] < -0.89)
			{
				SphereY[i] = -0.9f;
				SphereMoveY[i] = true;
			}
		}

		if (SphereMoveZ[i])
		{
			SphereZ[i] += 0.05f;
			if (SphereZ[i] > 0.89f)
			{
				SphereZ[i] = 0.9f;
				SphereMoveZ[i] = false;
			}
		}

		else
		{
			SphereZ[i] -= 0.05f;
			if (SphereZ[i] < -0.89)
			{
				SphereZ[i] = -0.9f;
				SphereMoveZ[i] = true;
			}
		}
	}
}

void RotateCameraCenterY()
{
	GLfloat tmpX = cameraZ * glm::sin(glm::radians(1.0)) + cameraX * glm::cos(glm::radians(1.0));
	GLfloat tmpZ = cameraZ * glm::cos(glm::radians(1.0)) - cameraX * glm::sin(glm::radians(1.0));

	cameraX = tmpX;
	cameraZ = tmpZ;
}