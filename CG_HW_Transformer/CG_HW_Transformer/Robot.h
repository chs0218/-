#ifndef _Robot
#define _Robot
#include <random>
#include <ctime>

#define BOXSIZE 1.0
#define MAXNUM 10

enum State
{
	mainState,
	obstacleState,
	followState
};

class Robot {
	GLfloat transX = 0.0f, transY = 2.0f, transZ = 0.0f;
	GLfloat Speed = 0.05f, armlegR = 0.0f;
	glm::mat4 modeltransformMatrix = glm::mat4(1.0f);
	glm::mat4 faceMatrix = glm::mat4(1.0f);
	glm::mat4 bodyMatrix = glm::mat4(1.0f);
	glm::mat4 headMatrix = glm::mat4(1.0f);
	glm::mat4 leftarmMatrix = glm::mat4(1.0f);
	glm::mat4 rightarmMatrix = glm::mat4(1.0f);
	glm::mat4 leftlegMatrix = glm::mat4(1.0f);
	glm::mat4 rightlegMatrix = glm::mat4(1.0f);
	int nowState = obstacleState;
	bool armlegPlus = true, modelMove = false, jumpUp = false, jumpDown = true;
	int dir = 0;
	clock_t now = clock();

	GLfloat RecordX[10] = {};
	GLfloat RecordY[10] = {};
	GLfloat RecordZ[10] = {};
	int RecordDir[10] = {};
	bool RecordMove[10] = {};
public:
	Robot* target = nullptr;

	Robot() {
		transX = 0.0f;
		transY = 2.0f;
		transZ = 0.0f;
		Speed = 0.05f;
		armlegR = 0.0f;
		target = nullptr;
		dir = 0;
	}

	Robot(GLfloat x, GLfloat y, GLfloat z, int Dir) {
		transX = x;
		transY = 2.0f;
		transZ = z;
		Speed = 0.05f;
		armlegR = 0.0f;
		target = nullptr;
		dir = Dir;
	}

	void GetCommand(unsigned char key)
	{
		switch (key) {
		case 'S':
		case 's':
			if (dir == 0 && modelMove)
				modelMove = false;
			else
			{
				modelMove = true;
				dir = 0;
			}
			break;
		case 'W':
		case 'w':
			if (dir == 2 && modelMove)
				modelMove = false;
			else
			{
				modelMove = true;
				dir = 2;
			}
			break;
		case 'A':
		case 'a':
			if (dir == 3 && modelMove)
				modelMove = false;
			else
			{
				modelMove = true;
				dir = 3;
			}
			break;
		case 'D':
		case 'd':
			if (dir == 1 && modelMove)
				modelMove = false;
			else
			{
				modelMove = true;
				dir = 1;
			}
			break;
		case 'J':
		case 'j':
			if (!jumpUp)
				jumpUp = true;
			break;
		}
	}

	void automove()
	{
		if (!modelMove)
			modelMove = true;

		clock_t tmp = clock();

		if ((difftime(tmp, now) / 1000.0) > 2.0)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(0, 3);

			dir = dis(gen);
			now = tmp;
		}
	}

	int GetState() {
		return nowState;
	}

	void update() {
		switch (nowState)
		{
		case mainState:
			move();
			break;
		case obstacleState:
			break;
		case followState:
			Follow();
			break;
		}
	}

	void move() {
		for (int i = 9; i > 0; --i)
		{
			RecordX[i] = RecordX[i - 1];
			RecordY[i] = RecordY[i - 1];
			RecordZ[i] = RecordZ[i - 1];
			RecordDir[i] = RecordDir[i - 1];
			RecordMove[i] = RecordMove[i - 1];
		}
		RecordX[0] = transX, RecordY[0] = transY, RecordZ[0] = transZ;
		RecordDir[0] = dir, RecordMove[0] = modelMove;

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
				break;
			case 1:
				transX += Speed;
				if (transX > 8.0f)
					transX = transX - 16.0f;
				break;
			case 2:
				transZ -= Speed;
				if (transZ < -8.0f)
					transZ = transZ + 16.0f;
				break;
			case 3:
				transX -= Speed;
				if (transX < -8.0f)
					transX = transX + 16.0f;
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
			if (transY < 2.0)
				transY = 2.0;
		}

		UpdateMatrix();
	}

	void UpdateMatrix()
	{
		modeltransformMatrix = glm::mat4(1.0f);
		faceMatrix = glm::mat4(1.0f);
		bodyMatrix = glm::mat4(1.0f);
		headMatrix = glm::mat4(1.0f);
		leftarmMatrix = glm::mat4(1.0f);
		rightarmMatrix = glm::mat4(1.0f);
		leftlegMatrix = glm::mat4(1.0f);
		rightlegMatrix = glm::mat4(1.0f);

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
	}

	void RandomObjects()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(-600, 600);

		transX = (GLfloat)dis(gen) * 0.01;
		transZ = (GLfloat)dis(gen) * 0.01;

		while (transX < 3.0 && transX > -3.0)
			transX = (GLfloat)dis(gen) * 0.01;
		while (transZ < 3.0 && transZ > -3.0)
			transZ = (GLfloat)dis(gen) * 0.01;
	}

	void Follow()
	{
		for (int i = 9; i > 0; --i)
		{
			RecordX[i] = RecordX[i - 1];
			RecordY[i] = RecordY[i - 1];
			RecordZ[i] = RecordZ[i - 1];
			RecordDir[i] = RecordDir[i - 1];
			RecordMove[i] = RecordMove[i - 1];
		}
		RecordX[0] = transX, RecordY[0] = transY, RecordZ[0] = transZ;
		RecordDir[0] = dir, RecordMove[0] = modelMove;

		transX = target->getRecordX();
		transY = target->getRecordY();
		transZ = target->getRecordZ();
		dir = target->getRecordDir();
		modelMove = target->getRecordMove();

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
		}

		else
		{
			armlegR = 0.0f;
		}
		UpdateMatrix();
	}

	glm::mat4 getModelM()
	{
		return modeltransformMatrix;
	}

	glm::mat4 getFaceM()
	{
		return faceMatrix;
	}

	glm::mat4 getBodyM()
	{
		return bodyMatrix;
	}

	glm::mat4 getHeadM()
	{
		return headMatrix;
	}

	glm::mat4 getLeftArmM()
	{
		return leftarmMatrix;
	}

	glm::mat4 getRightArmM()
	{
		return rightarmMatrix;
	}

	glm::mat4 getLeftLegM()
	{
		return leftlegMatrix;
	}

	glm::mat4 getRightLegM()
	{
		return rightlegMatrix;
	}

	GLfloat getTransX() 
	{
		return transX;
	}

	GLfloat getTransY()
	{
		return transY;
	}

	GLfloat getTransZ()
	{
		return transZ;
	}

	GLfloat getRecordX()
	{
		return RecordX[9];
	}

	GLfloat getRecordY()
	{
		return RecordY[9];
	}

	GLfloat getRecordZ()
	{
		return RecordZ[9];
	}
	int getDir()
	{
		return dir;
	}

	int getRecordDir()
	{
		return RecordDir[9];
	}

	bool getRecordMove()
	{
		return RecordMove[9];
	}

	void SetFollow(Robot* Ftarget)
	{
		target = Ftarget;
	}

	void setState(int state)
	{
		nowState = state;
	}
};

Robot robots[10];
int Index = 0;
void InitRobot();
void CheckCollision();
#endif