//---------------------------------------------------------------------------
#ifndef FiguresH
#define FiguresH

#ifdef __BORLANDC__
#include <conio.h>
#else
#include "bor_conio.h"
#endif
//---------------------------------------------------------------------------
typedef unsigned char byte;

#define FIGURE_SIZE 4
#define FIGURE_COUNT 7
#define FIGURE_POS_CNT 4

typedef struct
{
	int Width, Height;
	byte Figure[FIGURE_SIZE][FIGURE_SIZE];
}
FIGURE;

#define ELEM_HEIGHT 2
#define ELEM_WIDTH  3

#define FIG_ELEM '\xDB'
#define FILLER   '.'

FIGURE *GetFigure(int N);
void SetCurFigure();
bool CanPlaceFigure(FIGURE &Fig, int y, int x);
void ClearFigureFromScreen();
void DrawFigureOnScreen();
void ClearAndDrawFigure(int Y, int X, int Fig, int ElemHeight, int ElemWidth, COLORS clr);
void SetFigureInGlass();
void ClearGlass();
void ShowGlass(int y, int x);
int CheckGlass();
void Rotate(FIGURE &Temp);
void SaveRotatedFig();

void SetEmptyGlassLine(int r);
//---------------------------------------------------------------------------
#endif
