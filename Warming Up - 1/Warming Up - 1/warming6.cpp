#include <iostream>
#include <iomanip>
#include <random>
#include <time.h>

void MakeRoad(int iMap[][13])
{
	srand((unsigned int)time(0));

	int iLocationX = 0;
	int iLocationY = 0;
	int iVec[4] = {};
	int iSeed = 0;
	int iDistance = 0;
	int iFootprint = 1;
	bool Reset = true;

	while (iMap[12][12] == 0)
	{
		iSeed = rand() % 4;
		if (iVec[iSeed] != 0)
			continue;
		iVec[iSeed] = 1;

		if (iSeed < 2)
			iDistance = rand() % 2 + 3;
		else
			iDistance = rand() % 3 + 1;

		switch (iSeed)
		{
		case 0:
			for (int i = 0; i < iDistance; ++i)
			{
				if (iLocationX < 12)
					iMap[iLocationY][++iLocationX] = ++iFootprint;
				else
					break;
			}
			iVec[iSeed] = 1;
			break;
		case 1:
			for (int i = 0; i < iDistance; ++i)
			{
				if (iLocationY < 12)
					iMap[++iLocationY][iLocationX] = ++iFootprint;
				else
					break;
			}
			iVec[iSeed] = 1;
			break;
		case 2:
			for (int i = 0; i < iDistance; ++i)
			{
				if (iLocationX > 0)
					iMap[iLocationY][--iLocationX] = ++iFootprint;
				else
					break;
			}
			iVec[iSeed] = 1;
			break;
		case 3:
			for (int i = 0; i < iDistance; ++i)
			{
				if (iLocationY > 0)
					iMap[--iLocationY][iLocationX] = ++iFootprint;
				else
					break;
			}
			iVec[iSeed] = 1;
			break;
		default:

			break;
		}

		Reset = true;
		for (int i = 0; i < 4; ++i)
		{
			if (iVec[i] == 0)
			{
				Reset = false;
				break;
			}
		}

		if (Reset)
		{
			for (int i = 0; i < 4; ++i)
			{
				iVec[i] = 0;
			}
		}
	}
}

void ResetMap(int iMap[][13])
{
	for (int i = 0; i < 13; ++i)
	{
		for (int j = 0; j < 13; ++j)
			iMap[i][j] = 0;
	}

	iMap[0][0] = 1;
}

void PrintMap(int iMap[][13])
{
	for (int i = 0; i < 13; ++i)
	{
		for (int j = 0; j < 13; ++j)
			std::cout << std::setw(3) << iMap[i][j];
			std::cout << std::endl << std::endl;
	}
}
int main()
{

	
	int iMap[13][13] = { };
	
	char Command;

	iMap[0][0] = 1;
	MakeRoad(iMap);
	
	while (1)
	{
		
		PrintMap(iMap);

		std::cout << "Command: ";
		std::cin >> Command;
		while (getchar() != '\n');

		if (Command == 'q')
			break;

		else if (Command == 'r')
		{
			system("cls");
			ResetMap(iMap);
			MakeRoad(iMap);
		}

	}

	return 0;
}