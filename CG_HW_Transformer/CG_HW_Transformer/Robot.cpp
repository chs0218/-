#include "Robot.h"

void CreateRobot(GLfloat x, GLfloat y, GLfloat z, int dir)
{
	if (robotNum < 10)
	{
		robots[robotNum] = Robot(x, y, z, dir);
		robots[robotNum].UpdateMatrix();
		++robotNum;
	}	
}

void RandomObjects()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(-600, 600);

	for (int i = 0; i < NUM; ++i)
	{
		RandomX[i] = (GLfloat)dis(gen) * 0.01;
		RandomZ[i] = (GLfloat)dis(gen) * 0.01;

		while (RandomX[i] < 3.0 && RandomX[i] > -3.0)
			RandomX[i] = (GLfloat)dis(gen) * 0.01;
		while (RandomX[i] < 3.0 && RandomX[i] > -3.0)
			RandomX[i] = (GLfloat)dis(gen) * 0.01;
	}
}

