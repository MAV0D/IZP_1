CFLAGS = -std=c11 -Wall -Wextra -Werror

tnine: tnine.c
	CC ${CFLAGS} tnine.c -o tnine.exe

run: tnine
	.\tnine 11 < seznam.txt