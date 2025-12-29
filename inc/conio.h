#ifndef _CONIO_H_
#define _CONIO_H_

#include <stdio.h>
#include <termio.h>

int getch(void);
void clrscr(void);
void gotoxy(short row, short col);

#endif