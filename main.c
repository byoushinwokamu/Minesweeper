#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <SFML/System.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "global.h"
#include "action.h"
#include "count.h"

// --- 전역 변수 ---
int board[ROWMAX + 1][COLMAX + 1];
char msg[100];
int rcur, ccur;
int mines = 0, minesnow = 0;
bool gameover = false;
bool restart = false;
enum game_status gs;

// 터진 지뢰 위치 저장용
int exploded_r = -1;
int exploded_c = -1;

const int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

// --- 텍스처 리소스 변수 ---
sfTexture *tex_numbers[9]; // 1~8 저장 (0은 안씀)
sfTexture *tex_empty;      // TileEmpty.png (빈 땅, 0)
sfTexture *tex_unknown;    // TileUnknown.png (닫힌 칸)
sfTexture *tex_flag;       // TileFlag.png
sfTexture *tex_mine;       // TileMine.png (그냥 지뢰)
sfTexture *tex_exploded;   // TileExploded.png (터진 지뢰)

sfSprite *sprite;

// --- 리소스 로드 함수 ---
bool load_resources()
{
  char path[64];

  // 1. 숫자 타일 로드 (Tile1.png ~ Tile8.png)
  for (int i = 1; i <= 8; i++)
  {
    sprintf(path, "resource/Tile%d.png", i);
    tex_numbers[i] = sfTexture_createFromFile(path, NULL);
    if (!tex_numbers[i])
      return false;
  }

  // 2. 특수 타일 로드
  tex_empty = sfTexture_createFromFile("resource/TileEmpty.png", NULL);
  tex_unknown = sfTexture_createFromFile("resource/TileUnknown.png", NULL);
  tex_flag = sfTexture_createFromFile("resource/TileFlag.png", NULL);
  tex_mine = sfTexture_createFromFile("resource/TileMine.png", NULL);
  tex_exploded = sfTexture_createFromFile("resource/TileExploded.png", NULL);

  // 하나라도 로드 실패 시 false 반환
  if (!tex_empty || !tex_unknown || !tex_flag || !tex_mine || !tex_exploded)
    return false;

  return true;
}

// --- 초기화 (기존 로직) ---
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
  strcpy(msg, "Game Start");
}

// --- 그리기 함수 ---
void render(sfRenderWindow *window)
{
  sfRenderWindow_clear(window, sfBlack);

  for (int r = ROWMIN; r <= ROWMAX; r++)
  {
    for (int c = COLMIN; c <= COLMAX; c++)
    {
      int cell = board[r][c];
      sfTexture *current_tex = tex_unknown; // 기본값

      // ... (텍스처 선택 로직은 기존과 동일) ...
      if (gameover)
      {
        if (r == exploded_r && c == exploded_c)
          current_tex = tex_exploded;
        else if (cell == CELL_CLOSED_MINE)
          current_tex = tex_mine;
        else if (cell == CELL_FLAG_MINE)
          current_tex = tex_flag;
      }

      if (current_tex == tex_unknown)
      {
        if (cell >= 1 && cell <= 8)
          current_tex = tex_numbers[cell];
        else if (cell == CELL_LAND)
          current_tex = tex_empty;
        else if (cell == CELL_FLAG_NOMINE || cell == CELL_FLAG_MINE)
          current_tex = tex_flag;
        else if (cell == CELL_CLOSED_NOMINE || cell == CELL_CLOSED_MINE)
          current_tex = tex_unknown;
      }

      // 1. 텍스처 적용
      sfSprite_setTexture(sprite, current_tex, sfTrue);

      // [추가된 부분] 2. 이미지 크기에 맞춰 스케일 조정 (꽉 차게 만들기)
      sfVector2u texSize = sfTexture_getSize(current_tex);
      sfVector2f scale = {
          (float)TILE_SIZE / texSize.x,
          (float)TILE_SIZE / texSize.y};
      sfSprite_setScale(sprite, scale);

      // 3. 위치 잡고 그리기
      sfSprite_setPosition(sprite, (sfVector2f){c * TILE_SIZE, r * TILE_SIZE});
      sfRenderWindow_drawSprite(window, sprite, NULL);
    }
  }
  sfRenderWindow_display(window);
}

int main()
{
  // 1. 윈도우 생성
  sfVideoMode mode = {SCREEN_WIDTH, SCREEN_HEIGHT, 32};
  sfRenderWindow *window = sfRenderWindow_create(mode, "Minesweeper", sfClose | sfTitlebar, NULL);
  sfRenderWindow_setFramerateLimit(window, 60);

  // 2. 리소스 로드
  sprite = sfSprite_create();
  if (!load_resources())
  {
    printf("Failed to load resources from 'resource/' folder.\n");
    return -1;
  }

  // 3. 게임 루프
  do
  {
    bool gamestart = false;
    minesnow = 0;
    gameover = false;
    restart = false;
    gs = PLAY;

    exploded_r = -1; // 초기화
    exploded_c = -1;

    // 보드 초기화 (화면만 닫힌 상태로)
    for (int i = ROWMIN; i <= ROWMAX; i++)
      for (int j = COLMIN; j <= COLMAX; j++)
        board[i][j] = CELL_CLOSED_NOMINE;

    while (sfRenderWindow_isOpen(window) && !restart)
    {
      sfEvent event;
      while (sfRenderWindow_pollEvent(window, &event))
      {
        if (event.type == sfEvtClosed)
        {
          sfRenderWindow_close(window);
          return 0;
        }

        // [수정된 부분] 마우스 클릭 처리
        if (event.type == sfEvtMouseButtonPressed && !gameover && gs != COMPLETE)
        {
          int c = event.mouseButton.x / TILE_SIZE;
          int r = event.mouseButton.y / TILE_SIZE;

          if (r >= ROWMIN && r <= ROWMAX && c >= COLMIN && c <= COLMAX)
          {

            // 1. 휠 클릭 (Middle) -> Autodig
            if (event.mouseButton.button == sfMouseMiddle)
            {
              autodig(r, c);
            }
            // 2. 왼쪽 클릭
            else if (event.mouseButton.button == sfMouseLeft)
            {
              // 오른쪽 버튼이 이미 눌려있다면 -> Autodig (좌우 동시 클릭)
              if (sfMouse_isButtonPressed(sfMouseRight))
              {
                autodig(r, c);
              }
              else
              {
                // 순수 왼쪽 클릭
                if (!gamestart)
                {
                  gamestart = true;
                  initboard(r, c);
                }
                if (board[r][c] == CELL_CLOSED_MINE)
                {
                  exploded_r = r;
                  exploded_c = c;
                }
                dig(r, c);
              }
            }
            // 3. 오른쪽 클릭
            else if (event.mouseButton.button == sfMouseRight)
            {
              // 왼쪽 버튼이 이미 눌려있다면 -> Autodig (좌우 동시 클릭)
              if (sfMouse_isButtonPressed(sfMouseLeft))
              {
                autodig(r, c);
              }
              else
              {
                // 순수 오른쪽 클릭 -> 깃발
                flag(r, c);
              }
            }
          }
        }

        // R키 재시작
        if (event.type == sfEvtKeyPressed)
        {
          if (event.key.code == sfKeyR)
          {
            restart = true;
          }
        }
      }

      if (checkcomplete())
        gs = COMPLETE;
      if (gameover)
        gs = OVER;

      render(window);
    }

  } while (restart);

  // 리소스 정리 (생략 가능하나 권장)
  sfSprite_destroy(sprite);
  sfRenderWindow_destroy(window);

  return 0;
}