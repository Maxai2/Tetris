//---------------------------------------------------------------------------
#ifndef windowH
#define windowH

#ifdef __BORLANDC__
#include <conio.h>
#else
#include "bor_conio.h"
#endif
//---------------------------------------------------------------------------

enum BOUND_TYPE { btNone, btSingle, btDouble };

typedef struct
{
	int Width, Height;
	COLORS BackColor, ForeColor;
	BOUND_TYPE BoundType;
	int X, Y;
	bool bShow;
	void *SavedBackGround;
	int XOffset, YOffset;
}
WINDOW;

WINDOW *CreateConWindow(int w, int h, COLORS back, COLORS fore, BOUND_TYPE bound);
void DeleteConWindow(WINDOW * &win);
void ShowConWindow(WINDOW *win, int x, int y);
void HideConWindow(WINDOW *win);
void SetConWindowXY(WINDOW *win, int x, int y);
//---------------------------------------------------------------------------
#endif
