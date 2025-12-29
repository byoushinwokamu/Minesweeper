#include "conio.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int rcur = 7, ccur = 14;
int mines = 0, minesnow = 0;
bool gameover = false;

#define ROWMIN 0
#define COLMIN 0
#define ROWMAX 15
#define COLMAX 29

#define MINE "★"
#define FLAG "†"
#define LAND " "
#define CLOSED "■"

#define CELL_FLAG_NOMINE 9
#define CELL_FLAG_MINE 10
#define CELL_LAND 11
#define CELL_CLOSED_NOMINE 12
#define CELL_CLOSED_MINE 13

// 글자색 정의
#define CLR_RESET "\033[0m"
#define CLR_RED "\033[31m"
#define CLR_GREEN "\033[32m"
#define CLR_YELLOW "\033[33m"
#define CLR_BLUE "\033[34m"
#define CLR_MAGENTA "\033[35m"
#define CLR_CYAN "\033[36m"
#define CLR_WHITE "\033[37m"

// 지뢰찾기 숫자별 256색 코드
#define CLR_1 "\033[38;5;27m"  // 파란색 (Blue)
#define CLR_2 "\033[38;5;28m"  // 어두운 초록색 (Dark Green)
#define CLR_3 "\033[38;5;160m" // 빨간색 (Red)
#define CLR_4 "\033[38;5;18m"  // 어두운 파란색 (Dark Blue)
#define CLR_5 "\033[38;5;88m"  // 갈색/진한 빨강 (Maroon)
#define CLR_6 "\033[38;5;30m"  // 청록색 (Teal)
#define CLR_7 "\033[38;5;0m"   // 검정색 (실제 7은 보통 검정, 요청대로 빨강시 160 사용)
#define CLR_8 "\033[38;5;242m" // 회색 (실제 8은 보통 회색, 요청대로 빨강시 160 사용)

// 배경색 포함 (지뢰 폭발 등 강조용)
// #define BG_RED "\033[48;5;160;38;5;15m"
#define BG_RED "\x1b[48;2;255;0;0m"
// #define BG_CYAN "\033[48;5;30;38;5;15m"
#define BG_CYAN "\x1b[48;2;30;138;155m"

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

void initgame()
{
  srand(time(NULL));

  for (int i = ROWMIN; i <= ROWMAX; i++)
    for (int j = COLMIN; j <= COLMAX; j++)
      board[i][j] = CELL_CLOSED_NOMINE;

  while (mines < 99)
  {
    int r = rand() % (ROWMAX + 1);
    int c = rand() % (COLMAX + 1);
    if (board[r][c] == CELL_CLOSED_MINE)
      continue;
    board[r][c] = CELL_CLOSED_MINE;
    mines++;
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
  printf("Change view with <v>, Restart with <r>, Terminate with <t>\n");
}

int main(int argc, char **argv)
{
  int ch = 0;
  while (true)
  {
    mines = 0, minesnow = 0, rcur = 7, ccur = 14;
    gameover = false;
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
      else if (ch == 'r')
      {
        gameover = false;
        break;
      }
      print_gameover();
    }
    if (gameover)
      break;
  }
  return 0;
}