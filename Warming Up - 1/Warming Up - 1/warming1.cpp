#include <iostream>
#include <random>
#include <time.h>

void PrintMatrix(const int iMatrix[][4], const int iSize)
{
	for (int i = 0; i < iSize; ++i)
	{
		for (int j = 0; j < iSize; ++j)
			std::cout << "\t" << iMatrix[i][j];
		std::cout << std::endl;
	}
}

void RandomMatrix(int iFirstMatrix[][4], int iSecondMatrix[][4], int iSize)
{
	srand(time(0));

	for (int i = 0; i < iSize; ++i)
	{
		for (int j = 0; j < iSize; ++j)
		{
			iFirstMatrix[i][j] = rand() % 2;
			iSecondMatrix[i][j] = rand() % 2;
		}
	}

	std::cout << "============1번 행렬============\n";
	PrintMatrix(iFirstMatrix, iSize);


	std::cout << "\n============2번 행렬============\n";
	PrintMatrix(iSecondMatrix, iSize);
}

int iThreeByThreeMatrix(const int iMatrix[][3])
{
	int iResult = 0;

	iResult += iMatrix[0][0] * iMatrix[1][1] * iMatrix[2][2];
	iResult += iMatrix[0][1] * iMatrix[1][2] * iMatrix[2][0];
	iResult += iMatrix[0][2] * iMatrix[1][0] * iMatrix[2][1];
	iResult -= iMatrix[0][2] * iMatrix[1][1] * iMatrix[2][0];
	iResult -= iMatrix[0][0] * iMatrix[1][2] * iMatrix[2][1];
	iResult -= iMatrix[0][1] * iMatrix[1][0] * iMatrix[2][2];

	return iResult;
}

int iFourByFourMatrix(const int iMatrix[][4])
{
	int iResult = 0;
	int iTmpMatrix[3][3] = {  };
	int iTmp = 0;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			iTmpMatrix[i][j] = iMatrix[i + 1][j + 1];
	}
	iResult += iMatrix[0][0] * iThreeByThreeMatrix(iTmpMatrix);

	for (int i = 0; i < 3; ++i)
	{
		iTmpMatrix[i][0] = iMatrix[i + 1][0];
	}
	iResult -= iMatrix[0][1] * iThreeByThreeMatrix(iTmpMatrix);

	for (int i = 0; i < 3; ++i)
	{
		iTmpMatrix[i][1] = iMatrix[i + 1][1];
	}
	iResult += iMatrix[0][2] * iThreeByThreeMatrix(iTmpMatrix);

	for (int i = 0; i < 3; ++i)
	{
		iTmpMatrix[i][2] = iMatrix[i + 1][2];
	}
	iResult -= iMatrix[0][3] * iThreeByThreeMatrix(iTmpMatrix);

	return iResult;
}

void CommandA(const int iFirstMatrix[][4], const int iSecondMatrix[][4], const int iSize)
{
	int iResultMatrix[4][4] = {};
	for (int i = 0; i < iSize; ++i)
	{
		for (int j = 0; j < iSize; ++j)
			iResultMatrix[i][j] = iFirstMatrix[i][j] + iSecondMatrix[i][j];
	}

	std::cout << "\n============결과 행렬============\n";
	PrintMatrix(iResultMatrix, iSize);
}

void CommandD(const int iFirstMatrix[][4], const int iSecondMatrix[][4], const int iSize)
{
	int iResultMatrix[4][4] = {};
	for (int i = 0; i < iSize; ++i)
	{
		for (int j = 0; j < iSize; ++j)
			iResultMatrix[i][j] = iFirstMatrix[i][j] - iSecondMatrix[i][j];
	}

	std::cout << "\n============결과 행렬============\n";
	PrintMatrix(iResultMatrix, iSize);
}

void CommandM(const int iFirstMatrix[][4], const int iSecondMatrix[][4], const int iSize)
{
	int iResultMatrix[4][4] = {};
	int iSum = 0;
	for (int i = 0; i < iSize; ++i)
	{
		for (int j = 0; j < iSize; ++j)
		{
			iSum = 0;
			for (int k = 0; k < iSize; ++k)
				iSum += iFirstMatrix[i][k] * iSecondMatrix[k][j];
			iResultMatrix[i][j] = iSum;
		}
	}

	std::cout << "\n============결과 행렬============\n";
	PrintMatrix(iResultMatrix, iSize);
}

