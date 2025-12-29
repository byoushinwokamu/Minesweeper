#include "conio.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int rcur = 8, ccur = 15;
int mines = 0, minesnow = 0;
bool gameover = false;

#define ROWMIN 0
#define COLMIN 0
#define ROWMAX 15
#define COLMAX 29

#define MINE "★"
#define FLAG "†"
#define LAND "□"
#define CLOSED "■"

#define CELL_FLAG_NOMINE 9
#define CELL_FLAG_MINE 10
#define CELL_LAND 11
#define CELL_CLOSED_NOMINE 12
#define CELL_CLOSED_MINE 13

int board[ROWMAX + 1][COLMAX + 1];
char msg[100];

int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

int checkmine(int r, int c)
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

void print()
{
  system("clear");
  for (int r = 0; r < ROWMAX; r++)
  {
    for (int c = 0; c < COLMAX; c++)
    {
      printf(rcur == r && ccur == c ? "<" : " ");

      if (board[r][c] == CELL_FLAG_NOMINE || board[r][c] == CELL_FLAG_MINE)
        printf(FLAG);
      else if (board[r][c] == CELL_LAND)
        printf(LAND);
      else if (board[r][c] == CELL_CLOSED_NOMINE || board[r][c] == CELL_CLOSED_MINE)
        printf(CLOSED);
      else
        printf("%d", board[r][c]);

      printf(rcur == r && ccur == c ? ">" : " ");
    }
    printf("\n");
  }
  printf("Move with <h,j,k,l>, Plant a flag with <f>, Dig a cell with <d>, Terminate "
         "with <t>\n");
  printf("Mines: %d/%d\n", minesnow, mines);
  printf("%s", msg);
}

void flag(int r, int c)
{
  switch (board[r][c])
  {
  case CELL_CLOSED_MINE:
    board[r][c] = CELL_FLAG_MINE;
    break;
  case CELL_CLOSED_NOMINE:
    board[r][c] = CELL_FLAG_NOMINE;
    break;
  case CELL_FLAG_MINE:
    board[r][c] = CELL_CLOSED_MINE;
    break;
  case CELL_FLAG_NOMINE:
    board[r][c] = CELL_CLOSED_NOMINE;
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
    board[r][c] = checkmine(r, c);
    strcpy(msg, "Digged.");
    break;
  case CELL_FLAG_MINE:
  case CELL_FLAG_NOMINE:
  case CELL_LAND:
    strcpy(msg, "You can't dig on this cell.");
    break;
  }
}

void initgame()
{
  // 지뢰100개 칸480개 = 확률 20.83%
  srand(time(NULL));
  for (int i = ROWMIN; i <= ROWMAX; i++)
    for (int j = COLMIN; j <= COLMAX; j++)
    {
      if (rand() % 100 < 20)
      {
        mines++;
        board[i][j] = CELL_CLOSED_MINE;
      }
      else
        board[i][j] = CELL_CLOSED_NOMINE;
    }

  strcpy(msg, "Game start");
}

bool viewmine = true;
void print_gameover()
{
  system("clear");
  for (int r = 0; r < ROWMAX; r++)
  {
    for (int c = 0; c < COLMAX; c++)
    {
      printf(" ");
      switch (board[r][c])
      {
      case CELL_CLOSED_MINE:
        printf(viewmine ? MINE : CLOSED);
        break;
      case CELL_CLOSED_NOMINE:
        printf(viewmine ? LAND : CLOSED);
        break;
      case CELL_FLAG_MINE:
        printf(viewmine ? MINE : FLAG);
        break;
      case CELL_FLAG_NOMINE:
        printf(viewmine ? LAND : FLAG);
        break;
      case CELL_LAND:
        printf(LAND);
        break;
      default:
        printf("%d", board[r][c]);
      }
      printf(" ");
    }
    printf("\n");
  }
  printf("Game over.\n");
  printf("Mines: %d/%d\n", minesnow, mines);
  printf("Change view with <v>, Terminate with <t>\n");
}

int main(int argc, char **argv)
{
  int ch = 0;
  initgame();
  print();
  while ((ch = getch()) != 't')
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
      dig(rcur, ccur);
      break;
    }
    if (gameover)
      break;
    print();
  }
  print_gameover();
  while ((ch = getch()) != 't')
  {
    if (ch == 'v')
      viewmine = !viewmine;
    print_gameover();
  }
  return 0;
}