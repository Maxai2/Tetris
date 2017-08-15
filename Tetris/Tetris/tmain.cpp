//---------------------------------------------------------------------------
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef __BORLANDC__
#include <time.h>
#endif
#pragma hdrstop

#include "tmain.h"
#include "Figures.h"
#include "window.h"

bool Game();

extern COLORS FigColors[FIGURE_COUNT];

int Stat[FIGURE_COUNT];

FIGURE CurFigure;

int NextFigN;
int CurFigN;
int X, Y;

typedef struct
{
	DWORD Timeout;
	int LineBonus;
	int MaxCount;
}
LEVEL;

LEVEL Levels[MAX_LEVEL] =
{
	//easy						//0
	{ 700, 10,    200 },   	//200
	{ 600, 20,    600 },	//400
	{ 500, 30,   1200 },	//600
							//medium
	{ 400, 40,   2000 },	//800
	{ 300, 50,   3000 },	//1000
	{ 200, 60,   4200 },	//1200
							//difficult
	{ 160, 70,   5600 },	//1400
	{ 115, 80,   7200 },	//1600
	{ 70, 90,   9000 }		//1800
};

int CurLevel;
int Count;
//---------------------------------------------------------------------------
#ifndef __BORLANDC__ //if not defined
void randomize()
{
	srand((unsigned int)time(0));
}
//---------------------------------------------------------------------------
int random(int max)
{
	return rand() % max;
}
//---------------------------------------------------------------------------
int cprintf(const char *format, ...)
{
	va_list param;
	va_start(param, format);

	char *Buffer;
	int l = _vsnprintf(NULL, 0, format, param);
	if (l > 0)
	{
		Buffer = (char *)malloc(l + 1);
		_vsnprintf(Buffer, l + 1, format, param);
		cputs(Buffer);
		free(Buffer);
	}

	va_end(param);

	return l;
}
#endif
//---------------------------------------------------------------------------
bool WaitYesNo(int Yes, int No)
{
	while (1)
	{
		int c = getch();
		if (c == 0)
			c = getch();

		if (c == No)
			return false;
		else
			if (c == Yes)
				return true;
	}
}
//---------------------------------------------------------------------------
bool MessageWindow(int x, int y, char *Msg1, char *Msg2, int Yes, int No)
{
	int L = strlen(Msg1) > strlen(Msg2) ? strlen(Msg1) : strlen(Msg2);

	WINDOW *win = CreateConWindow(L, 2, BLACK, LIGHTGRAY, btDouble);

	ShowConWindow(win, x, y);

	SetConWindowXY(win, 0, 0);
	cputs(Msg1);
	SetConWindowXY(win, 0, 1);
	cputs(Msg2);

	bool ret = WaitYesNo(Yes, No);

	DeleteConWindow(win);

	return ret;
}
//---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
#ifndef __BORLANDC__
	OpenBorConio();
#endif

	textmode(C80X50);

	_setcursortype(_NOCURSOR);

	randomize();

	while (1)
	{
		bool ret = Game();	//true - game over, false - press ESC

							//	hall of fame
							//	game over
		if (ret)
		{
			if (!MessageWindow(20, 20, "Game over", "Press ENTER to continue or ESC to exit", '\r', 0x1B))
				break;
		}
		else
		{
			if (!MessageWindow(20, 20, "Game interrupted by user", "Press ENTER to continue or ESC to exit", '\r', 0x1B))
				break;
		}
	}

	textmode(_ORIGMODE);
	_setcursortype(_NORMALCURSOR);
	textcolor(LIGHTGRAY);
	clrscr();

#ifndef __BORLANDC__
	CloseBorConio();
#endif

	return 0;
}
//---------------------------------------------------------------------------
void ShowGlassBounds(int y, int x)
{
	textcolor(YELLOW);

	for (int i = 0; i<GLASS_HEIGHT*ELEM_HEIGHT; ++i, ++y)
	{
		gotoxy(x - 1, y);

		putch(0xB3);
		gotoxy(x + GLASS_WIDTH*ELEM_WIDTH, y);
		putch(0xB3);
	}

	gotoxy(x - 1, y);

	putch(0xC0);
	for (int j = 0; j<GLASS_WIDTH*ELEM_WIDTH; ++j)
		putch(0xC4);
	putch(0xD9);
}
//---------------------------------------------------------------------------
void ShowStat(int Fig)
{
	textcolor(YELLOW);
	gotoxy(STAT_X + FIGURE_SIZE + 2, STAT_Y + Fig*(FIGURE_SIZE + 2));
	cprintf("- %-3d", Stat[Fig]);
}
//---------------------------------------------------------------------------
void ShowStats()
{
	for (int i = 0; i < FIGURE_COUNT; i++)
	{
		ClearAndDrawFigure(STAT_Y + i*(FIGURE_SIZE + 2), STAT_X, i, 1, 1, YELLOW);

		ShowStat(i);
	}
}
//---------------------------------------------------------------------------
void ShowLevel()
{
	textcolor(YELLOW);
	gotoxy(LEVEL_X, LEVEL_Y);
	cprintf("Level %-2d  Count %-5d", CurLevel + 1, Count);
}
//---------------------------------------------------------------------------
void Init(int Difficult)
{
	ClearGlass();

	for (int i = 0; i < FIGURE_COUNT; i++)
		Stat[i] = 0;

	textcolor(YELLOW);
	gotoxy(STAT_X, STAT_Y - 2);
	cputs("Statistics:");

	ShowStats();

	textcolor(YELLOW);
	gotoxy(NEXT_X, NEXT_Y - 2);
	cputs("Next Figure");

	for (int i = 0; i<GLASS_HEIGHT; ++i)
		SetEmptyGlassLine(i);

	if (Difficult == 0)
	{
		CurLevel = 0;
		Count = 0;
	}
	else
	if (Difficult == 1)
	{
		CurLevel = 3;
		Count = Levels[CurLevel - 1].MaxCount;
	}
	else
	if (Difficult == 2)
	{
		CurLevel = 6;
		Count = Levels[CurLevel - 1].MaxCount;
	}

	ShowLevel();
}
//---------------------------------------------------------------------------
bool MoveFigure(FIGURE &Fig, int NewY, int NewX, bool bNoCheck) 
{
	if (bNoCheck || CanPlaceFigure(Fig, NewY, NewX))
	{
		//------------------------- clear old figure
		ClearFigureFromScreen();

		//------------------------- draw new figure
		memcpy(&CurFigure, &Fig, sizeof(FIGURE));
		X = NewX;
		Y = NewY;

		DrawFigureOnScreen();
		return true;
	}
	else
		return false;
}
//---------------------------------------------------------------------------
DWORD dwLastDownMoveTime;