void CommandR(const int iFirstMatrix[][4], const int iSecondMatrix[][4], const int iSize)
{
	std::cout << "============1번 행렬============\n";
	PrintMatrix(iFirstMatrix, iSize);
	if (iSize == 3)
	{
		int iMatrix[3][3] = {};

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
				iMatrix[i][j] = iFirstMatrix[i][j];
		}
		std::cout << "1번 행렬값: " << iThreeByThreeMatrix(iMatrix) << std::endl;
	}

	else
	{
		std::cout << "1번 행렬값: " << iFourByFourMatrix(iFirstMatrix) << std::endl;
	}
	

	std::cout << "============2번 행렬============\n";
	PrintMatrix(iSecondMatrix, iSize);
	if (iSize == 3)
	{
		int iMatrix[3][3];

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
				iMatrix[i][j] = iSecondMatrix[i][j];
		}
		std::cout << "2번 행렬값: " << iThreeByThreeMatrix(iMatrix) << std::endl;
	}

	else
	{
		std::cout << "2번 행렬값: " << iFourByFourMatrix(iSecondMatrix) << std::endl;
	}
}

void CommandT(int iFirstMatrix[][4], int iSecondMatrix[][4], const int iSize)
{
	int iTmpMatrix[4][4] = {};

	for (int i = 0; i < iSize; ++i)
	{
		for (int j = 0; j < iSize; ++j)
			iTmpMatrix[i][j] = iFirstMatrix[j][i];
	}

	for (int i = 0; i < iSize; ++i)
	{
		for (int j = 0; j < iSize; ++j)
			iFirstMatrix[i][j] = iTmpMatrix[i][j];
	}

	for (int i = 0; i < iSize; ++i)
	{
		for (int j = 0; j < iSize; ++j)
			iTmpMatrix[i][j] = iSecondMatrix[j][i];
	}

	for (int i = 0; i < iSize; ++i)
	{
		for (int j = 0; j < iSize; ++j)
			iSecondMatrix[i][j] = iTmpMatrix[i][j];
	}

}

int main()
{
	int iFirstMatrix[4][4] = {}, iSecondMatrix[4][4] = {}, iResultMatrix[4][4];
	int iSize = 3;
	char cCommand;

	RandomMatrix(iFirstMatrix, iSecondMatrix, iSize);

	while (1)
	{
		std::cout << "명령을 입력해주세요: ";
		std::cin >> cCommand;

		while (getchar() != '\n');

		if (cCommand < 'a' || cCommand > 'z')
			continue;

		else if (cCommand == 'q')
			break;

		switch (cCommand)
		{
		case 'a':
			CommandA(iFirstMatrix, iSecondMatrix, iSize);
			break;
		case 'd':
			CommandD(iFirstMatrix, iSecondMatrix, iSize);
			break;
		case 'm':
			CommandM(iFirstMatrix, iSecondMatrix, iSize);
			break;
		case 'r':
			CommandR(iFirstMatrix, iSecondMatrix, iSize);
			break;
		case 'h':
			if (iSize == 3)
			{
				iSize = 4;
				for (int i = 0; i < 3; ++i)
				{
					iFirstMatrix[3][i] = 0;
					iFirstMatrix[i][3] = 0;
					iSecondMatrix[3][i] = 0;
					iSecondMatrix[i][3] = 0;
				}
				iFirstMatrix[3][3] = 1;
				iSecondMatrix[3][3] = 1;
			}
			else
				iSize = 3;

			CommandR(iFirstMatrix, iSecondMatrix, iSize);
			break;
		case 't':
			CommandT(iFirstMatrix, iSecondMatrix, iSize);
			CommandR(iFirstMatrix, iSecondMatrix, iSize);
			break;
		case 's':
			RandomMatrix(iFirstMatrix, iSecondMatrix, iSize);
			break;
		default:
			break;
		}


	}
	
	

	return 0;
}