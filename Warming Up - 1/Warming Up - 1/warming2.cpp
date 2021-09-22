#include <iostream>
#include <fstream>
#include <string>

struct Word {
	int iNum = 0;
	char cWord[256];
	bool bIsNumber = false;
};

int main()
{
	std::ifstream TextFile;
	std::string FileName;
	char str[10][80];
	Word W_MyWord[50];

	int iLine = 0;
	int iWord = 0;
	int iNumberWord = 0;
	
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

	for (int i = 0; i < iLine; ++i)
	{
		for (int j = 0; j < 80; ++j)
		{
			if (str[i][j] == '\0')
			{
				++iWord;
				break;
			}
			else
			{
				if (str[i][j] == ' ')
				{
					++iWord;
				}

				else
				{
					if ((int)str[i][j] > 47 && (int)str[i][j] < 58)
						W_MyWord[iWord].bIsNumber = true;

					W_MyWord[iWord].cWord[W_MyWord[iWord].iNum] = str[i][j];
					++W_MyWord[iWord].iNum;
				}
				std::cout << str[i][j];
			}
		}
		std::cout << std::endl;
	}

	
	std::cout << "\n\nNumberWord: ";
	for (int i = 0; i < iWord; ++i)
	{
		if (W_MyWord[i].bIsNumber)
		{
			++iNumberWord;
			for (int j = 0; j < W_MyWord[i].iNum; ++j)
			{
				std::cout << W_MyWord[i].cWord[j];
			}
			std::cout << " ";
		}
	}
	std::cout << "\nNumber Word Count: " << iNumberWord << std::endl;
	std::cout << "Only Word Count: " << iWord - iNumberWord << std::endl;
	std::cout << "Total Word Count: " << iWord << std::endl;

	TextFile.close();
	return 0;
}