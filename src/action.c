#include <stdbool.h>
#include <string.h>

#include "action.h"
#include "macro.h"
#include "count.h"

extern int board[ROWMAX + 1][COLMAX + 1];
extern int mines, minesnow;
extern bool gameover;
extern char msg[100];
extern const int dx[8];
extern const int dy[8];

void flag(int r, int c)
{
  switch (board[r][c])
  {
  case CELL_CLOSED_MINE:
    board[r][c] = CELL_FLAG_MINE;
    minesnow++;
    break;
  case CELL_CLOSED_NOMINE:
    board[r][c] = CELL_FLAG_NOMINE;
    minesnow++;
    break;
  case CELL_FLAG_MINE:
    board[r][c] = CELL_CLOSED_MINE;
    minesnow--;
    break;
  case CELL_FLAG_NOMINE:
    board[r][c] = CELL_CLOSED_NOMINE;
    minesnow--;
    break;
  case CELL_LAND:
    strcpy(msg, "You can't plant a flag on this cell.");
    break;
  }
}

void dig(int r, int c)
{
  switch (board[r][c])
  {
  case CELL_CLOSED_MINE:
    gameover = true;
    break;
  case CELL_CLOSED_NOMINE:
    board[r][c] = countmine(r, c);
    if (board[r][c] == 0)
    {
      board[r][c] = CELL_LAND;
      int rr, cc;
      for (int i = 0; i < 8; i++)
      {
        rr = r + dx[i], cc = c + dy[i];
        if (rr < ROWMIN || rr > ROWMAX || cc < COLMIN || cc > COLMAX)
          continue;
        dig(rr, cc);
      }
    }
    strcpy(msg, "Digged.");
    break;
  case CELL_FLAG_MINE:
  case CELL_FLAG_NOMINE:
  case CELL_LAND:
    strcpy(msg, "You can't dig on this cell.");
    break;
  }
}

void autodig(int r, int c)
{
  int rr, cc;
  if ((board[r][c] != CELL_CLOSED_MINE && board[r][c] == !CELL_CLOSED_NOMINE) || countflag(r, c) != countmine(r, c))
  {
    strcpy(msg, "You can't autodig.");
    return;
  }
  for (int i = 0; i < 8; i++)
  {
    rr = r + dx[i], cc = c + dy[i];
    if (rr < ROWMIN || rr > ROWMAX || cc < COLMIN || cc > COLMAX)
      continue;
    if (board[rr][cc] == CELL_CLOSED_MINE || board[rr][cc] == CELL_CLOSED_NOMINE)
      dig(rr, cc);
  }
}