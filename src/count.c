#include "count.h"
#include "global.h"

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

bool checkcomplete()
{
  int flagok = 0;
  int flagno = 0;
  for (int i = ROWMIN; i <= ROWMAX; i++)
    for (int j = COLMIN; j <= COLMAX; j++)
      switch (board[i][j])
      {
      case CELL_FLAG_MINE:
        flagok++;
        break;
      case CELL_FLAG_NOMINE:
        flagno++;
        break;
      }

  return (flagok == MINE_AMOUNT && flagno == 0);
}