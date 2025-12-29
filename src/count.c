#include "count.h"
#include "macro.h"

extern int board[ROWMAX + 1][COLMAX + 1];
extern const int dx[8];
extern const int dy[8];

int countmine(int r, int c)
{
  int rr, cc;
  int m = 0;
  for (int i = 0; i < 8; i++)
  {
    rr = r + dx[i], cc = c + dy[i];
    if (rr < ROWMIN || rr > ROWMAX || cc < COLMIN || cc > COLMAX)
      continue;
    if (board[rr][cc] == CELL_FLAG_MINE || board[rr][cc] == CELL_CLOSED_MINE)
      m++;
  }
  return m;
}

int countflag(int r, int c)
{
  int rr, cc;
  int f = 0;
  for (int i = 0; i < 8; i++)
  {
    rr = r + dx[i], cc = c + dy[i];
    if (rr < ROWMIN || rr > ROWMAX || cc < COLMIN || cc > COLMAX)
      continue;
    if (board[rr][cc] == CELL_FLAG_MINE || board[rr][cc] == CELL_FLAG_NOMINE)
      f++;
  }
  return f;
}