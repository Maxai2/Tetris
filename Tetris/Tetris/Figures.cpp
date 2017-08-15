//---------------------------------------------------------------------------
#include <memory.h>
#include <stdlib.h>
#pragma hdrstop

#include "Figures.h"
#include "tmain.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)

COLORS FigColors[FIGURE_COUNT] = //{ YELLOW, CYAN, GREEN, RED, BROWN, BLUE, MAGENTA };
				 {LIGHTRED, YELLOW, LIGHTGREEN, LIGHTCYAN, LIGHTBLUE, LIGHTMAGENTA, LIGHTGRAY};

typedef struct
{
	byte Char;
	byte Color;
}
SCREEN_ELEM;

#define FIGURE_SCR_WIDTH FIGURE_SIZE*ELEM_WIDTH
SCREEN_ELEM FigureBack[FIGURE_SIZE*ELEM_HEIGHT*FIGURE_SCR_WIDTH];

byte Glass[GLASS_HEIGHT][GLASS_WIDTH];

#define GLASS_SCR_WIDTH GLASS_WIDTH*ELEM_WIDTH
SCREEN_ELEM GlassScr[GLASS_HEIGHT*ELEM_HEIGHT * GLASS_SCR_WIDTH];

extern int NextFigN;
extern int CurFigN;
extern int X, Y;
extern FIGURE CurFigure;

//---------------------------------------------------------------------------
FIGURE Figures[FIGURE_COUNT][FIGURE_POS_CNT] =
{
	//------------------------------------ 1
	{
		{
			1, 4,
			{
				{ 1 },
				{ 1 },
				{ 1 },
				{ 1 }
			}
		},
		{
			4, 1,
			{
				{ 1,1,1,1 }
			}
		},
		{
			1, 4,
			{
				{ 1 },
				{ 1 },
				{ 1 },
				{ 1 }
			}
		},
		{
			4, 1,
			{
				{ 1,1,1,1 }
			}
		}
	},
	//------------------------------------ 2
	{
		{
			2, 2,
			{
				{ 2,2 },
				{ 2,2 }
			}
		},
		{
			2, 2,
			{
				{ 2,2 },
				{ 2,2 }
			}
		},
		{
			2, 2,
			{
				{ 2,2 },
				{ 2,2 }
			}
		},
		{
			2, 2,
			{
				{ 2,2 },
				{ 2,2 }
			}
		}
	},
	//------------------------------------ 3
	{
		{
			2, 3,
			{
				{ 3 },
				{ 3,3 },
				{ 3 }
			}
		},
		{
			3, 2,
			{
				{ 3,3,3 },
				{ 0,3 }
			}
		},
		{
			2, 3,
			{
				{ 0,3 },
				{ 3,3 },
				{ 0,3 }
			}
		},
		{
			3, 2,
			{
				{ 0,3 },
				{ 3,3,3 }
			}
		}
	},
	//------------------------------------ 4
	{
		{
			2, 3,
			{
				{ 4,4 },
				{ 0,4 },
				{ 0,4 }
			}
		},
		{
			3, 2,
			{
				{ 0,0,4 },
				{ 4,4,4 }
			}
		},
		{
			2, 3,
			{
				{ 4 },
				{ 4 },
				{ 4,4 }
			}
		},
		{
			3, 2,
			{
				{ 4,4,4 },
				{ 4 }
			}
		}
	},
	//------------------------------------ 5
	{
		{
			2, 3,
			{
				{ 5,5 },
				{ 5 },
				{ 5 }
			}
		},
		{
			3, 2,
			{
				{ 5,5,5 },
				{ 0,0,5 }
			}
		},
		{
			2, 3,
			{
				{ 0,5 },
				{ 0,5 },
				{ 5,5 }
			}
		},
		{
			3, 2,
			{
				{ 5 },
				{ 5,5,5 }
			}
		},
	},
	//------------------------------------ 6
	{
		{
			2, 3,
			{
				{ 0,6 },
				{ 6,6 },
				{ 6 }
			}
		},
		{
			3, 2,
			{
				{ 6,6 },
				{ 0,6,6 }
			}
		},
		{
			2, 3,
			{
				{ 0,6 },
				{ 6,6 },
				{ 6 }
			}
		},
		{
			3, 2,
			{
				{ 6,6 },
				{ 0,6,6 }
			}
		},
	},
	//------------------------------------ 7
	{
		{
			2, 3,
			{
				{ 7 },
				{ 7,7 },
				{ 0,7 }
			}
		},
		{
			3, 2,
			{
				{ 0,7,7 },
				{ 7,7 }
			}
		},
		{
			2, 3,
			{
				{ 7 },
				{ 7,7 },
				{ 0,7 }
			}
		},
		{
			3, 2,
			{
				{ 0,7,7 },
				{ 7,7 }
			}
		}
	}
};

