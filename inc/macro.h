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

#define MINE "★"
#define FLAG "¶"
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

#endif