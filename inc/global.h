#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "macro.h"
#include <stdbool.h>

extern int rcur, ccur;
extern int board[ROWMAX + 1][COLMAX + 1];
extern int mines, minesnow;
extern bool gameover;
extern bool restart;
enum game_status
{
    PLAY,
    COMPLETE,
    OVER,
    TERMINATE
};
extern enum game_status gs;
extern char msg[100];
extern const int dx[8];
extern const int dy[8];

#endif