int CurPos;

//---------------------------------------------------------------------------
FIGURE *GetFigure(int N)
{
	return &Figures[N][0];
}
//--------------------------------------------------------------------
void SetCurFigure()
{
	memcpy(&CurFigure, GetFigure(CurFigN), sizeof(FIGURE));
	CurPos = 0;
}
//--------------------------------------------------------------------
void Rotate(FIGURE &Temp)
{
	int p = (CurPos + 1) & 3;
	memcpy(&Temp, &Figures[CurFigN][p], sizeof(FIGURE));
}
//---------------------------------------------------------------------------
void SaveRotatedFig()
{
	CurPos = (CurPos + 1) & 3;
}
//---------------------------------------------------------------------------

void ClearGlass()
{
	memset(Glass, 0, sizeof(Glass));
}
//--------------------------------------------------------------------
bool CanPlaceFigure(FIGURE &Fig, int y, int x)
{
	if (x < 0    ||
		x + Fig.Width > GLASS_WIDTH ||
		y + Fig.Height > GLASS_HEIGHT)
			return false;

	int fystart, gystart;
	if (y < 0)
	{
		fystart = -y;
		gystart = 0;
	}
	else
	{
		fystart = 0;
		gystart = y;
	}

	//----?
	for (int fy = fystart, gy=gystart; fy < Fig.Height ; fy++, gy++)
		for (int fx = 0; fx < Fig.Width ; fx++)
			if (Fig.Figure[fy][fx] && Glass[gy][x+fx])
					return false;

	return true;
}
//--------------------------------------------------------------------
void StoreFigureElementInScrBuff(SCREEN_ELEM *ScrBuff, int w, int r, int c, byte p, COLORS clr)
{
	SCREEN_ELEM *start = ScrBuff + r*ELEM_HEIGHT*w + c*ELEM_WIDTH;

	for (int i = 0; i < ELEM_HEIGHT ; i++)
	{
		SCREEN_ELEM *s = start;

		for (int j = 0; j < ELEM_WIDTH ; j++)
		{
			s->Char = p;
			s->Color = clr;
			++s;
		}

		start += w;
	}
}
//--------------------------------------------------------------------
void SetFigureInGlass()
{
	int fystart, gystart;
	if (Y < 0)
	{
		fystart = -Y;
		gystart = 0;
	}
	else
	{
		fystart = 0;
		gystart = Y;
	}

	COLORS clr = FigColors[CurFigN];

	for (int fy = fystart, gy=gystart; fy < CurFigure.Height ; fy++, gy++)
		for (int fx = 0; fx < CurFigure.Width ; fx++)
		{
			byte p = CurFigure.Figure[fy][fx];

			if (p)
			{
				Glass[gy][X+fx] = p;

				StoreFigureElementInScrBuff(GlassScr, GLASS_SCR_WIDTH, gy, X+fx, FIG_ELEM, clr);
			}
		}
}
//--------------------------------------------------------------------

