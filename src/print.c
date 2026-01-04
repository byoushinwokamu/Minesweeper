#include <stdio.h>
#include <stdlib.h>

#include "print.h"
#include "conio.h"
#include "global.h"

void print()
{
  gotoxy(0, 0);
  for (int r = ROWMIN; r <= ROWMAX; r++)
  {
    for (int c = COLMIN; c <= COLMAX; c++)
    {
      printf(rcur == r && ccur == c ? "<" : " ");

      switch (board[r][c])
      {
      case CELL_FLAG_MINE:
      case CELL_FLAG_NOMINE:
        printf(CLR_RED FLAG CLR_RESET);
        break;
      case CELL_CLOSED_MINE:
      case CELL_CLOSED_NOMINE:
        printf(CLOSED);
        break;
      case CELL_LAND:
        printf(LAND);
        break;
      case 1:
        printf(CLR_1 "1" CLR_RESET);
        break;
      case 2:
        printf(CLR_2 "2" CLR_RESET);
        break;
      case 3:
        printf(CLR_3 "3" CLR_RESET);
        break;
      case 4:
        printf(CLR_4 "4" CLR_RESET);
        break;
      case 5:
        printf(CLR_5 "5" CLR_RESET);
        break;
      case 6:
        printf(CLR_6 "6" CLR_RESET);
        break;
      case 7:
        printf(CLR_3 "7" CLR_RESET);
        break;
      case 8:
        printf(CLR_3 "8" CLR_RESET);
        break;
      default:
        printf("%d", board[r][c]);
      }

      printf(rcur == r && ccur == c ? ">" : " ");
    }
    printf("\n");
  }
  printf("Move with <h,j,k,l>, Plant a flag with <f>, Dig a cell with <d>, Autodig with <a>, Terminate "
         "with <t>\n");
  printf("Mines: %d/%d\n", minesnow, mines);
  printf("%s                            ", msg);
}

void print_gameover()
{
  system("clear");
  for (int r = ROWMIN; r <= ROWMAX; r++)
  {
    for (int c = COLMIN; c <= COLMAX; c++)
    {
      printf(" ");
      switch (board[r][c])
      {
      case CELL_CLOSED_MINE:
        printf(BG_RED MINE);
        break;
      case CELL_CLOSED_NOMINE:
        printf(CLOSED);
        break;
      case CELL_FLAG_MINE:
        printf(BG_RED FLAG);
        break;
      case CELL_FLAG_NOMINE:
        printf(BG_CYAN FLAG);
        break;
      case CELL_LAND:
        printf(LAND);
        break;
      case 1:
        printf(CLR_1 "1");
        break;
      case 2:
        printf(CLR_2 "2");
        break;
      case 3:
        printf(CLR_3 "3");
        break;
      case 4:
        printf(CLR_4 "4");
        break;
      case 5:
        printf(CLR_5 "5");
        break;
      case 6:
        printf(CLR_6 "6");
        break;
      case 7:
        printf(CLR_3 "7");
        break;
      case 8:
        printf(CLR_3 "8");
        break;
      default:
        printf("%d", board[r][c]);
      }
      printf(" " CLR_RESET);
    }
    printf("\n");
  }
  printf("Game over.\n");
  printf("Mines: %d/%d\n", minesnow, mines);
  printf("Restart with <r>, Terminate with <t>\n");
  int ch;
  while (ch = getch())
  {
    if (ch == 'r')
    {
      restart = true;
      gs = PLAY;
      break;
    }
    if (ch == 't')
    {
      gs = TERMINATE;
      break;
    }
  }
}

void print_complete()
{
  system("clear");
  for (int r = ROWMIN; r <= ROWMAX; r++)
  {
    for (int c = COLMIN; c <= COLMAX; c++)
    {
      printf(" ");
      switch (board[r][c])
      {
      case CELL_CLOSED_MINE:
        printf(BG_RED MINE);
        break;
      case CELL_CLOSED_NOMINE:
        printf(CLOSED);
        break;
      case CELL_FLAG_MINE:
        printf(BG_RED FLAG);
        break;
      case CELL_FLAG_NOMINE:
        printf(BG_CYAN FLAG);
        break;
      case CELL_LAND:
        printf(LAND);
        break;
      case 1:
        printf(CLR_1 "1");
        break;
      case 2:
        printf(CLR_2 "2");
        break;
      case 3:
        printf(CLR_3 "3");
        break;
      case 4:
        printf(CLR_4 "4");
        break;
      case 5:
        printf(CLR_5 "5");
        break;
      case 6:
        printf(CLR_6 "6");
        break;
      case 7:
        printf(CLR_3 "7");
        break;
      case 8:
        printf(CLR_3 "8");
        break;
      default:
        printf("%d", board[r][c]);
      }
      printf(" " CLR_RESET);
    }
    printf("\n");
  }
  printf("YOU FOUND ALL MINES!!!!!!!!!!!\n");
  printf("Mines: %d/%d\n", minesnow, mines);
  printf("Restart with <r>, Terminate with <t>\n");
  int ch;
  while (ch = getch())
  {
    if (ch == 'r')
    {
      restart = true;
      gs = PLAY;
      break;
    }
    if (ch == 't')
    {
      gs = TERMINATE;
      break;
    }
  }
}
