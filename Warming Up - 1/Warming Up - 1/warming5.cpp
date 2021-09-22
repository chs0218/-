#include <iostream>
#include <math.h>
#include <crtdbg.h>

#define LIST_MAX 10

struct Point {
	int x = 0;
	int y = 0;
	int z = 0;
	double iLength = 0;
	bool isEmpty = true;
	Point* next;
	Point* last;

	void print() {
		if (isEmpty)
		{
			std::cout << "||                 \t\t\t||\n";
		}
		else
			std::cout << "|| " << x << " " << y << " " << z << " \t Length: " << iLength << "\t||\n";
	}
};

void addList(Point* head)
{
	Point* p = new Point;
	p->next = head->next;
	p->last = head;
	head->next->last = p;
	head->next = p;
	
}

void CommandPlus(Point* head, Point* last, int x, int y, int z)
{
	Point* p;
	for (p = last; p->last != head; p = p->last)
	{
		if (p->last->isEmpty)
		{
			p->last->x = x;
			p->last->y = y;
			p->last->z = z;
			p->last->iLength = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
			p->last->isEmpty = false;
			break;
		}
	}
}

void CommandMinus(Point* head, Point* last)
{
	Point* p;
	for (p = head; p->next != last; p = p->next)
	{
		if (!p->next->isEmpty)
		{
			p->next->isEmpty = true;
			break;
		}
	}
}


void CommandE(Point* head, Point* last, int x, int y, int z)
{
	Point* p;

	if (last->last->isEmpty)
	{
		last->last->x = x;
		last->last->y = y;
		last->last->z = z;
		last->last->iLength = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		last->last->isEmpty = false;
	}
	
	else
	{
		for (p = head; p->next != last->last; p = p->next)
		{
			if (!p->next->next->isEmpty)
			{
				p->next->x = p->next->next->x;
				p->next->y = p->next->next->y;
				p->next->z = p->next->next->z;
				p->next->iLength = p->next->next->iLength;
				p->next->isEmpty = false;
			}
		}

		last->last->x = x;
		last->last->y = y;
		last->last->z = z;
		last->last->iLength = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

}

void CommandD(Point* head, Point* last)
{
	Point* p;
	for (p = last; p->last != head; p = p->last)
	{
		if (!p->last->isEmpty)
		{
			p->last->isEmpty = true;
			break;
		}
	}

}

void CommandL(Point* head, Point* last)
{
	Point* p;
	int iNum = 0;
	for (p = head; p->next != last; p = p->next)
	{
		if (!p->next->isEmpty)
		{
			++iNum;
		}
	}
	std::cout << "점의 개수는 " << iNum << "개 입니다.\n";
}
void CommandPrintAll(Point* head, Point* last, Point* arrangehead, Point* arrangelast, bool isCommandS)
{
	Point* p;
	
	if (isCommandS)
	{
		for (p = arrangehead; p->next != arrangelast; p = p->next)
		{
			p->next->print();
		}
	}
	else
	{
		for (p = head; p->next != last; p = p->next)
		{
			p->next->print();
		}
	}

}

void CommandDelete(Point* target)
{
	Point* Delete = target->next;

	Delete->next->last = target;
	target->next = Delete->next;
	
	delete Delete;
}

void CommandC(Point* head, Point* last)
{
	Point* p;
	for (p = last; p->last != head; p = p->last)
		p->last->isEmpty = true;
}

void CommandM(Point* head, Point* last)
{
	Point* p;
	double iMax = INT_MIN;
	int x, y, z;
	for (p = head; p->next != last; p = p->next)
	{
		if (!p->next->isEmpty)
		{
			if (p->next->iLength > iMax)
			{
				iMax = p->next->iLength;
				x = p->next->x;
				y = p->next->y;
				z = p->next->z;
			}
		}
	}

	if (iMax != INT_MIN)
		std::cout << "원점에서 가장 먼 거리에 있는 점의 좌표는 (" << x << ", " << y << ", " << z << ")입니다.\n";
}

void CommandN(Point* head, Point* last)
{
	Point* p;
	double iMin = INT_MAX;
	int x, y, z;
	for (p = head; p->next != last; p = p->next)
	{
		if (!p->next->isEmpty)
		{
			if (p->next->iLength < iMin)
			{
				iMin = p->next->iLength;
				x = p->next->x;
				y = p->next->y;
				z = p->next->z;
			}
		}
	}

	if (iMin != INT_MAX)
		std::cout << "원점에서 가장 가까운 거리에 있는 점의 좌표는 (" << x << ", " << y << ", " << z << ")입니다.\n";
}

void CommandS(Point* head, Point* last, Point* arrangehead, Point* arrangelast, bool& isCommandS)
{
	Point* p;
	Point* q;
	double iMax = INT_MIN;
	double oldMax = INT_MAX;
	bool isNotChanged = true;
	int tmpx, tmpy, tmpz, tmplength;

	if (isCommandS)
		isCommandS = false;
	else
	{
		CommandC(arrangehead, arrangelast);
		for (int i = 0; i < 10; ++i)
		{
			isNotChanged = true;
			for (p = head; p->next != last; p = p->next)
			{
				if (iMax < p->next->iLength && oldMax > p->next->iLength && !p->next->isEmpty)
				{
					iMax = p->next->iLength;
					tmpx = p->next->x;
					tmpy = p->next->y;
					tmpz = p->next->z;
					tmplength = p->next->iLength;
					isNotChanged = false;
				}
			}

			if (isNotChanged)
				break;
			else
			{
				CommandPlus(arrangehead, arrangelast, tmpx, tmpy, tmpz);
				oldMax = iMax;
				iMax = INT_MIN;
			}
		}
		isCommandS = true;
	}
}

int main()
{
	Point* head = new Point;
	Point* last = new Point;
	Point* arrangehead = new Point;
	Point* arrangelast = new Point;

	arrangehead->next = arrangelast;
	arrangehead->last = arrangelast;
	arrangelast->next = arrangehead;
	arrangelast->last = arrangehead;

	head->next = last;
	head->last = last;
	last->next = head;
	last->last = head;

	char Command;
	bool isCommandS = false;
	int x = 0, y = 0, z = 0;

	for (int i = 0; i < LIST_MAX; ++i)
	{
		addList(head);
		addList(arrangehead);
	}

	while (1)
	{
		std::cout << "Command: ";
		std::cin >> Command;

		if (Command == 'q')
			break;
		
		else if (Command == '+' || Command == 'e')
		{
			while (getchar() != ' ');
			std::cin >> x >> y >> z;
			if (!std::cin)
			{
				std::cout << "숫자가 아닙니다.\n";
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				continue;
			}
		}

		while (getchar() != '\n');


		switch (Command)
		{
		case '+':
			if (isCommandS)
				std::cout << "S 명령을 종료해주세요\n";
			else
			{
				CommandPlus(head, last, x, y, z);
				CommandPrintAll(head, last, arrangehead, arrangelast, isCommandS);
			}
			break;
		case '-':
			if (isCommandS)
				std::cout << "S 명령을 종료해주세요\n";
			else
			{
				CommandMinus(head, last);
				CommandPrintAll(head, last, arrangehead, arrangelast, isCommandS);
			}
			break;
		case 'e':
			if (isCommandS)
				std::cout << "S 명령을 종료해주세요\n";
			else
			{
				CommandE(head, last, x, y, z);
				CommandPrintAll(head, last, arrangehead, arrangelast, isCommandS);
			}
			break;
		case 'd':
			if (isCommandS)
				std::cout << "S 명령을 종료해주세요\n";
			else
			{
				CommandD(head, last);
				CommandPrintAll(head, last, arrangehead, arrangelast, isCommandS);
			}
			break;
		case 'l':
			CommandPrintAll(head, last, arrangehead, arrangelast, isCommandS);
			CommandL(head, last);
			break;
		case 'c':
			if (isCommandS)
				std::cout << "S 명령을 종료해주세요\n";
			else
			{
				CommandC(head, last);
				CommandPrintAll(head, last, arrangehead, arrangelast, isCommandS);
			}
			break;
		case 'm':
			CommandM(head, last);
			break;
		case 'n':
			CommandN(head, last);
			break;
		case 's':
			CommandS(head, last, arrangehead, arrangelast, isCommandS);
			CommandPrintAll(head, last, arrangehead, arrangelast, isCommandS);

			break;
		default:
			std::cout << "명령을 잘못 입력하셨습니다.\n";
			break;
		}
	}

	while (head->next != last)
		CommandDelete(head);

	while (arrangehead->next != arrangelast)
		CommandDelete(arrangehead);
	

	delete head;
	delete last;
	delete arrangehead;
	delete arrangelast;
	_CrtDumpMemoryLeaks();
	return 0;
}