void DrawFigureElement(int y, int x, int r, int c, byte p, COLORS clr, int ElemHeight, int ElemWidth)
{
	textcolor(clr);
	
	for (int i = 0; i < ElemHeight ; i++, r)
	{
		gotoxy(x + c*ElemWidth, y + r*ElemHeight + i);

		for (int j = 0; j < ElemWidth ; j++)
			putch(p);
	}
}
//--------------------------------------------------------------------
void ClearFigureFromScreen()
{
	// restore saved figure background
	puttext(S_X + X*ELEM_WIDTH,
		S_Y + Y*ELEM_HEIGHT,
		S_X + (X + CurFigure.Width)*ELEM_WIDTH - 1,
		S_Y + (Y + CurFigure.Height)*ELEM_HEIGHT - 1,
		FigureBack);
}
//--------------------------------------------------------------------
void DrawFigureOnScreen()
{
	// save figure background
	gettext(S_X + X*ELEM_WIDTH,
			S_Y + Y*ELEM_HEIGHT,
			S_X + (X+CurFigure.Width)*ELEM_WIDTH - 1,
			S_Y + (Y+CurFigure.Height)*ELEM_HEIGHT - 1,
			FigureBack);

	COLORS clr = FigColors[CurFigN];

	for (int y = 0; y < CurFigure.Height ; y++)
		for (int x = 0; x < CurFigure.Width ; x++)
			if (CurFigure.Figure[y][x])
				DrawFigureElement(S_Y, S_X, Y+y, X+x, FIG_ELEM, clr, ELEM_HEIGHT, ELEM_WIDTH);

}
//--------------------------------------------------------------------
void ClearAndDrawFigure(int Y, int X, int FigN, int ElemHeight, int ElemWidth, COLORS clr)
{
	FIGURE *Fig = GetFigure(FigN);

	for (int y = 0; y < FIGURE_SIZE ; y++)
		for (int x = 0; x < FIGURE_SIZE ; x++)
		{
			if (Fig->Figure[y][x])
				DrawFigureElement(Y, X, y, x, FIG_ELEM, clr, ElemHeight, ElemWidth);
			else
				DrawFigureElement(Y, X, y, x, ' ', BLACK, ElemHeight, ElemWidth);
		}
}
//--------------------------------------------------------------------
void ShowGlass(int y, int x)
{
//clear 3 row above glass
	for (int i= -3*ELEM_HEIGHT ; i<0 ; ++i)
	{
		gotoxy(x, y + i);

		for (int j=0 ; j<GLASS_WIDTH*ELEM_WIDTH; ++j)
			putch(' ');
	}

		puttext(x,
				y,
				x + GLASS_WIDTH*ELEM_WIDTH - 1,
				y + GLASS_HEIGHT* ELEM_HEIGHT - 1,
				GlassScr);
}
//---------------------------------------------------------------------------
void SetEmptyGlassLine(int r)
{
	SCREEN_ELEM *s = &GlassScr[r*ELEM_HEIGHT*GLASS_SCR_WIDTH];


	for (int i=0 ; i<ELEM_HEIGHT ; ++i)

		for (int j=0 ; j<GLASS_WIDTH*ELEM_WIDTH ; ++j)

		{
			s->Char = FILLER;
			s->Color = YELLOW;
			++s;
		}
}
//---------------------------------------------------------------------------
int CheckGlass()
{
	int rows = 0;

	for (int y=GLASS_HEIGHT-1 ; y>=0 ; --y)
	{
		int x;
		for (x=0 ; x<GLASS_WIDTH ; ++x)
			if (Glass[y][x] == 0)
				break;

		if (x == GLASS_WIDTH)	// need delete row
		{
			++rows;

			memmove(&Glass[1][0], &Glass[0][0], y*GLASS_WIDTH);
			memset(&Glass[0][0], 0, GLASS_WIDTH);

//-------------------------------- delete row on screen
			memmove(&GlassScr[ELEM_HEIGHT*GLASS_SCR_WIDTH],
					&GlassScr[0],
					y*ELEM_HEIGHT*GLASS_SCR_WIDTH*sizeof(SCREEN_ELEM));

			SetEmptyGlassLine(0);

//-------------------------------- delete row on screen
			++y;
		}
	}

	return rows;
}
//---------------------------------------------------------------------------