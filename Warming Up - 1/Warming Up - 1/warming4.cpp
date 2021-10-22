#include <iostream>
#include <random>
#include <time.h>
#include <math.h>

#define WIDTH_MAX 800
#define HEIGHT_MAX 600

struct Point {
	int x = 0;
	int y = 0;
};

int CCW(const Point a, const Point b, const Point c)
{
	int iAns = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	if (iAns < 0) return 1;
	else if (iAns > 0) return -1;
	else return 0;
}

bool ParallelCollide(int p1, int p2, int p3, int p4)
{
	int tmp;

	if (p1 > p2)
	{
		tmp = p1;
		p1 = p2;
		p2 = tmp;
	}

	if (p3 > p4)
	{
		tmp = p3;
		p3 = p4;
		p4 = tmp;
	}

	if (p2 >= p3 && p1 <= p4)
		return true;
	else
		return false;
}

bool isCollide(const Point P_Rect[], const Point P_Line[])
{
	Point Tmp_Rect[4];
	
	Tmp_Rect[0].x = P_Rect[0].x;
	Tmp_Rect[0].y = P_Rect[0].y;

	Tmp_Rect[1].x = P_Rect[0].x;
	Tmp_Rect[1].y = P_Rect[1].y;

	Tmp_Rect[2].x = P_Rect[1].x;
	Tmp_Rect[2].y = P_Rect[0].y;

	Tmp_Rect[3].x = P_Rect[1].x;
	Tmp_Rect[3].y = P_Rect[1].y;


	// CCW(A,B,C)*CCW(A,B,D) <=0 && CCW(C,D,A)*CCW(C,D,B) <=0

	if (CCW(Tmp_Rect[0], Tmp_Rect[1], P_Line[0]) * CCW(Tmp_Rect[0], Tmp_Rect[1], P_Line[1]) <= 0 &&
		CCW(P_Line[0], P_Line[1], Tmp_Rect[0]) * CCW(P_Line[0], P_Line[1], Tmp_Rect[1]) <= 0)
	{
		if (CCW(Tmp_Rect[0], Tmp_Rect[1], P_Line[0]) * CCW(Tmp_Rect[0], Tmp_Rect[1], P_Line[1]) == 0 &&
			CCW(P_Line[0], P_Line[1], Tmp_Rect[0]) * CCW(P_Line[0], P_Line[1], Tmp_Rect[1]) == 0)
		{
			if(ParallelCollide(Tmp_Rect[0].y, Tmp_Rect[1].y, P_Line[0].y, P_Line[1].y))
				return true;
			else
				return false;
		}
		else
			return true;
	}

	if (CCW(Tmp_Rect[0], Tmp_Rect[2], P_Line[0]) * CCW(Tmp_Rect[0], Tmp_Rect[2], P_Line[1]) <= 0 &&
		CCW(P_Line[0], P_Line[1], Tmp_Rect[0]) * CCW(P_Line[0], P_Line[1], Tmp_Rect[2]) <= 0)
	{
		if (CCW(Tmp_Rect[0], Tmp_Rect[2], P_Line[0]) * CCW(Tmp_Rect[0], Tmp_Rect[2], P_Line[1]) == 0 &&
			CCW(P_Line[0], P_Line[1], Tmp_Rect[0]) * CCW(P_Line[0], P_Line[1], Tmp_Rect[2]) == 0)
		{
			if (ParallelCollide(Tmp_Rect[0].x, Tmp_Rect[2].x, P_Line[0].x, P_Line[1].x))
				return true;
			else
				return false;
		}
		else
			return true;
	}

	if (CCW(Tmp_Rect[1], Tmp_Rect[3], P_Line[0]) * CCW(Tmp_Rect[1], Tmp_Rect[3], P_Line[1]) <= 0 &&
		CCW(P_Line[0], P_Line[1], Tmp_Rect[1]) * CCW(P_Line[0], P_Line[1], Tmp_Rect[3]) <= 0)
	{
		if (CCW(Tmp_Rect[1], Tmp_Rect[3], P_Line[0]) * CCW(Tmp_Rect[1], Tmp_Rect[3], P_Line[1]) == 0 &&
			CCW(P_Line[0], P_Line[1], Tmp_Rect[1]) * CCW(P_Line[0], P_Line[1], Tmp_Rect[3]) == 0)
		{
			if (ParallelCollide(Tmp_Rect[1].x, Tmp_Rect[3].x, P_Line[0].x, P_Line[1].x))
				return true;
			else
				return false;
		}
		else
			return true;
	}

	if (CCW(Tmp_Rect[2], Tmp_Rect[3], P_Line[0]) * CCW(Tmp_Rect[2], Tmp_Rect[3], P_Line[1]) <= 0 &&
		CCW(P_Line[0], P_Line[1], Tmp_Rect[2]) * CCW(P_Line[0], P_Line[1], Tmp_Rect[3]) <= 0)
	{
		if (CCW(Tmp_Rect[2], Tmp_Rect[3], P_Line[0]) * CCW(Tmp_Rect[2], Tmp_Rect[3], P_Line[1]) == 0 &&
			CCW(P_Line[0], P_Line[1], Tmp_Rect[2]) * CCW(P_Line[0], P_Line[1], Tmp_Rect[3]) == 0)
		{
			if (ParallelCollide(Tmp_Rect[1].y, Tmp_Rect[3].y, P_Line[0].y, P_Line[1].y))
				return true;
			else
				return false;
		}
		else
			return true;
	}

	return false;
}

