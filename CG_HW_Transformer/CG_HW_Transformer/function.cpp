#include "function.h"

void InitBuffer()
{
	glGenVertexArrays(1, &ShapeVAO);
	glGenBuffers(1, &ShapeVBO);

	glBindVertexArray(ShapeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shapevertices), shapevertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glEnable(GL_DEPTH_TEST);
}

void Display()
{
	glm::vec3 cameraPos, cameraDirection;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::mat4 ObstacleMatrix = glm::mat4(1.0f);

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;
	int ambientLocation;
	int lightPosLocation;
	int lightColorLocation;
	int objColorLocation;
	int viewPosition;

	if (CommandN)
	{
		cameraPos = glm::vec3(robots[0].getTransX(), robots[0].getTransY(), robots[0].getTransZ());
		switch (robots[0].getDir())
		{
		case 0:
			cameraDirection = glm::vec3(robots[0].getTransX(), robots[0].getTransY(), robots[0].getTransZ() + 1.0f);
			break;
		case 1:
			cameraDirection = glm::vec3(robots[0].getTransX() + 1.0f, robots[0].getTransY(), robots[0].getTransZ());
			break;
		case 2:
			cameraDirection = glm::vec3(robots[0].getTransX(), robots[0].getTransY(), robots[0].getTransZ() - 1.0f);
			break;
		case 3:
			cameraDirection = glm::vec3(robots[0].getTransX() - 1.0f, robots[0].getTransY(), robots[0].getTransZ());
			break;
		default:
			break;
		}
	}
	else
	{
		cameraPos = glm::vec3(cameraX, cameraY, cameraZ);
		cameraDirection = glm::vec3(robots[0].getTransX(), robots[0].getTransY(), robots[0].getTransZ()); //--- 카메라 바라보는 방향
	}

	transformMatrix = glm::scale(transformMatrix, glm::vec3(8.0, 8.0, 8.0));
	transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0, -BOXSIZE, 0.0));


	viewLocation = glGetUniformLocation(s_program, "viewTransform");	//---뷰변환행렬
	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");	//---투영변환행렬
	lightPosLocation = glGetUniformLocation(s_program, "lightPos");		//---조명위치
	lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //---조명색깔
	viewPosition = glGetUniformLocation(s_program, "viewPos");	//--- 카메라 위치
	modelLocation = glGetUniformLocation(s_program, "modelTransform");	//---모델변환행렬
	objColorLocation = glGetUniformLocation(s_program, "objectColor"); //---오브젝트색상
	ambientLocation = glGetUniformLocation(s_program, "ambient");	//---빛 세기

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	projection = glm::perspective(glm::radians(65.0f), (float)g_window_w / (float)g_window_h, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(s_program);
	glBindVertexArray(ShapeVAO);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	glUniform3f(lightPosLocation, 0.0, 3.0, 8.0);
	if(CommandT)
		glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	else
		glUniform3f(lightColorLocation, 0.0, 0.0, 0.0);
	glUniform3f(viewPosition, 0.0, 0.0, 4.0);
	glUniform1f(ambientLocation, 0.3);

	// 바닥
	glUniform3f(objColorLocation, 0.7, 0.7, 0.7);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	for (int i = 0; i < MAXNUM; ++i)
	{
		if (robots[i].GetState() != obstacleState)
		{
			// skyblue
			glUniform3f(objColorLocation, 0.0, 1.0, 1.0);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(robots[i].getRightArmM()));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// blue
			glUniform3f(objColorLocation, 0.0, 0.0, 1.0);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(robots[i].getBodyM()));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(robots[i].getHeadM()));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// purple
			glUniform3f(objColorLocation, 1.0, 0.0, 1.0);

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(robots[i].getLeftArmM()));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// green
			glUniform3f(objColorLocation, 0.0, 1.0, 0.0);

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(robots[i].getFaceM()));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// yello
			glUniform3f(objColorLocation, 1.0, 1.0, 0.0);

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(robots[i].getLeftLegM()));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(robots[i].getRightLegM()));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		else
		{
			// red
			glUniform3f(objColorLocation, 1.0, 0.0, 0.0);

			ObstacleMatrix = glm::mat4(1.0f);
			ObstacleMatrix = glm::translate(ObstacleMatrix, glm::vec3(robots[i].getTransX(), -0.85 + BOXSIZE, robots[i].getTransZ()));
			ObstacleMatrix = glm::scale(ObstacleMatrix, glm::vec3(0.5, 0.3, 0.5));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ObstacleMatrix));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	for (int i = 0; i < OBS_NUM; ++i)
	{
		glUniform3f(objColorLocation, 0.0, 1.0, 0.0);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(rt_Obs[i].ReturnM()));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glutSwapBuffers();
}


void Reshape(int w, int h)
{
	g_window_w = w;
	g_window_h = h;
	glViewport(0, 0, w, h);
}

