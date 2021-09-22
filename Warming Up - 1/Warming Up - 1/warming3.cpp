#include <iostream>
#include <fstream>
#include <string>

void CommandD(char str[][80], const int iLine)
{
	char Tmp[80] = {};
	int iSize;
	for (int i = 0; i < iLine; ++i)
	{
		iSize = strlen(str[i]);
		strncpy_s(Tmp, str[i], iSize);
		for (int j = 0; j < iSize; ++j)
		{
			str[i][j] = Tmp[iSize - j - 1];
		}
	}

	for (int i = 0; i < iLine; ++i)
	{
		for (int j = 0; j < 80; ++j)
		{
			if (str[i][j] == '\0')
				break;
	
			else
				std::cout << str[i][j];
		
		}
		std::cout << std::endl;
	}
}

void CommandE(char str[][80], const int iLine, bool& IsCommandE)
{
	int iNum = 0;

	if (IsCommandE)
	{
		for (int i = 0; i < iLine; ++i)
		{
			for (int j = 0; j < strlen(str[i]); ++j)
			{
				if (str[i][j] == '*')
				{
					for (int k = j; k < strlen(str[i]); ++k)
					{
						str[i][k] = str[i][k + 2];
					}

					
				}
			}
		}

		IsCommandE = false;
	}

	else
	{
		for (int i = 0; i < iLine; ++i)
		{
			iNum = 0;

			for (int j = 0; j < strlen(str[i]); ++j)
			{
				if (iNum == 3)
				{
					for (int k = strlen(str[i]) + 2; k > j; --k)
					{
						str[i][k] = str[i][k - 2];
					}
					str[i][j] = '*';
					str[i][++j] = '*';
					iNum = 0;
				}

				else
					++iNum;
			}

		}

		IsCommandE = true;

		
	}

	for (int i = 0; i < iLine; ++i)
	{
		for (int j = 0; j < 80; ++j)
		{
			if (str[i][j] == '\0')
				break;

			else
				std::cout << str[i][j];

		}
		std::cout << std::endl;
	}

}

void CommandF(char str[][80], const int iLine)
{
	char Tmp[80] = {};

	int iStart, iNum;

	for (int i = 0; i < iLine; ++i)
	{
		iStart = 0;
		iNum = 0;
		for (int j = 0; j < 80; ++j)
		{
			if (str[i][j] == '*' || str[i][j] == ' ' || str[i][j] == '\0')
			{
				for (int k = 0; k < iNum; ++k)
				{
					str[i][iStart + k] = Tmp[iNum - k - 1];
				}
				iStart = j + 1;
				iNum = 0;
			}

			else
			{
				Tmp[iNum] = str[i][j];
				++iNum;
			}
		}
	}

	for (int i = 0; i < iLine; ++i)
	{
		for (int j = 0; j < 80; ++j)
		{
			if (str[i][j] == '\0')
				break;

			else
				std::cout << str[i][j];

		}
		std::cout << std::endl;
	}
}

void CommandG(char str[][80], const int iLine)
{
	char OldLetter;
	char NewLetter;

	while (1)
	{
		std::cout << "바꿀 문자를 입력해주세요: ";
		std::cin >> OldLetter;

		while (getchar() != '\n');

		if (OldLetter < 'A' || OldLetter > 'z')
			continue;

		break;
	}

	while (1)
	{
		std::cout << "새롭게 넣을 문자를 입력해주세요: ";
		std::cin >> NewLetter;
		while (getchar() != '\n');

		if (NewLetter < 'A' || NewLetter > 'z')
			continue;

		break;
	}

	for (int i = 0; i < iLine; ++i)
	{
		for (int j = 0; j < strlen(str[i]); ++j)
		{
			if (str[i][j] == OldLetter)
				str[i][j] = NewLetter;
		}
	}

	for (int i = 0; i < iLine; ++i)
	{
		for (int j = 0; j < 80; ++j)
		{
			if (str[i][j] == '\0')
				break;

			else
				std::cout << str[i][j];

		}
		std::cout << std::endl;
	}
}

void CommandH(const char str[][80], const int iLine)
{
	char Tmp[80] = {};
	int iNum = 0;
	for (int i = 0; i < iLine; ++i)
	{
		for (int j = 0; j < 80; ++j)
		{
			if (str[i][j] == '\0')
			{
				iNum = 0;
				for (int k = 0; k < 80; ++k)
				{
					if (str[i][k] == str[i][strlen(str[i])- 1 - k])
					{
						Tmp[iNum] = str[i][k];
						++iNum;
					}

					else 
					{
						std::cout << " : ";

						if (iNum == 0)
							std::cout << "0";

						else
						{
							for (int l = 0; l < iNum; ++l)
							{
								std::cout << Tmp[l];
							}
						}

						break;
					}
					
				}
				break;
			}

			else
				std::cout << str[i][j];

		}
		std::cout << std::endl;
	}
}

int main()
{
	std::ifstream TextFile;
	std::string FileName;
	char str[10][80] = {};
	char cCommand;
	int iLine = 0;
	bool IsCommandE = false;
	while (1)
	{
		std::cout << "Input Data File Name: ";
		std::getline(std::cin, FileName);
		TextFile.open(FileName);

		if (TextFile.is_open())
		{
			while (!TextFile.eof())
			{
				TextFile.getline(str[iLine], 80);
				++iLine;
			}
			break;
		}

		else
			continue;
	}

	while (1)
	{
		std::cout << "명령을 입력해주세요: ";
		std::cin >> cCommand;

		while (getchar() != '\n');

		if (cCommand < 'a' || cCommand > 'z')
			continue;

		else if(cCommand == 'q')
			break;


		switch (cCommand)
		{
		case 'd':
			CommandD(str, iLine);
			break;
		case 'e':
			CommandE(str, iLine, IsCommandE);
			break;
		case 'g':
			CommandG(str, iLine);
			break;
		case 'f':
			CommandF(str, iLine);
			break;
		case 'h':
			CommandH(str, iLine);
			break;
		default:
			std::cout << "잘못 입력하셨습니다.\n";
		}
		std::cout << std::endl;
	}

	

	TextFile.close();
	return 0;
}