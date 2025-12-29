#ifndef _MACRO_H_
#define _MACRO_H_

/////////////////////////////////////////////////

#define LEVEL3

// 초급 9x9 10개
#ifdef LEVEL1
#define ROW_SIZE 9
#define COL_SIZE 9
#define MINE_AMOUNT 10
#endif

// 중급 16x16 40개
#ifdef LEVEL2
#define ROW_SIZE 16
#define COL_SIZE 16
#define MINE_AMOUNT 40
#endif

// 고급 30x16 99개
#ifdef LEVEL3
#define ROW_SIZE 16
#define COL_SIZE 30
#define MINE_AMOUNT 99
#endif

/////////////////////////////////////////////////

#define ROWMIN 0
#define COLMIN 0
#define ROWMAX (ROW_SIZE - 1)
#define COLMAX (COL_SIZE - 1)
#define ROWINI (ROWMAX / 2)
#define COLINI (COLMAX / 2)

// *** 여기부터 그래픽 관련 설정이 추가되어야 합니다 ***
#define TILE_SIZE 32 // 타일 이미지의 픽셀 크기
#define SCREEN_WIDTH (COL_SIZE * TILE_SIZE)
#define SCREEN_HEIGHT (ROW_SIZE * TILE_SIZE)

// 상태값 매크로
#define CELL_FLAG_NOMINE 9
#define CELL_FLAG_MINE 10
#define CELL_LAND 11
#define CELL_CLOSED_NOMINE 12
#define CELL_CLOSED_MINE 13

#endif