void TimerFunc(int value)
{
	for (int i = 0; i < MAXNUM; ++i)
	{
		robots[i].update();
	}

	for (int i = 0; i < OBS_NUM; ++i)
	{
		rt_Obs[i].update();
	}

	if (RotateY)
		RotateCameraCenterY();

	if (AutoMove)
	{
		robots[0].automove();
	}

	CheckCollision();
	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{	
	switch (key) {
	case 'S':
	case 's':
		std::cout << "s/S: Z축으로 + 이동\n";
		if (AutoMove)
			AutoMove = false;
		robots[0].GetCommand(key);
		break;
	case 'W':
	case 'w':
		std::cout << "w/W: Z축으로 - 이동\n";
		if (AutoMove)
			AutoMove = false;
		robots[0].GetCommand(key);
		break;
	case 'A':
	case 'a':
		std::cout << "a/A: X축으로 - 이동\n";
		if (AutoMove)
			AutoMove = false;
		robots[0].GetCommand(key);
		break;
	case 'D':
	case 'd':
		std::cout << "d/D: X축으로 + 이동\n";
		if (AutoMove)
			AutoMove = false;
		robots[0].GetCommand(key);
		break;
	case 'J':
	case 'j':
		std::cout << "j/J: 점프\n";
		if (AutoMove)
			AutoMove = false;
		robots[0].GetCommand(key);
		break;
	case 'M':
	case 'm':
		AutoMove = true;
		std::cout << "m/M: 주인공 자동 이동\n";
		break;
	case 'N':
	case 'n':
		CommandN = !CommandN;
		std::cout << "n/N: 시점 변환\n";
		break;
	case 'T':
	case 't':
		CommandT = !CommandT;
		std::cout << "t/T: 조명 on/off\n";
		break;
	case 'X':
		cameraX += 0.5f;
		std::cout << "X: 카메라 X축 +방향 이동\n";
		break;
	case 'x':
		cameraX -= 0.5f;
		std::cout << "x: 카메라 X축 -방향 이동\n";
		break;
	case 'Z':
		cameraZ += 0.5f;
		std::cout << "Z: 카메라 Z축 +방향 이동\n";
		break;
	case 'z':
		cameraZ -= 0.5f;
		std::cout << "z: 카메라 Z축 -방향 이동\n";
		break;
	case 'I':
	case 'i':
		RotateY = false, RotateMinus = false;
		std::cout << "i/I: 카메라 정지\n";
		break;
	case 'F':
	case 'f':
		cameraX = 0.0, cameraY = 3.0, cameraZ = 8.0;
		RotateY = false, RotateMinus = false;
		AutoMove = false, CommandT = true, CommandN = false;
		InitRobot();
		std::cout << "f/F: 초기화\n";
		break;
	case 'C':
		if (RotateY && RotateMinus)
			RotateY = false;
		else
		{
			RotateY = true;
			RotateMinus = true;
		}
		std::cout << "C: 카메라 Y축 시계방향 회전\n";
		break;
	case 'c':
		if(RotateY && !RotateMinus)
			RotateY = false;
		else
		{
			RotateY = true;
			RotateMinus = false;
		}
		std::cout << "c: 카메라 Y축 반시계방향 회전\n";
		break;
	case 'Q':
	case 'q':
		std::cout << "q/Q: 프로그램 종료\n";
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void InitVertices()
{
	for (int i = 0; i < 12; ++i)
	{
		shapevertices[18 * i] = Dots[Shapeindex[3 * i]][0];
		shapevertices[18 * i + 1] = Dots[Shapeindex[3 * i]][1];
		shapevertices[18 * i + 2] = Dots[Shapeindex[3 * i]][2];
		shapevertices[18 * i + 3] = normal[normalindex[3 * i]][0];
		shapevertices[18 * i + 4] = normal[normalindex[3 * i]][1];
		shapevertices[18 * i + 5] = normal[normalindex[3 * i]][2];

		shapevertices[18 * i + 6] = Dots[Shapeindex[3 * i + 1]][0];
		shapevertices[18 * i + 7] = Dots[Shapeindex[3 * i + 1]][1];
		shapevertices[18 * i + 8] = Dots[Shapeindex[3 * i + 1]][2];
		shapevertices[18 * i + 9] = normal[normalindex[3 * i + 1]][0];
		shapevertices[18 * i + 10] = normal[normalindex[3 * i + 1]][1];
		shapevertices[18 * i + 11] = normal[normalindex[3 * i + 1]][2];

		shapevertices[18 * i + 12] = Dots[Shapeindex[3 * i + 2]][0];
		shapevertices[18 * i + 13] = Dots[Shapeindex[3 * i + 2]][1];
		shapevertices[18 * i + 14] = Dots[Shapeindex[3 * i + 2]][2];
		shapevertices[18 * i + 15] = normal[normalindex[3 * i + 2]][0];
		shapevertices[18 * i + 16] = normal[normalindex[3 * i + 2]][1];
		shapevertices[18 * i + 17] = normal[normalindex[3 * i + 2]][2];
	}
}


void RotateCameraCenterY()
{
	GLfloat tmpX, tmpZ;
	if (RotateMinus)
	{
		tmpX = cameraZ * glm::sin(glm::radians(-1.0)) + cameraX * glm::cos(glm::radians(-1.0));
		tmpZ = cameraZ * glm::cos(glm::radians(-1.0)) - cameraX * glm::sin(glm::radians(-1.0));
	}
	else
	{
		tmpX = cameraZ * glm::sin(glm::radians(1.0)) + cameraX * glm::cos(glm::radians(1.0));
		tmpZ = cameraZ * glm::cos(glm::radians(1.0)) - cameraX * glm::sin(glm::radians(1.0));
	}

	cameraX = tmpX;
	cameraZ = tmpZ;
}
