LIBS=-L.\SDL2-2.28.4\x86_64-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2
INCLUDES=-I.\SDL2-2.28.4\x86_64-w64-mingw32\include\SDL2
CFLAGS=-std=c11 -Wall -Wextra -Werror

all:
	gcc chip8.c -o chip8 $(CFLAGS) $(LIBS) $(INCLUDES)

debug:
	gcc chip8.c -o chip8 -DDEBUG $(CFLAGS) $(LIBS) $(INCLUDES)