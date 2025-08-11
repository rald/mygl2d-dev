all: libmygl2d.a game

libmygl2d.a: src/mygl2d.c include/mygl2d.h
	gcc -c  src/mygl2d.c -o obj/mygl2d.o -I include
	ar rcs lib/libmygl2d.a obj/mygl2d.o

game: src/main.c src/mouse.c include/mouse.h
	gcc  src/main.c  src/mouse.c -o game -I include -L lib -lm -lmygl2d -lglfw -lGL -lGLU -lX11 -lXrandr -lminiaudio

clean:
	rm obj/*.o
	rm game
	rm lib/libmygl2d.a

