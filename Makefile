CC=gcc
TARGET=main
CFLAGS=-I inc
# 라이브러리 플래그 정의
LDFLAGS=-lcsfml-graphics -lcsfml-window -lcsfml-system

# 수정 1: wildcard 대신 필요한 파일만 직접 지정 (conio.c, print.c 제외)
SRC=src/action.c src/count.c
OBJ=$(SRC:.c=.o)

all: $(TARGET)

$(TARGET): main.c $(OBJ)
# 수정 2: $(LDFLAGS)를 명령어 맨 뒤로 이동
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJ)