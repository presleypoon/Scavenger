#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>

void Move(int *PlayerX, int *PlayerY);
void PickUpLogic(int PlayerX, int PlayerY, int *PickUpX, int *PickUpY, int64_t *Score);
void EnemyLogic(int PlayerX, int PlayerY, int *EnemyX, int *EnemyY, int64_t *Score, int64_t Tick);
int sign(int num);
int RandomInt(int max);
void Render(char Screen[], int PlayerX, int PlayerY, int PickUpX, int PickUpY, int EnemyX, int EnemyY, int64_t Score);
void PrintColouredPx(char colour, int length);

int main()
{
#ifdef _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return 1;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return 1;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		return 1;
	}
#endif

	uint64_t Time = time(NULL);
	srand(Time);

	int PlayerX = 0;
	int PlayerY = 0;
	char Screen[101];
	int PickUpX = 0;
	int PickUpY = 0;
	int EnemyX = 10;
	int EnemyY = 10;

	int64_t Score = -10;
	const int fps = 30;
	int64_t Tick = 0;

	system("cls");
	while (1)
	{
		Move(&PlayerX, &PlayerY);
		PickUpLogic(PlayerX, PlayerY, &PickUpX, &PickUpY, &Score);
		EnemyLogic(PlayerX, PlayerY, &EnemyX, &EnemyY, &Score, Tick);
		Render(Screen, PlayerX, PlayerY, PickUpX, PickUpY, EnemyX, EnemyY, Score);
		rand();
		Tick++;
		printf("\033[?25h");
		Sleep(1000 / fps);
	}
}

void Move(int *PlayerX, int *PlayerY)
{
	if (!_kbhit())
	{
		return;
	}
	char c = _getch();
	switch (c)
	{
	case 'w':
	{
		(*PlayerY)--;
		if ((*PlayerY) < 0)
		{
			(*PlayerY) = 9;
		}
		break;
	}
	case 'a':
	{
		(*PlayerX)--;
		if ((*PlayerX) < 0)
		{
			(*PlayerX) = 9;
		}
		break;
	}
	case 's':
	{
		(*PlayerY)++;
		if ((*PlayerY) > 9)
		{
			(*PlayerY) = 0;
		}
		break;
	}
	case 'd':
	{
		(*PlayerX)++;
		if ((*PlayerX) > 9)
		{
			(*PlayerX) = 0;
		}
		break;
	}
	}
	printf("%d, %d\n", *PlayerX, *PlayerY);
}

void PickUpLogic(int PlayerX, int PlayerY, int *PickUpX, int *PickUpY, int64_t *Score)
{
	if (PlayerX == *PickUpX && PlayerY == *PickUpY)
	{
		*PickUpX = RandomInt(10);
		*PickUpY = RandomInt(10);
		*Score += 10;
	}
}

void EnemyLogic(int PlayerX, int PlayerY, int *EnemyX, int *EnemyY, int64_t *Score, int64_t Tick)
{
	if (Tick % 10)
	{
		return;
	}
	int XDist = *EnemyX - PlayerX;
	int YDist = *EnemyY - PlayerY;
	if (abs(XDist) > abs(YDist))
	{
		*EnemyX -= sign(XDist);
		*EnemyX = (*EnemyX + 10) % 10;
	}
	else
	{
		*EnemyY -= sign(YDist);
		*EnemyY = (*EnemyY + 10) % 10;
	}
	if (!((*EnemyX == PlayerX) && (*EnemyY == PlayerY)))
	{
		return;
	}
	*Score -= 10;
}

int sign(int num)
{
	if (num > 0)
	{
		return 1;
	}
	if (num < 0)
	{
		return -1;
	}
	return 0;
}

int RandomInt(int max)
{
	for (int i = 0; i < rand() % 128; i++)
	{
		rand();
	}
	return rand() % max;
}

void Render(char Screen[], int PlayerX, int PlayerY, int PickUpX, int PickUpY, int EnemyX, int EnemyY, int64_t Score)
{
	char Buffer[] = "++++++++++++\n+          +\n+          +\n+          +\n+          +\n+          +\n+          +\n+          +\n+          +\n+          +\n+          +\n++++++++++++";

	Buffer[(PlayerX + 1) + 13 * (PlayerY + 1)] = '@';
	Buffer[(EnemyX + 1) + 13 * (EnemyY + 1)] = '!';
	Buffer[(PickUpX + 1) + 13 * (PickUpY + 1)] = '$';

	printf("\033[?25l\033[H%ld                    \n", Score);

	int i = 0;
	while (true)
	{
		switch (Buffer[i])
		{
		case ' ':
		{
			printf("  ");
			break;
		}
		case ('+'):
		{
			PrintColouredPx('w', 2);
			break;
		}
		case ('@'):
		{
			PrintColouredPx('g', 2);
			break;
		}
		case ('$'):
		{
			PrintColouredPx('y', 2);
			break;
		}
		case ('!'):
		{
			PrintColouredPx('r', 2);
			break;
		}
		case ('\n'):
		{
			printf("\n");
			break;
		}
		case ('\0'):
		{
			printf("\033[?25h");
			return;
		}
		}
		i++;
	}
}

/**
 * @param colour: b for black, r for red, g for green, y for yellow, l for blue, m for magenta, c for cyan and w for white, other will be default
 */
void PrintColouredPx(char colour, int length)
{
	switch (colour)
	{
	case 'b':
	{
		printf("\033[40m");
		break;
	}
	case 'r':
	{
		printf("\033[41m");
		break;
	}
	case 'g':
	{
		printf("\033[42m");
		break;
	}
	case 'y':
	{
		printf("\033[43m");
		break;
	}
	case 'l':
	{
		printf("\033[44m");
		break;
	}
	case 'm':
	{
		printf("\033[45m");
		break;
	}
	case 'c':
	{
		printf("\033[46m");
		break;
	}
	case 'w':
	{
		printf("\033[47m");
		break;
	}
	}

	for (int i = 0; i < length; i++)
	{
		printf(" ");
	}

	printf("\033[49m");
}
