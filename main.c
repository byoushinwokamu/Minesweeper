#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "conio.h"
#include "global.h"
#include "count.h"
#include "action.h"
#include "print.h"

int board[ROWMAX + 1][COLMAX + 1];
char msg[100];

int rcur, ccur;
int mines = 0, minesnow = 0;
bool gameover = false;
bool restart = false;
enum game_status gs;

const int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

void initboard(int ri, int ci)
{
  srand(time(NULL));
  do
  {
    mines = 0;
    for (int i = ROWMIN; i <= ROWMAX; i++)
      for (int j = COLMIN; j <= COLMAX; j++)
        board[i][j] = CELL_CLOSED_NOMINE;

    while (mines < MINE_AMOUNT)
    {
      int r = rand() % (ROWMAX + 1);
      int c = rand() % (COLMAX + 1);
      if (board[r][c] == CELL_CLOSED_MINE)
        continue;
      board[r][c] = CELL_CLOSED_MINE;
      mines++;
    }
  } while (board[ri][ci] != CELL_CLOSED_NOMINE || countmine(ri, ci) != 0);
  strcpy(msg, "Game start");
}

int main(int argc, char **argv)
{
  int ch = 0;
  do
  {
    bool gamestart = false;
    rcur = ROWINI, ccur = COLINI;
    minesnow = 0;
    gameover = false;
    initboard(ROWINI, COLINI);
    print();

    while (ch = getch())
    {
      printf("%d\n", ch);
      switch (ch)
      {
      case 'h':
        (ccur == COLMIN) ? ccur : ccur--;
        break;
      case 'j':
        (rcur == ROWMAX) ? rcur : rcur++;
        break;
      case 'k':
        (rcur == ROWMIN) ? rcur : rcur--;
        break;
      case 'l':
        (ccur == COLMAX) ? ccur : ccur++;
        break;
      case 'f':
        flag(rcur, ccur);
        break;
      case 'd':
        if (!gamestart)
          gamestart = true, initboard(rcur, ccur);
        dig(rcur, ccur);
        break;
      case 'a':
        autodig(rcur, ccur);
        break;
      case 't':
        gs = TERMINATE;
        break;
      }
      if (checkcomplete())
        gs = COMPLETE;
      if (gameover)
        gs = OVER;

      if (gs != PLAY)
        break;
      print();
    }

    if (gs == OVER)
      print_gameover();
    else if (gs == COMPLETE)
      print_complete();

    if (gs == TERMINATE)
      break;
  } while (restart);
  return 0;
}