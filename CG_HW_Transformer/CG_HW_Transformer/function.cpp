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
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glm::vec3 cameraPos = glm::vec3(cameraX, cameraY, cameraZ); //--- 카메라 위치
	//glm::vec3 cameraDirection = glm::vec3(transX, transY, transZ); //--- 카메라 바라보는 방향
	glm::vec3 cameraDirection = glm::vec3(0.0, 0.0, 0.0); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 ObstacleMatrix = glm::mat4(1.0f);
	glm::mat4 modeltransformMatrix = glm::mat4(1.0f);

	glm::mat4 faceMatrix = glm::mat4(1.0f);
	glm::mat4 bodyMatrix = glm::mat4(1.0f);
	glm::mat4 headMatrix = glm::mat4(1.0f);
	glm::mat4 leftarmMatrix = glm::mat4(1.0f);
	glm::mat4 rightarmMatrix = glm::mat4(1.0f);
	glm::mat4 leftlegMatrix = glm::mat4(1.0f);
	glm::mat4 rightlegMatrix = glm::mat4(1.0f);

	glm::mat4 FrontMatrix = glm::mat4(1.0f);

	unsigned int projectionLocation;
	unsigned int modelLocation;
	unsigned int viewLocation;
	int ambientLocation;
	int lightPosLocation;
	int lightColorLocation;
	int objColorLocation;
	int viewPosition;

	transformMatrix = glm::scale(transformMatrix, glm::vec3(8.0, 8.0, 8.0));
	transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0, -BOXSIZE, 0.0));

	modeltransformMatrix = glm::translate(modeltransformMatrix, glm::vec3(transX, -1.4 * BOXSIZE + transY, transZ));
	modeltransformMatrix = glm::rotate(modeltransformMatrix, (GLfloat)glm::radians(90.0 * (GLfloat)dir), glm::vec3(0.0, 1.0, 0.0));

	bodyMatrix = glm::translate(bodyMatrix, glm::vec3(0.0, 0.0, 0.0));
	bodyMatrix = glm::scale(bodyMatrix, glm::vec3(0.2, 0.3, 0.2));
	bodyMatrix = modeltransformMatrix * bodyMatrix;

	headMatrix = glm::translate(headMatrix, glm::vec3(0.0, BOXSIZE * 0.45, 0.0));
	headMatrix = glm::scale(headMatrix, glm::vec3(0.1, 0.15, 0.1));
	headMatrix = modeltransformMatrix * headMatrix;

	faceMatrix = glm::translate(faceMatrix, glm::vec3(0.0, BOXSIZE * 0.45, BOXSIZE * 0.1));
	faceMatrix = glm::scale(faceMatrix, glm::vec3(0.05, 0.05, 0.05));
	faceMatrix = modeltransformMatrix * faceMatrix;

	leftarmMatrix = glm::translate(leftarmMatrix, glm::vec3(-0.275 * BOXSIZE, 0.0, 0.0));
	leftarmMatrix = glm::translate(leftarmMatrix, glm::vec3(0.0, 0.075, 0.0));
	leftarmMatrix = glm::rotate(leftarmMatrix, (GLfloat)glm::radians(armlegR), glm::vec3(1.0, 0.0, 0.0));
	leftarmMatrix = glm::translate(leftarmMatrix, glm::vec3(0.0, -0.075, 0.0));
	leftarmMatrix = glm::rotate(leftarmMatrix, (GLfloat)glm::radians(-30.0), glm::vec3(0.0, 0.0, 1.0));
	leftarmMatrix = glm::scale(leftarmMatrix, glm::vec3(0.05, 0.15, 0.05));
	leftarmMatrix = modeltransformMatrix * leftarmMatrix;

	rightarmMatrix = glm::translate(rightarmMatrix, glm::vec3(0.275 * BOXSIZE, 0.0, 0.0));
	rightarmMatrix = glm::translate(rightarmMatrix, glm::vec3(0.0, 0.075, 0.0));
	rightarmMatrix = glm::rotate(rightarmMatrix, (GLfloat)glm::radians(-armlegR), glm::vec3(1.0, 0.0, 0.0));
	rightarmMatrix = glm::translate(rightarmMatrix, glm::vec3(0.0, -0.075, 0.0));
	rightarmMatrix = glm::rotate(rightarmMatrix, (GLfloat)glm::radians(30.0), glm::vec3(0.0, 0.0, 1.0));
	rightarmMatrix = glm::scale(rightarmMatrix, glm::vec3(0.05, 0.15, 0.05));
	rightarmMatrix = modeltransformMatrix * rightarmMatrix;

	leftlegMatrix = glm::translate(leftlegMatrix, glm::vec3(-0.1 * BOXSIZE, -BOXSIZE * 0.45, 0.0));
	leftlegMatrix = glm::translate(leftlegMatrix, glm::vec3(0.0, 0.15 * BOXSIZE, 0.0));
	leftlegMatrix = glm::rotate(leftlegMatrix, (GLfloat)glm::radians(-armlegR), glm::vec3(1.0, 0.0, 0.0));
	leftlegMatrix = glm::translate(leftlegMatrix, glm::vec3(0.0, -0.15 * BOXSIZE, 0.0));
	leftlegMatrix = glm::scale(leftlegMatrix, glm::vec3(0.05, 0.15, 0.05));
	leftlegMatrix = modeltransformMatrix * leftlegMatrix;

	rightlegMatrix = glm::translate(rightlegMatrix, glm::vec3(0.1 * BOXSIZE, -BOXSIZE * 0.45, 0.0));
	rightlegMatrix = glm::translate(rightlegMatrix, glm::vec3(0.0, 0.15 * BOXSIZE, 0.0));
	rightlegMatrix = glm::rotate(rightlegMatrix, (GLfloat)glm::radians(armlegR), glm::vec3(1.0, 0.0, 0.0));
	rightlegMatrix = glm::translate(rightlegMatrix, glm::vec3(0.0, -0.15 * BOXSIZE, 0.0));
	rightlegMatrix = glm::scale(rightlegMatrix, glm::vec3(0.05, 0.15, 0.05));
	rightlegMatrix = modeltransformMatrix * rightlegMatrix;

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
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	glUniform3f(viewPosition, 0.0, 0.0, 4.0);
	glUniform1f(ambientLocation, 0.3);

	// 바닥
	glUniform3f(objColorLocation, 0.7, 0.7, 0.7);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// skyblue
	glUniform3f(objColorLocation, 0.0, 1.0, 1.0);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(rightarmMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// blue
	glUniform3f(objColorLocation, 0.0, 0.0, 1.0);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(bodyMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(headMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// purple
	glUniform3f(objColorLocation, 1.0, 0.0, 1.0);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(leftarmMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// green
	glUniform3f(objColorLocation, 0.0, 1.0, 0.0);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(faceMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// yello
	glUniform3f(objColorLocation, 1.0, 1.0, 0.0);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(leftlegMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(rightlegMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// red
	glUniform3f(objColorLocation, 1.0, 0.0, 0.0);

	ObstacleMatrix = glm::mat4(1.0f);
	ObstacleMatrix = glm::translate(ObstacleMatrix, glm::vec3(RandomX[0], -0.85 + BOXSIZE, RandomZ[0]));
	ObstacleMatrix = glm::scale(ObstacleMatrix, glm::vec3(0.5, 0.3, 0.5));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ObstacleMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);


	ObstacleMatrix = glm::mat4(1.0f);
	ObstacleMatrix = glm::translate(ObstacleMatrix, glm::vec3(RandomX[1], -0.85 + BOXSIZE, RandomZ[1]));
	ObstacleMatrix = glm::scale(ObstacleMatrix, glm::vec3(0.5, 0.3, 0.5));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ObstacleMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

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
	if (OpenF)
	{
		if (Fradius > -89.0)
		{
			Fradius = (GLfloat)((int)(Fradius - 1.0f) % 90);
		}
	}

	if (CloseF)
	{
		Fradius = (GLfloat)((int)(Fradius + 1.0f) % 90);
		if (Fradius > -1.0)
		{
			CloseF = false;
		}
	}

	if (modelMove)
	{
		if (armlegPlus)
		{
			armlegR = armlegR + 2.0;
			if (armlegR >= 45.0)
				armlegPlus = false;
		}
		else
		{
			armlegR = armlegR - 2.0;
			if (armlegR <= -45.0)
				armlegPlus = true;
		}

		switch (dir)
		{
		case 0:
			transZ += Speed;
			if (transZ > 8.0f)
				transZ = transZ - 16.0f;
			if (CheckCollision())
				transZ -= Speed;
			break;
		case 1:
			transX += Speed;
			if (transX > 8.0f)
				transX = transX - 16.0f;
			if (CheckCollision())
				transX -= Speed;
			break;
		case 2:
			transZ -= Speed;
			if (transZ < -8.0f)
				transZ = transZ + 16.0f;
			if (CheckCollision())
				transZ += Speed;
			break;
		case 3:
			transX -= Speed;
			if (transX < -8.0f)
				transX = transX + 16.0f;
			if (CheckCollision())
				transX += Speed;
			break;
		default:
			break;
		}
	}

	else
	{
		armlegR = 0.0f;
	}
	if (jumpUp)
	{
		transY = transY + Speed;
		if (transY > 3.0)
			jumpUp = false;
	}

	else
	{
		transY = transY - Speed;
		if (CheckCollision())
			transY += Speed;
		if (transY < 2.0)
			transY = 2.0;
	}

	if (RotateY)
		RotateCameraCenterY();

	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{	
	switch (key) {
	case 's':
		if (dir == 0 && modelMove)
			modelMove = false;
		else
		{
			modelMove = true;
			dir = 0;
		}
		break;
	case 'w':
		if (dir == 2 && modelMove)
			modelMove = false;
		else
		{
			modelMove = true;
			dir = 2;
		}
		break;
	case 'a':
		if (dir == 3 && modelMove)
			modelMove = false;
		else
		{
			modelMove = true;
			dir = 3;
		}
		break;
	case 'd':
		if (dir == 1 && modelMove)
			modelMove = false;
		else
		{
			modelMove = true;
			dir = 1;
		}
		break;
	case 'X':
		cameraX += 0.5f;
		break;
	case 'x':
		cameraX -= 0.5f;
		break;
	case 'Z':
		cameraZ += 0.5f;
		break;
	case 'z':
		cameraZ -= 0.5f;
		break;
	case 'O':
	case 'o':
		if (OpenF)
		{
			OpenF = false;
			CloseF = true;
		}
		else
			OpenF = true;
		break;
	case 'I':
	case 'i':
		RandomObjects();
		cameraX = 0.0, cameraY = 0.0, cameraZ = 2.0, Fradius = 0.0f, armlegR = 0.0;
		transX = 0.0, transY = 0.0, transZ = 0.0;
		OpenF = false, CloseF = false, armlegPlus = true, modelMove = false, jumpUp = false, jumpDown = true, RotateY = false;
		dir = 0;
		break;
	case 'Y':
	case 'y':
		RotateY = !RotateY;
		break;
	case 'J':
	case 'j':
		if (!jumpUp)
			jumpUp = true;
		break;
	case 'Q':
	case 'q':
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

void RandomObjects()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(-600, 600);

	RandomX[0] = (GLfloat)dis(gen) * 0.01;
	RandomX[1] = (GLfloat)dis(gen) * 0.01;
	RandomZ[0] = (GLfloat)dis(gen) * 0.01;
	RandomZ[1] = (GLfloat)dis(gen) * 0.01;

	while (RandomX[0] < 3.0 && RandomX[0] > -3.0)
		RandomX[0] = (GLfloat)dis(gen) * 0.01;
	while (RandomX[1] < 3.0 && RandomX[1] > -3.0)
		RandomX[1] = (GLfloat)dis(gen) * 0.01;
	while (RandomZ[0] < 3.0 && RandomZ[0] > -3.0)
		RandomZ[0] = (GLfloat)dis(gen) * 0.01;
	while (RandomZ[1] < 3.0 && RandomZ[1] > -3.0)
		RandomZ[1] = (GLfloat)dis(gen) * 0.01;
}

void RotateCameraCenterY()
{
	GLfloat tmpX = cameraZ * glm::sin(glm::radians(1.0)) + cameraX * glm::cos(glm::radians(1.0));
	GLfloat tmpZ = cameraZ * glm::cos(glm::radians(1.0)) - cameraX * glm::sin(glm::radians(1.0));

	cameraX = tmpX;
	cameraZ = tmpZ;
}

bool CheckCollision()
{
	if (RandomX[0] - BOXSIZE * 0.5 < transX + BOXSIZE * 0.2 &&
		RandomX[0] + BOXSIZE * 0.5 > transX - BOXSIZE * 0.2 &&
		0.0 < -1.1 * BOXSIZE + transY &&
		0.3 > -2.05 * BOXSIZE + transY &&
		RandomZ[0] - BOXSIZE * 0.5 < transZ + BOXSIZE * 0.2 &&
		RandomZ[0] + BOXSIZE * 0.5 > transZ - BOXSIZE * 0.2)
		return true;

	if (RandomX[1] - BOXSIZE * 0.5 < transX + BOXSIZE * 0.2 &&
		RandomX[1] + BOXSIZE * 0.5 > transX - BOXSIZE * 0.2 &&
		0.0 < -1.05 * BOXSIZE + transY &&
		0.3 > -2.05 * BOXSIZE + transY &&
		RandomZ[1] - BOXSIZE * 0.5 < transZ + BOXSIZE * 0.2 &&
		RandomZ[1] + BOXSIZE * 0.5 > transZ - BOXSIZE * 0.2)
		return true;
	return false;
}