#include "conio.h"

int getch(void) {
  int ch;
  struct termios buf, save;

  tcgetattr(0, &save);
  buf = save;

  buf.c_lflag &= ~(ICANON | ECHO);
  buf.c_cc[VMIN] = 1;
  buf.c_cc[VTIME] = 0;

  tcsetattr(0, TCSAFLUSH, &buf);
  ch = getchar();
  tcsetattr(0, TCSAFLUSH, &save);

  return ch;
}

void clrscr(void) { printf("%c[2J%c[1;1H", 27, 27); }

void gotoxy(short row, short col) { printf("%c[%d;%dH", 27, row + 1, col + 1); }