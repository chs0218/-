#include "Robot.h"

void InitRobot()
{
	robots[0] = Robot(0.0, 2.0, 0.0, 0);
	robots[0].UpdateMatrix();
	robots[0].setState(mainState);
	Index = 0;

	for (int i = 1; i < MAXNUM; ++i)
	{
		robots[i] = Robot();
		robots[i].setState(obstacleState);
		robots[i].RandomObjects();
		robots[i].UpdateMatrix();
	}
}

void CheckCollision()
{
	for (int i = 1; i < MAXNUM; ++i)
	{
		if (robots[i].getTransX() - BOXSIZE * 0.5 < robots[0].getTransX() + BOXSIZE * 0.2 &&
			robots[i].getTransX() + BOXSIZE * 0.5 > robots[0].getTransX() - BOXSIZE * 0.2 &&
			0.0 < -1.1 * BOXSIZE + robots[0].getTransY() &&
			0.3 > -2.05 * BOXSIZE + robots[0].getTransY() &&
			robots[i].getTransZ() - BOXSIZE * 0.5 < robots[0].getTransZ() + BOXSIZE * 0.2 &&
			robots[i].getTransZ() + BOXSIZE * 0.5 > robots[0].getTransZ() - BOXSIZE * 0.2 &&
			robots[i].GetState() == obstacleState)
		{
			robots[i].setState(followState);
			robots[i].SetFollow(&robots[Index]);
			Index = i;
		}
	}
}