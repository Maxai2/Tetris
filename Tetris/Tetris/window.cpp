//---------------------------------------------------------------------------
#pragma hdrstop

#include "window.h"
//---------------------------------------------------------------------------
WINDOW *CreateConWindow(int w, int h, COLORS back, COLORS fore, BOUND_TYPE bound)
{
	WINDOW *win = new WINDOW;

	win->Width = w + (bound == btNone ? 2 : 4);
	win->Height = h + (bound == btNone ? 2 : 4);
	win->XOffset = bound == btNone ? 1 : 2;
	win->YOffset = bound == btNone ? 1 : 2;
	win->BackColor = back;
	win->ForeColor = fore;
	win->BoundType = bound;
	win->X = win->Y = 0;
	win->bShow = false;
	win->SavedBackGround = 0;
	return win;
}
//---------------------------------------------------------------------------
void SetConWindowXY(WINDOW *win, int x, int y)
{
	if (win->bShow)
		gotoxy(win->X + win->XOffset + x, win->Y + win->YOffset + y);
}
//---------------------------------------------------------------------------
void DeleteConWindow(WINDOW * &win)
{
	if (!win)
		return;

	if (win->bShow)
		HideConWindow(win);

	if (win->SavedBackGround)
		delete [] win->SavedBackGround;

	delete win;
	win = 0;
}
//---------------------------------------------------------------------------
void HideConWindow(WINDOW *win)
{
	if (!win)
		return;

	if (!win->bShow)
		return;

	if (!win->SavedBackGround)
		return;

	puttext(win->X, win->Y, win->X + win->Width - 1, win->Y + win->Height - 1, win->SavedBackGround);

	delete [] win->SavedBackGround;
	win->SavedBackGround = 0;

	win->bShow = false;
}
//---------------------------------------------------------------------------
void ShowConWindow(WINDOW *win, int X, int Y)
{
	if (!win)
		return;

	HideConWindow(win);

	win->X = X;
	win->Y = Y;

	win->SavedBackGround = new char[win->Width * win->Height * 2];
	gettext(win->X, win->Y, win->X + win->Width - 1, win->Y + win->Height - 1, win->SavedBackGround);

//-- draw rect ----------------------

char ul, ur, bl, br, hl, vl;

//	ul - hl - ur
//  |         |
//  vl        vl
//  |         |
//	bl - hl - br

	if (win->BoundType == btNone)
	{
		ul = ur = bl = br = hl = vl = ' ';
	}
	else
	if (win->BoundType == btSingle)
	{
		ul = '\xDA';
		hl = '\xC4';
		ur = '\xBF';
		vl = '\xB3';
		bl = '\xC0';
		br = '\xD9';
	}
	else
//	if (win->BoundType == btDouble)
	{
		ul = '\xC9';
		hl = '\xCD';
		ur = '\xBB';
		vl = '\xBA';
		bl = '\xC8';
		br = '\xBC';
	}

	textcolor(win->ForeColor);
	textbackground(win->BackColor);

	int y = Y;
	gotoxy(X, y++);
	putch(ul);
	for (int j=0 ; j<win->Width-2 ; ++j)
		putch(hl);
	putch(ur);

	for (int i=0 ; i<win->Height-2 ; ++i, y++)
	{
		gotoxy(X, y);
		putch(vl);

		for (int j = 0; j<win->Width - 2; ++j)
			putch(' ');

		gotoxy(X+win->Width-1, y);
		putch(vl);
	}

	gotoxy(X, y++);
	putch(bl);
	for (int j=0 ; j<win->Width-2 ; ++j)
		putch(hl);
	putch(br);

	win->bShow = true;

//	window(win->X + win->XOffset, win->Y + win->YOffset,
//		  win->X + win->XOffset + win->Width - (win->BoundType == btNone ? 2 : 4),
//		  win->Y + win->YOffset + win->Height - (win->BoundType == btNone ? 2 : 4));
}
//---------------------------------------------------------------------------