int main()
{
	srand((unsigned int)time(0));
	Point P_Rect[2] = {};
	Point P_Line[2] = {};
	char Command;
	int tmp;

	P_Rect[0].x = rand() % 801;
	P_Rect[0].y = rand() % 601;
	P_Rect[1].x = rand() % 801;
	P_Rect[1].y = rand() % 601;

	P_Rect[0].x = 100;
	P_Rect[0].y = 100;
	P_Rect[1].x = 200;
	P_Rect[1].y = 200;

	P_Line[0].x = rand() % 801;
	P_Line[0].y = rand() % 601;
	P_Line[1].x = rand() % 801;
	P_Line[1].y = rand() % 601;

	P_Line[0].x = 50;
	P_Line[0].y = 50;
	P_Line[1].x = 50;
	P_Line[1].y = 150;

	if (P_Rect[0].x > P_Rect[1].x)
	{
		tmp = P_Rect[0].x;
		P_Rect[0].x = P_Rect[1].x;
		P_Rect[1].x = tmp;

		tmp = P_Rect[0].y;
		P_Rect[0].y = P_Rect[1].y;
		P_Rect[1].y = tmp;
	}

	if (P_Line[0].x > P_Line[1].x)
	{
		tmp = P_Line[0].x;
		P_Line[0].x = P_Line[1].x;
		P_Line[1].x = tmp;

		tmp = P_Rect[0].y;
		P_Line[0].y = P_Line[1].y;
		P_Line[1].y = tmp;
	}
	if(P_Rect[0].x)
	std::cout << "Input Shape Coordinates Value: \n";
	std::cout << "\tRect: (" << P_Rect[0].x << ", " << P_Rect[0].y << ") (" << P_Rect[1].x << ", " << P_Rect[1].y << ")\n";
	std::cout << "\tLine: (" << P_Line[0].x << ", " << P_Line[0].y << ") (" << P_Line[1].x << ", " << P_Line[1].y << ")\n";

	while (1)
	{
		std::cout << "Command: ";
		std::cin >> Command;

		while (getchar() != '\n');

		if (Command < 'a' || Command > 'z')
			continue;

		switch (Command)
		{
		case 'W':
		case 'w':
			if (P_Rect[0].y <= 550 && P_Rect[1].y <= 550)
			{
				std::cout << "// 사각형을 위로 50만큼 옮깁니다.( Y 좌표 값 +50이동)\n";
				P_Rect[0].y += 50;
				P_Rect[1].y += 50;
			}
			else
				std::cout << "옮길 수 없습니다.\n";
			break;
		case 'A':
		case 'a':
			if (P_Rect[0].x >= 50 && P_Rect[1].x >= 50)
			{
				std::cout << "// 사각형을 왼쪽으로 50만큼 옮깁니다.( x 좌표 값 -50이동)\n";
				P_Rect[0].x -= 50;
				P_Rect[1].x -= 50;
			}
			else
				std::cout << "옮길 수 없습니다.\n";
			break;
		case 'S':
		case 's':
			if (P_Rect[0].y >= 50 && P_Rect[1].y >= 50)
			{
				std::cout << "// 사각형을 아래로 50만큼 옮깁니다.( Y 좌표 값 -50이동)\n";
				P_Rect[0].y -= 50;
				P_Rect[1].y -= 50;
			}
			else
				std::cout << "옮길 수 없습니다.\n";
			break;
		case 'D':
		case 'd':
			if (P_Rect[0].x <= 750 && P_Rect[1].x <= 750)
			{
				std::cout << "// 사각형을 오른쪽으로 50만큼 옮깁니다.( x 좌표 값 +50이동)\n";
				P_Rect[0].x += 50;
				P_Rect[1].x += 50;
			}
			else
				std::cout << "옮길 수 없습니다.\n";
			break;
		case 'I':
		case 'i':
			if (P_Line[0].y <= 550 && P_Line[1].y <= 550)
			{
				std::cout << "// 직선을 위로 50만큼 옮깁니다.( Y 좌표 값 +50이동)\n";
				P_Line[0].y += 50;
				P_Line[1].y += 50;
			}
			else
				std::cout << "옮길 수 없습니다.\n";
			break;
		case 'J':
		case 'j':
			if (P_Line[0].x >= 50 && P_Line[1].x >= 50)
			{
				std::cout << "// 직선을 왼쪽으로 50만큼 옮깁니다.( x 좌표 값 -50이동)\n";
				P_Line[0].x -= 50;
				P_Line[1].x -= 50;
			}
			else
				std::cout << "옮길 수 없습니다.\n";
			break;
		case 'K':
		case 'k':
			if (P_Line[0].y >= 50 && P_Line[1].y >= 50)
			{
				std::cout << "// 직선을 아래로 50만큼 옮깁니다.( Y 좌표 값 -50이동)\n";
				P_Line[0].y -= 50;
				P_Line[1].y -= 50;
			}
			else
				std::cout << "옮길 수 없습니다.\n";
			break;
		case 'L':
		case 'l':
			if (P_Line[0].x <= 750 && P_Line[1].x <= 750)
			{
				std::cout << "// 직선을 오른쪽으로 50만큼 옮깁니다.( x 좌표 값 +50이동)\n";
				P_Line[0].x += 50;
				P_Line[1].x += 50;
			}
			else
				std::cout << "옮길 수 없습니다.\n";
			break;
		default:
			std::cout << "잘못 입력하셨습니다.\n";
			break;
		}

		if (isCollide(P_Rect, P_Line))
		{
			std::cout << "\tRect: (" << P_Rect[0].x << ", " << P_Rect[0].y << ") (" << P_Rect[1].x << ", " << P_Rect[1].y << ")\n";
			std::cout << "\tLine: (" << P_Line[0].x << ", " << P_Line[0].y << ") (" << P_Line[1].x << ", " << P_Line[1].y << ")\n";
			std::cout << "충돌하였습니다.\n";
		}
		else
		{
			std::cout << "\tRect: (" << P_Rect[0].x << ", " << P_Rect[0].y << ") (" << P_Rect[1].x << ", " << P_Rect[1].y << ")\n";
			std::cout << "\tLine: (" << P_Line[0].x << ", " << P_Line[0].y << ") (" << P_Line[1].x << ", " << P_Line[1].y << ")\n";
			std::cout << "충돌하지않았습니다.\n";
		}

	}

	return 0;
}