bool MoveOneFigure()
{
	while (1)
	{
		if (kbhit())
		{
			int c = getch();
			if (c == 0)
				c = getch();

			switch (c)
			{
			case '\r':			// pause
				MessageWindow(20, 20, "Game paused", "Press ENTER to continue", '\r', '\r');
				dwLastDownMoveTime = GetTickCount();
				break;
			case 0x48:			//Rotate
			case '8':
			{
				FIGURE Temp;
				Rotate(Temp);

				//центровка
				int x = X + (CurFigure.Width - Temp.Width) / 2;

				//отскакивание от стенок
				if (x < 0)
					x = 0;
				else
					if (x + Temp.Width > GLASS_WIDTH)
						x = GLASS_WIDTH - Temp.Width;

				if (MoveFigure(Temp, Y, x, false))
					SaveRotatedFig();
			}
			break;
			case 0x4B:			//Left
			case '4':
				MoveFigure(CurFigure, Y, X - 1, false);
				break;
			case 0x4D:			//Right
			case '6':
				MoveFigure(CurFigure, Y, X + 1, false);
				break;
			case 0x50:			//Down
			case '5':
			{
#define DOWN_STEP 2

				int y = Y;
				int i;
				for (i = 0; i < DOWN_STEP; i++)
					if (CanPlaceFigure(CurFigure, y + 1, X))
						y++;

				if (y != Y)
				{
					MoveFigure(CurFigure, y, X, true);
					dwLastDownMoveTime = GetTickCount();
				}

				if (i != DOWN_STEP)
					return true;		// next figure
			}
			continue;
			//				break;
			case ' ':   		//Drop
			{
				int y = Y;
				while (CanPlaceFigure(CurFigure, y + 1, X))
					y++;

				if (y != Y)
					MoveFigure(CurFigure, y, X, true);

				dwLastDownMoveTime = GetTickCount();
			}
			return true;	// next figure
			case 0x1B:
				return false;	// exit from game
			}
		}

		DWORD CurTime = GetTickCount();
		if (CurTime - dwLastDownMoveTime > Levels[CurLevel].Timeout)
		{
			dwLastDownMoveTime = CurTime;

			if (!MoveFigure(CurFigure, Y + 1, X, false))
				return true;		// next figure
		}
	}
}
//---------------------------------------------------------------------------
int CountCombo(int DeletedRows)
{
	if (DeletedRows == 1)
		return Levels[CurLevel].LineBonus;

	int sum = (1 + DeletedRows) * DeletedRows / 2;
	return Levels[CurLevel].LineBonus * sum;
}
//---------------------------------------------------------------------------
bool Game()
{
	Init(0); //0, 1, 2

	ShowGlassBounds(S_Y, S_X);
	ShowGlass(S_Y, S_X);

	NextFigN = random(FIGURE_COUNT);

	dwLastDownMoveTime = GetTickCount();

	while (1)
	{
		CurFigN = NextFigN;
		SetCurFigure();

		Stat[CurFigN]++;
		ShowStat(CurFigN);

		Y = 1 - CurFigure.Height;
		X = GLASS_WIDTH / 2 - CurFigure.Width / 2;
		if (!CanPlaceFigure(CurFigure, Y, X))
			return true; 		//game over

		DrawFigureOnScreen();

		NextFigN = random(FIGURE_COUNT);
		ClearAndDrawFigure(NEXT_Y, NEXT_X, NextFigN, ELEM_HEIGHT, ELEM_WIDTH, FigColors[NextFigN]);

		if (!MoveOneFigure())
			return false; //press ESC

		if (Y<0)
			return true; 		//game over

		SetFigureInGlass();

		int DeletedRows = CheckGlass();
		if (DeletedRows)
		{
			ShowGlass(S_Y, S_X);

			Count += CountCombo(DeletedRows);

			if (Count >= Levels[CurLevel].MaxCount)
				if (CurLevel + 1 < MAX_LEVEL)
					CurLevel++;

			ShowLevel();
		}
	}
}
//---------------------------------------------------------------------------

