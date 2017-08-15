//---------------------------------------------------------------------------
#ifndef TMainH
#define TMainH
//---------------------------------------------------------------------------
#define GLASS_WIDTH  10
#define GLASS_HEIGHT 20

#define S_X      5
#define S_Y      10

#define NEXT_X    45
#define NEXT_Y    15

#define STAT_X    60
#define STAT_Y    10

#define LEVEL_X    50
#define LEVEL_Y    3

#define MAX_LEVEL 9

//---------------------------------------------------------------------------

/*---- режимы отображения стакана для стирания фигуры или удаления строки -----*/

/* стакан перерисовывается путем отображения всех фигур */
#define GV_REBUILD_GLASS 1

/* стакан перерисовывается путем отображения экранного буфера где сохранены все изменения */
#define GV_USE_SCREEN_BUFFER 2

/* стакан перерисовывается только для удаления строки путем перемещения участка экрана
внимание - стакан не перерисовывается для движения фигуры т.е. нельзя использовать FM_REPAINT_GLASS
*/
#define GV_USE_SCREEN_RECT_MOVE 3

/*---- режимы движения фигуры -----*/

/* фигура движется путем стирания на экране фигуры по старым координатам и построения по новым */
#define FM_CLEAR_FIGURE 1

/* фигура движется путем переотображения стакана и построения фигуры по новым координатам */
#define FM_REPAINT_GLASS 2

/* фигура движется путем восстановления сохраненного участка экрана по старым координатам
и построения фигуры по новым координатам
*/
#define FM_SAVE_FIG_BACK 3

/*---- режимы рисования фигуры -----*/

/* фигура рисуется на непосредственно на экране */
#define FD_DRAW_ON_SCREEN 1

/* фигура рисуется на экранном буфере а затем переносится на экран */
#define FD_DRAW_ON_SCR_BUFF 2

//---------------------------------------------------------------------------

/*---- режим отображения стакана для стирания фигуры или удаления строки -----*/
//#define GV_GLASS_REPAINT 		GV_REBUILD_GLASS
#define GV_GLASS_REPAINT 		GV_USE_SCREEN_BUFFER
//#define GV_GLASS_REPAINT 		GV_USE_SCREEN_RECT_MOVE

/*---- режим движения фигуры -----*/
//#define FM_FIGURE_MOVE 		FM_CLEAR_FIGURE
//#define FM_FIGURE_MOVE 		FM_REPAINT_GLASS
#define FM_FIGURE_MOVE 			FM_SAVE_FIG_BACK

/*---- режим рисования фигуры -----*/
#define FD_FIGURE_DRAW 		FD_DRAW_ON_SCREEN
//#define FD_FIGURE_DRAW 		FD_DRAW_ON_SCR_BUFF

#if ((GV_GLASS_REPAINT == GV_USE_SCREEN_RECT_MOVE) && (FM_FIGURE_MOVE == FM_REPAINT_GLASS))
#error Not use GV_USE_SCREEN_RECT_MOVE && FM_REPAINT_GLASS together
#endif

/* использовать вращение матрицы фигуры или самому задать все 4 положения */
//#define FR_USE_ROTATE
//---------------------------------------------------------------------------
#endif

