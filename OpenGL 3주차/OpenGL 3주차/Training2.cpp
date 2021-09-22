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
GLchar* fragmentsource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
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

GLfloat colors[12][3] = { //--- 삼각형 꼭지점 색상
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
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, NULL);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
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
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, NULL);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
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
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(triShape), triShape, GL_DYNAMIC_DRAW);
	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//--- colors 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기

	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(s_program);
}

void DrawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	InitTriangle();
	InitBuffer();
	//--- 변경된 배경색 설정
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(s_program);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);
	//--- 삼각형 그리기
	glDrawArrays(GL_TRIANGLES, 0, 18);
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
	glutTimerFunc(50, TimerFunction, 1); // 타이머함수 재 설정
}

void main(int argc, char** argv)	//---윈도우 출력하고 콜백함수 설정
{
	width = 900;
	height = 900;

	for (int i = 0; i < 4; ++i)
		Randomdir(i);
	InitTriangle();

	//---윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 30);
	glutInitWindowSize(width, height);
	glutCreateWindow("Training2");

	//--- GLEW 초기화하기
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











